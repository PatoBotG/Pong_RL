// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Pong/DQN_Manager/DQN_Manager.h"
#include "Environment.generated.h"

UCLASS()
class PONG_API AEnvironment : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnvironment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	double speed = 100.0;

	double RacketSpeed;

	UPROPERTY(EditAnywhere)
	double CollisionRayMod = 100.0;
	
	UPROPERTY(EditAnywhere, Category="Initial Settings")FVector initialdirection = FVector(1.0, 0.0, 0.0);
	UPROPERTY(EditAnywhere, Category = "Initial Settings") float y1;
	UPROPERTY(EditAnywhere, Category = "Initial Settings") float y2;
	UPROPERTY(EditAnywhere, Category = "Initial Settings") float x1;
	UPROPERTY(EditAnywhere, Category = "Initial Settings") float x2;

	UPROPERTY(EditAnywhere, Category = "Initial Settings") float RacketLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initial Settings") AActor* p1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initial Settings") AActor* p2;

	int goal = 0;

	int lasthit = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward Fuction") double StepReward = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward Fuction") double PositiveGoal = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward Fuction") double NegativeGoal = -1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward Fuction") double BallHitReward = 0.1;



	FVector direction;

	


	void BallMovement();

	void MoveRacket(AActor* p,float mod);

	void Bot1(AActor* p);

	void Restart();

	TArray<double> GetState(int num);
	bool GetReward(int num, double &reward);



	int p1goals = 0;
	int p2goals = 0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network Settings") TArray<int> LayS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Neural Network Settings") TArray<FString> LayT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double epsilon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double epsilonDecay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double EpsilonStepDecay = 0.001;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double minepsilon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double LR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double LRpow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double LRDecay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double minLR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") double gamma = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") int MaxBufferSize = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") int Batch = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") int TargetRenew = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") int TrainRate = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DQN Settings") int TrainningSteps = 100000;










private:
	UPROPERTY()
	UDQN_Manager* dqn;
};
