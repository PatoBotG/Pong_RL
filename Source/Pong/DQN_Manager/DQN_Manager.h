// DQN_Manager.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Pong/ReplayBuffer/ReplayBuffer.h"
#include "Pong/DataPiece/DataPiece.h"
#include "Pong/NeuralNetwork/NeuralNetwork.h"
#include "DQN_Manager.generated.h"

class AEnvironment;

UCLASS()
class PONG_API UDQN_Manager : public UObject
{
   GENERATED_BODY()

public:
    void Init(AEnvironment* InOwner);

    void Build(AActor* inplyr, int innum,TArray<int> inLayS, TArray<FString> inLayT, double ineps, double inedecay,double inesdecay, double inmine, double inLR, double inLRd, double inminLR,double ingamma, int inbufsiz, int inbatch,int trenew, int trainrt, int intrainsteps);

    void DQN();

    void DoAction(int act);

    void train();

    AActor* plyr;
    int num;

    NeuralNetwork PolicyNN, TargetNN;

    FReplayBuffer Buffer;

    double epsilon, epsilon_decay,epsilon_stepdecay, minepsilon;

    int N;
    double LRate, LRate_decay, minLRate;
    TArray<int> LayS;
    TArray<FString> LayT;

    double gamma;

    int BufferSize;
    int batch;

    int steps = 0;
    int TargetRenewRate;

    int TrainRate;
    int TotalTrainSteps;

private:
    AEnvironment* Environment; // pointer back to Class1
};
