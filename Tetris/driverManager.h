#pragma once

#include <stdio.h>
#include <stdbool.h>

#define LED_MAX 255

bool OpenDriver_kappa();
void CloseDriver_kappa();

bool SetLED(unsigned char value);
unsigned char GetLED();
void ClearLED();

size_t GetSwitchStatus(unsigned char statusBuffer[]);
bool IsSwitchOn(int switchNumber);

bool SetDotMatrix(int matrix[][7]);
bool SetDotMatrixByNumber(int value);
void GetDotMatrix(unsigned char map[][7]);
void ClearDotMatrix();
void SetAllDotMatrix();
