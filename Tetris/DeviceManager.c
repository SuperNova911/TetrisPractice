#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

#include "DeviceManager.h"
#include "DotAnimator.h"

int LEDFD;
int PushSwitchFD;
int DotMatrixFD;

unsigned char LED_Value;
unsigned char DotMatrix_Value[DOT_MATRIX_ROW][DOT_MATRIX_COL];

// Open, Close Drivers
bool OpenDevice()
{
	ClearLED();
	ClearDotMatrix();

	return true;
}

void CloseDevice()
{
}

// LED Management
bool SetLED(unsigned char value)
{
	LED_Value = value;
	return true;
}

unsigned char GetLED()
{
	return LED_Value;
}

void ClearLED()
{
	LED_Value = LED_CLEAR_VALUE;
}

// PushSwitch Management
int GetSwitchStatus(unsigned char statusBuffer[])
{
	const int SIZE = 9;
	memset(statusBuffer, 0, sizeof(unsigned char) * SIZE);

	statusBuffer[0] = GetAsyncKeyState(VK_NUMPAD1) != 0 ? 1 : 0;
	statusBuffer[1] = GetAsyncKeyState(VK_NUMPAD2) != 0 ? 1 : 0;
	statusBuffer[2] = GetAsyncKeyState(VK_NUMPAD3) != 0 ? 1 : 0;
	statusBuffer[3] = GetAsyncKeyState(VK_NUMPAD4) != 0 ? 1 : 0;
	statusBuffer[4] = GetAsyncKeyState(VK_NUMPAD5) != 0 ? 1 : 0;
	statusBuffer[5] = GetAsyncKeyState(VK_NUMPAD6) != 0 ? 1 : 0;
	statusBuffer[6] = GetAsyncKeyState(VK_NUMPAD7) != 0 ? 1 : 0;
	statusBuffer[7] = GetAsyncKeyState(VK_NUMPAD8) != 0 ? 1 : 0;
	statusBuffer[8] = GetAsyncKeyState(VK_NUMPAD9) != 0 ? 1 : 0;

	return SIZE;
}

bool IsSwitchOn(int switchNumber)
{
	unsigned char statusBuffer[9];

	if (GetSwitchStatus(statusBuffer) != PUSH_SWITCH_NUMBER)
	{
		printf("Cannot get PushSwitch status\n");
		return false;
	}

	return statusBuffer[switchNumber - 1] ? true : false;
}

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

// DotMatrix Management
bool SetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL])
{
	memcpy(DotMatrix_Value, matrix, sizeof(unsigned char) * DOT_MATRIX_ROW * DOT_MATRIX_COL);
	return true;
}

bool SetDotMatrixByNumber(int value)
{
    int row;
	for (row = 0; row < 10; row++)
	{
		DotMatrix_Value[row][0] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 6 ? 1 : 0;
		DotMatrix_Value[row][1] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 5 ? 1 : 0;
		DotMatrix_Value[row][2] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 4 ? 1 : 0;
		DotMatrix_Value[row][3] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 3 ? 1 : 0;
		DotMatrix_Value[row][4] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 2 ? 1 : 0;
		DotMatrix_Value[row][5] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 1 ? 1 : 0;
		DotMatrix_Value[row][6] = (int)DOT_BIG_NUMBER_FONT[value][row] & 0x1 << 0 ? 1 : 0;
	}

	return true;
}

void GetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL])
{
	memcpy(matrix, DotMatrix_Value, sizeof(DotMatrix_Value));
}

void ClearDotMatrix()
{
	memset(DotMatrix_Value, 0, sizeof(unsigned char) * 10 * 7);
}

void SetAllDotMatrix()
{
	memset(DotMatrix_Value, 1, sizeof(unsigned char) * 10 * 7);
}
