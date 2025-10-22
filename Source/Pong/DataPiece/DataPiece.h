
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataPiece.generated.h"

USTRUCT()
struct PONG_API FDataPiece
{
	GENERATED_BODY()

	TArray<double> InputState;
	int action;
	double reward;
	TArray<double> OutputState;
	bool Done;

	//FDataPiece(TArray<double> in, int act, double r, TArray<double> out, bool d) : InputState(in), action(act), reward(r), OutputState(out), Done(d){}

	FDataPiece() {
		InputState = {};
		action = -1;
		reward = 0.0f;
		OutputState = {};
		Done = false;
	}

	FDataPiece(TArray<double> in, int act, double r, TArray<double> out, bool d) {
		InputState = in;
		action = act;
		reward = r;
		OutputState = out;
		Done = d;
	}

	//void Build(TArray<double> in, int act, double r, TArray<double> out, bool d);
};
