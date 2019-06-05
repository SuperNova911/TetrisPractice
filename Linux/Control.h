#pragma once

#include <pthread.h>
#include <stdbool.h>

#define INPUT_SOURCE_NUMBER 9

typedef enum InputCommand_e
{
	Input_Special = 6, Input_DropDown = 7, Input_Pause = 8,
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

InputInfo GenerateInputInfo(InputCommand command, InputType type);

typedef struct InputCollection_t
{
	InputInfo Input[INPUT_SOURCE_NUMBER];
} InputCollection;

typedef struct InputInfoQueue_t
{
	InputInfo* Data;
	int Size;
	int Count;
	int CurrentIndex;
	pthread_mutex_t Lock;
} InputInfoQueue;

void InitializeQueue(InputInfoQueue* queue, int size);

void Enqueue(InputInfoQueue* queue, InputInfo data);
InputInfo Dequeue(InputInfoQueue* queue);
void ClearQueue(InputInfoQueue* queue);

bool IsQueueEmpty(InputInfoQueue* queue);
bool IsQueueFull(InputInfoQueue* queue);

typedef struct InputManager_t
{
	long PushHistory[INPUT_SOURCE_NUMBER];
	InputType InputMode[INPUT_SOURCE_NUMBER];
	InputInfoQueue InputQueue;
} InputManager;

void InitializeInputManager(InputManager* manager);
void HandleInput(InputManager* manager, unsigned char inputData[INPUT_SOURCE_NUMBER]);
void ClearInputHistory(InputManager* manager);