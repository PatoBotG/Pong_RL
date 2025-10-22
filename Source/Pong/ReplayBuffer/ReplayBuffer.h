#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Pong/DataPiece/DataPiece.h"
#include "ReplayBuffer.generated.h"

USTRUCT()
struct PONG_API FReplayBuffer
{
	GENERATED_BODY()

	TArray<FDataPiece> Buffer;
	int index; //Apunta al último elemento añadido
	int max_size;
	int current_size;

	FReplayBuffer() {
		Buffer = {};
		index = 0;
		max_size = 0;
		current_size = 0;
	}


	void Build(int sz);

	void Add(FDataPiece data);

	TArray<FDataPiece> RandomSample(int batchlen);
};
