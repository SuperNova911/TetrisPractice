#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Control.h"
#include "TickTimer.h"

void InitializeQueue(InputInfoQueue* queue, int size)
{
	queue->Data = (InputInfo*)malloc(sizeof(InputInfo) * size);
	queue->Size = size;
	pthread_mutex_init(&queue->Lock, NULL);

	ClearQueue(queue);
}

void Enqueue(InputInfoQueue* queue, InputInfo data)
{
	if (IsQueueFull(queue) == true)
	{
		return;
	}

	pthread_mutex_lock(&queue->Lock);
	queue->Data[(queue->CurrentIndex + queue->Count) % queue->Size] = data;
	queue->Count++;
	pthread_mutex_unlock(&queue->Lock);
}

InputInfo Dequeue(InputInfoQueue* queue)
{
	InputInfo data;

	if (IsQueueEmpty(queue) == true)
	{
		return GenerateInputInfo(0, InputType_None);
	}

	pthread_mutex_lock(&queue->Lock);
	data = queue->Data[queue->CurrentIndex];
	queue->CurrentIndex = (queue->CurrentIndex + 1) % queue->Size;
	queue->Count--;
	pthread_mutex_unlock(&queue->Lock);

	return data;
}

void ClearQueue(InputInfoQueue * queue)
{
	pthread_mutex_lock(&queue->Lock);
	memset(queue->Data, 0, sizeof(InputInfo) * queue->Size);
	queue->Count = 0;
	queue->CurrentIndex = 0;
	pthread_mutex_unlock(&queue->Lock);
}

bool IsQueueEmpty(InputInfoQueue * queue)
{
	bool result;

	pthread_mutex_lock(&queue->Lock);
	result = queue->Count == 0;
	pthread_mutex_unlock(&queue->Lock);

	return result;
}

bool IsQueueFull(InputInfoQueue * queue)
{
	bool result;

	pthread_mutex_lock(&queue->Lock);
	result = queue->Count == queue->Size;
	pthread_mutex_unlock(&queue->Lock);

	return result;
}

InputInfo GenerateInputInfo(InputCommand command, InputType type)
{
	InputInfo info;
	info.Command = command;
	info.Type = type;

	return info;
}

void InitializeInputManager(InputManager* manager)
{
	ClearInputHistory(manager);
	InitializeQueue(&manager->InputQueue, 10);
}

void HandleInput(InputManager* manager, unsigned char inputData[INPUT_SOURCE_NUMBER])
{
	int index;
    int kappa;

	for (index = 0; index < INPUT_SOURCE_NUMBER; index++)
	{
		if (inputData[index] == 0)
		{
			if (manager->InputHistory[index] != 0)
			{
                manager->Delay[index] = GetTickCount();
				manager->InputHistory[index] = 0;
			}
		}
		else
		{
			if (manager->InputHistory[index] == 0)
			{
               
                if ((kappa = GetTickCount() - manager->Delay[index]) < 100)
                {
                    continue;
                }
                Enqueue(&manager->InputQueue, GenerateInputInfo(index, InputType_Click));
				manager->InputHistory[index] = GetTickCount();
			}
			else
			{
				continue;
			}
		}
	}
}

void ClearInputHistory(InputManager* manager)
{
	memset(manager->InputHistory, 0, sizeof(long) * INPUT_SOURCE_NUMBER);
    memset(manager->Delay, 0, sizeof(long) * INPUT_SOURCE_NUMBER);
}