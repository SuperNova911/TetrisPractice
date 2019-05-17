#include "driverManager.h"
#include "tetris.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int UpdateCounter = 0;

void UpdateDevice()
{
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
	printf("Update Counter: '%d'\n", UpdateCounter++);
}

int main()
{
	OpenDriver_kappa();

	BlockTile map[10][7];
	BlockDirection moveDirection = Down;

	unsigned char pushSwitch[9];
	SpawnNextBlock();

	while (1)
	{
		GetSwitchStatus(pushSwitch);
		if (pushSwitch[3] == true)
		{
			moveDirection = Left;
		}
		else if (pushSwitch[4] == true)
		{
			moveDirection = Down;
		}
		else if (pushSwitch[5] == true)
		{
			moveDirection =  Right;
		}
		else if (pushSwitch[7] == true)
		{
			moveDirection = Up;
		}
		MoveBlock(moveDirection);


		if (pushSwitch[6] == true)
		{
			RotateBlock(Left);
		}
		else if (pushSwitch[8] == true)
		{
			RotateBlock(Right);
		}

		DrawMap();
		GetMap(map);
		SetDotMatrix(map);

		UpdateDevice();
		GenerateBlockBag();

		Sleep(100);
	}

	CloseDriver_kappa();

	return 0;
}