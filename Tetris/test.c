//#include "driverManager.h"
//#include "tetris.h"
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <Windows.h>
//
//int UpdateCounter = 0;
//
//
//int main()
//{
//	OpenDriver_kappa();
//
//	BlockTile map[10][7];
//	BlockDirection moveDirection = BlockDirection_Down;
//
//	unsigned char pushSwitch[9];
//	SpawnNextBlock();
//
//	while (1)
//	{
//		GetSwitchStatus(pushSwitch);
//		if (pushSwitch[3] == true)
//		{
//			moveDirection = BlockDirection_Left;
//		}
//		else if (pushSwitch[4] == true)
//		{
//			moveDirection = BlockDirection_Down;
//		}
//		else if (pushSwitch[5] == true)
//		{
//			moveDirection = BlockDirection_Right;
//		}
//		else if (pushSwitch[7] == true)
//		{
//			moveDirection = BlockDirection_Up;
//		}
//		MoveBlock(moveDirection);
//
//
//		if (pushSwitch[6] == true)
//		{
//			RotateOrder(Rotate_Left);
//		}
//		else if (pushSwitch[8] == true)
//		{
//			RotateOrder(Rotate_Right);
//		}
//
//		DrawMap();
//		GetMap(map);
//		SetDotMatrix(map);
//
//		UpdateDevice();
//		GenerateBlockBag();
//
//		Sleep(100);
//	}
//
//	CloseDriver_kappa();
//
//	return 0;
//}