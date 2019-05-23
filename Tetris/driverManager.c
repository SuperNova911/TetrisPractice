#include <fcntl.h>
#include <stdio.h>
#include <string.h>
//#include <Windows.h>

#include "dot_matrix_font.h"
#include "driverManager.h"

const unsigned char LED_CLEAR_VALUE = 0;
const int PUSH_SWITCH_NUMBER = 9;

int LEDFD;
int PushSwitchFD;
int DotMatrixFD;

unsigned char LED_Value;
unsigned char DotMatrix_Value[10][7];

// Open, Close Drivers
bool OpenDriver_kappa()
{
	ClearLED();
	ClearDotMatrix();

	return true;
}

void CloseDriver_kappa()
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
	LED_Value = 0;
}

// PushSwitch Management
size_t GetSwitchStatus(unsigned char statusBuffer[])
{
	const int SIZE = 9;
	memset(statusBuffer, 0, sizeof(unsigned char) * SIZE);

/*	statusBuffer[0] = GetAsyncKeyState(VK_NUMPAD1) != 0 ? 1 : 0;
	statusBuffer[1] = GetAsyncKeyState(VK_NUMPAD2) != 0 ? 1 : 0;
	statusBuffer[2] = GetAsyncKeyState(VK_NUMPAD3) != 0 ? 1 : 0;
	statusBuffer[3] = GetAsyncKeyState(VK_NUMPAD4) != 0 ? 1 : 0;
	statusBuffer[4] = GetAsyncKeyState(VK_NUMPAD5) != 0 ? 1 : 0;
	statusBuffer[5] = GetAsyncKeyState(VK_NUMPAD6) != 0 ? 1 : 0;
	statusBuffer[6] = GetAsyncKeyState(VK_NUMPAD7) != 0 ? 1 : 0;
	statusBuffer[7] = GetAsyncKeyState(VK_NUMPAD8) != 0 ? 1 : 0;
	statusBuffer[8] = GetAsyncKeyState(VK_NUMPAD9) != 0 ? 1 : 0;
*/
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

// DotMatrix Management
bool SetDotMatrix(int matrix[][7])
{
    int row, col;
	for (row = 0; row < 10; row++)
	{
		for (col = 0; col < 7; col++)
		{
			DotMatrix_Value[row][col] = matrix[row][col];
		}
	}

	return false;
}

bool SetDotMatrixByNumber(int value)
{
    int row;
	for (row = 0; row < 10; row++)
	{
		DotMatrix_Value[row][0] = (int)dot_matrix_font[value][row] & 0x1 << 6 ? 1 : 0;
		DotMatrix_Value[row][1] = (int)dot_matrix_font[value][row] & 0x1 << 5 ? 1 : 0;
		DotMatrix_Value[row][2] = (int)dot_matrix_font[value][row] & 0x1 << 4 ? 1 : 0;
		DotMatrix_Value[row][3] = (int)dot_matrix_font[value][row] & 0x1 << 3 ? 1 : 0;
		DotMatrix_Value[row][4] = (int)dot_matrix_font[value][row] & 0x1 << 2 ? 1 : 0;
		DotMatrix_Value[row][5] = (int)dot_matrix_font[value][row] & 0x1 << 1 ? 1 : 0;
		DotMatrix_Value[row][6] = (int)dot_matrix_font[value][row] & 0x1 << 0 ? 1 : 0;
	}

	return true;
}

void GetDotMatrix(unsigned char map[][7])
{
	memcpy(map, DotMatrix_Value, sizeof(DotMatrix_Value));
}

void ClearDotMatrix()
{
	memset(DotMatrix_Value, 0, sizeof(unsigned char) * 10 * 7);
}

void SetAllDotMatrix()
{
	memset(DotMatrix_Value, 1, sizeof(unsigned char) * 10 * 7);
}
