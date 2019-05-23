#pragma once

#define INPUT_SOURCE_NUMBER 9

typedef enum InputCommand_e
{
	Input_Pause = 6, Input_DropDown = 7, Input_Special = 8,
	Input_MoveLeft = 3, Input_MoveDown = 4, Input_MoveRight = 5,
	Input_RotateLeft = 0, Input_MoveUp = 1, Input_RotateRight = 2
} InputCommand;

typedef enum InputType_e
{
	InputType_None, InputType_Click, InputType_Hold
} InputType;

typedef struct InputInfo_t
{
	InputCommand Command;
	InputType Type;
} InputInfo;

typedef struct InputCollection_t
{
	InputInfo Input[INPUT_SOURCE_NUMBER];
} InputCollection;
