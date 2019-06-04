#pragma once

#include <stdio.h>
#include <stdbool.h>

#define LED_MAX 255
#define LED_CLEAR_VALUE 0
#define PUSH_SWITCH_NUMBER 9
#define DOT_MATRIX_ROW 10
#define DOT_MATRIX_COL 7
#define DOT_MATRIX_SIZE (sizeof(unsigned char) * DOT_MATRIX_ROW * DOT_MATRIX_COL)

bool OpenDevice();
void CloseDevice();

bool SetLED(unsigned char value);
unsigned char GetLED();
void ClearLED();

int GetSwitchStatus(unsigned char statusBuffer[]);
bool IsSwitchOn(int switchNumber);

void ConvertMatrixToFont(unsigned char matrix[][DOT_MATRIX_COL], unsigned char font[]);
void ConvertFontToMatrix(unsigned char font[], unsigned char matrix[][DOT_MATRIX_COL]);

bool SetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL]);
bool SetDotMatrixByNumber(int value);
void GetDotMatrix(unsigned char matrix[][DOT_MATRIX_COL]);
void ClearDotMatrix();
void SetAllDotMatrix();
