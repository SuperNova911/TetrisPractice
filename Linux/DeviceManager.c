#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "DeviceManager.h"
#include "DotAnimator.h"

int LEDFD;
int PushSwitchFD;
int DotMatrixFD;

// Open, Close Devices
bool OpenDevice()
{
    LEDFD = open("/dev/csemad_led", O_RDWR);
    if (LEDFD < 0)
    {
        printf("Failed to open LED driver, result: '%d'\n", LEDFD);
        return false;
    }

    PushSwitchFD = open("/dev/csemad_push_switch", O_RDONLY);
    if (PushSwitchFD < 0)
    {
        printf("Failed to open PushSwitch driver, result: '%d'\n", PushSwitchFD);
        return false;
    }

    DotMatrixFD = open("/dev/csemad_dot_matrix", O_WRONLY);
    if (DotMatrixFD < 0)
    {
        printf("Failed to open DotMatirx driver, result: '%d'\n", DotMatrixFD);
        return false;
    }

    return true;
}

void CloseDevice()
{
    close(LEDFD);
    close(PushSwitchFD);
    close(DotMatrixFD);
}

// LED Management
bool SetLED(unsigned char value)
{
    if (write(LEDFD, &value, sizeof(value)) != sizeof(value))
    {
        printf("Failed to set LED, value: '%d'\n", value);
        return false;
    }

    return true;
}

unsigned char GetLED()
{
    unsigned char currentValue = 0;

    if (read(LEDFD, &currentValue, sizeof(currentValue)) != sizeof(currentValue))
    {
        printf("Failed to get LED value\n");
    }

    return currentValue;
}

void ClearLED()
{
    int clearValue = LED_CLEAR_VALUE;
    if (write(LEDFD, &clearValue, sizeof(LED_CLEAR_VALUE)) != sizeof(LED_CLEAR_VALUE))
    {
        printf("Failed to clear LED\n");
    }
}

// PushSwitch Management
int GetSwitchStatus(unsigned char statusBuffer[])
{
    unsigned char buffer[PUSH_SWITCH_NUMBER];
    int readResult;

    readResult = read(PushSwitchFD, buffer, sizeof(buffer));

    if (readResult != sizeof(buffer))
    {
        printf("Failed to get PushSwitch status, read: '%d'bytes\n", readResult);
    }

    memcpy(statusBuffer, buffer, sizeof(buffer));

    return readResult;
}

bool IsSwitchOn(int switchNumber)
{
    unsigned char statusBuffer[PUSH_SWITCH_NUMBER];

    if (GetSwitchStatus(statusBuffer) != PUSH_SWITCH_NUMBER)
    {
        printf("Cannot get PushSwitch status\n");
        return false;
    }

    return statusBuffer[switchNumber] ? true : false;
}

// DotMatrix Management
void ConvertMatrixToFont(unsigned char matrix[][DOT_MATRIX_COL], unsigned char font[])
{
    int row, col;
    int power;

    memset(font, 0, sizeof(unsigned char) * DOT_MATRIX_ROW);
    
    for (row = 0; row < DOT_MATRIX_ROW; row++)
    {
        for (power = 0, col = DOT_MATRIX_COL - 1; col >= 0; power++, col--)
        {
            if (matrix[row][col] == 0)
            {
                continue;
            }

            font[row] += (unsigned char)pow(2, power);
        }
    }
}

void ConvertFontToMatrix(unsigned char font[], unsigned char matrix[][DOT_MATRIX_COL])
{

}

bool SetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL])
{
    unsigned char font[DOT_MATRIX_ROW];

    memset(font, 0, sizeof(font));
    ConvertMatrixToFont(matrix, font);

    if (write(DotMatrixFD, font, sizeof(font)) != sizeof(font))
    {
        printf("Failed to set DotMatrix\n");
        return false;
    }

    return true;
}

bool SetDotMatrixByNumber(int value)
{
    if (value < 0 || value > 9)
    {
        printf("Input value must be 0~9, value: '%d'\n", value);
        return false;
    }

    if (write(DotMatrixFD, DOT_BIG_NUMBER_FONT[value], sizeof(DOT_BIG_NUMBER_FONT[value])) != sizeof(DOT_BIG_NUMBER_FONT[value]))
    {
        printf("Failed to set DotMatrix, value: '%d'\n", value);
        return false;
    }

    return true;
}

void GetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL])
{

}

void ClearDotMatrix()
{
    write(DotMatrixFD, DOT_BLANK, sizeof(DOT_BLANK));
}

void SetAllDotMatrix()
{
    write(DotMatrixFD, DOT_FULL, sizeof(DOT_FULL));
}
