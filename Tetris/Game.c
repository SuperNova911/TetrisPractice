#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "driverManager.h"
#include "Tetris.h"
#include "Utility.h"

void UpdateDevice();
unsigned _stdcall Thread_Device(void* arg);

int main()
{
	OpenDriver_kappa();
	InitializeTetris();

	TickTimer timer;
	InitializeTickTimer(&timer, 10);

	//_beginthreadex(NULL, 0, Thread_Device, 0, 0, NULL);
	while (true)
	{
		WaitNextTick(&timer);
		UpdateTetris();
	}

	CloseDriver_kappa();

	return 0;
}

unsigned _stdcall Thread_Device(void* arg)
{
	while (true)
	{
		Sleep(10);
		UpdateDevice();
	}
}

void UpdateDevice()
{
	static int UPDATE_COUNTER = 0;

	unsigned char led = GetLED();
	unsigned char pushSwitch[9];
	GetSwitchStatus(pushSwitch);
	unsigned char dotMatrix[10][7];
	GetDotMatrix(dotMatrix);

	system("cls");
	printf("LED\n");
	for (int row = 1; row >= 0; row--)
	{
		for (int col = 3; col >= 0; col--)
		{
			if (led & 0x1 << (row * 4 + col))
			{
				printf("¡Ü");
			}
			else
			{
				printf("¡Û");
			}
		}
		printf("\n");
	}
	printf("\n");

	printf("PushSwitch\n");
	for (int index = 0; index < 9; index++)
	{
		printf("[%d:%d] ", index + 1, pushSwitch[index]);
	}
	printf("\n");
	printf("\n");

	printf("DotMatrix\n");
	for (int row = 0; row < 10; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			if (dotMatrix[row][col] == 0)
			{
				printf("¡Û");
			}
			else
			{
				printf("¡Ü");
			}
		}
		printf("\n");
	}
	printf("\n");
	printf("Update Counter: '%d'\n", UPDATE_COUNTER++);
}