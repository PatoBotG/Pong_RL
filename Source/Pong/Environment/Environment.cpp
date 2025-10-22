// Fill out your copyright notice in the Description page of Project Settings.


#include "Pong/Environment/Environment.h"

// Sets default values
AEnvironment::AEnvironment()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	dqn = CreateDefaultSubobject<UDQN_Manager>(TEXT("dqn"));

}

// Called when the game starts or when spawned
void AEnvironment::BeginPlay()
{
	Super::BeginPlay();
	
	
	LR = FMath::Exp(LRpow) * LR;
	minLR = FMath::Exp(LRpow) * minLR;

	RacketSpeed = (FMath::Sqrt(2.0) / 2.0) * speed;

	Restart();

	EpsilonStepDecay = (epsilon - minepsilon) / TrainningSteps;

	dqn->Init(this);
	dqn->Build(p2, 2,LayS,LayT, epsilon,epsilonDecay,EpsilonStepDecay,minepsilon,LR,LRDecay,minLR,gamma,MaxBufferSize,Batch,TargetRenew,TrainRate,TrainningSteps);
}

// Called every frame
void AEnvironment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Bot1(p1);

	//Bot1(p2);
	dqn->DQN();


	BallMovement();
}

// Called to bind functionality to input
void AEnvironment::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnvironment::Restart() {
	direction = initialdirection;

	FVector pos = GetActorLocation();
	pos.X = (x1 + x2) / 2;
	pos.Y = (y1 + y2) / 2;
	SetActorLocation(pos);

	if (p1)
	{
		UE_LOG(LogTemp, Warning, TEXT("P1 is: %s"), *p1->GetName());
		FVector pos1 = p1->GetActorLocation();
		pos1.X = x1;
		pos1.Y = (y1 + y2) / 2;
		p1->SetActorLocation(pos1);
	}

	if (p2)
	{
		UE_LOG(LogTemp, Warning, TEXT("P2 is: %s"), *p2->GetName());
		FVector pos2 = p2->GetActorLocation();
		pos2.X = x2;
		pos2.Y = (y1 + y2) / 2;
		p2->SetActorLocation(pos2);
	}
}

void AEnvironment::BallMovement(){

	//UE_LOG(LogTemp, Warning, TEXT("Ball Moving"));

	FHitResult hit, Hit;
	double dist = speed;
	int it = 0;
	while (dist > 0 and it<10) {
		it++;
		//UE_LOG(LogTemp, Warning, TEXT("Iteration: %d"), it);

		FVector pos = GetActorLocation();
		AddActorWorldOffset(direction * dist, 1, &Hit);
		if (Hit.GetActor() != NULL) {
			FVector dif = GetActorLocation() - pos;
			dist -= FMath::Sqrt(dif.X * dif.X + dif.Y * dif.Y);

			FVector vert = FVector(0.0, 1.0, 0.0)*2.0;
			FVector hor = FVector(1.0, 0.0, 0.0)*2.0;

			if (direction.X > 0) {
				FVector st1 = GetActorLocation() + (vert+hor)*CollisionRayMod;
				FVector st2 = GetActorLocation() + (-vert + hor) * CollisionRayMod;
				bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, st1, st1 + hor, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
				actorHit = actorHit or GetWorld()->LineTraceSingleByChannel(hit, st2, st2 + hor, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

				DrawDebugLine(GetWorld(), st1, st1 + hor, FColor::Red, false, 0.1f, 0.0f, 10.f);
				DrawDebugLine(GetWorld(), st2, st2 + hor, FColor::Red, false, 0.1f, 0.0f, 10.f);

				if (actorHit) {
					direction.X *= -1.0;
					UE_LOG(LogTemp, Warning, TEXT("Colision X"));
				}
			}

			else if (direction.X < 0) {
				FVector st1 = GetActorLocation() + (vert - hor) * CollisionRayMod;
				FVector st2 = GetActorLocation() + (-vert - hor) * CollisionRayMod;
				bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, st1, st1 - hor, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
				actorHit = actorHit or GetWorld()->LineTraceSingleByChannel(hit, st2, st2 - hor, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

				DrawDebugLine(GetWorld(), st1, st1 - hor, FColor::Red, false, 0.1f, 0.0f, 10.f);
				DrawDebugLine(GetWorld(), st2, st2 - hor, FColor::Red, false, 0.1f, 0.0f, 10.f);

				if (actorHit) {
					direction.X *= -1.0;
					UE_LOG(LogTemp, Warning, TEXT("Colision X"));
				}
			}

			if (direction.Y > 0) {
				FVector st1 = GetActorLocation() + (hor+vert) * CollisionRayMod;
				FVector st2 = GetActorLocation() + (-hor+vert) * CollisionRayMod;
				bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, st1, st1 + vert, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
				actorHit = actorHit or GetWorld()->LineTraceSingleByChannel(hit, st2, st2 + vert, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

				DrawDebugLine(GetWorld(), st1, st1 + vert, FColor::Red, false, 0.1f, 0.0f, 10.f);
				DrawDebugLine(GetWorld(), st2, st2 + vert, FColor::Red, false, 0.1f, 0.0f, 10.f);

				if (actorHit) {
					direction.Y *= -1.0;
					UE_LOG(LogTemp, Warning, TEXT("Colision Y"));
				}
			}

			else if (direction.Y < 0) {
				FVector st1 = GetActorLocation() + (hor - vert) * CollisionRayMod;
				FVector st2 = GetActorLocation() + (-hor - vert) * CollisionRayMod;
				bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, st1, st1 - vert, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
				actorHit = actorHit or GetWorld()->LineTraceSingleByChannel(hit, st2, st2 - vert, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());

				DrawDebugLine(GetWorld(), st1, st1 - vert, FColor::Red, false, 0.1f, 0.0f, 10.f);
				DrawDebugLine(GetWorld(), st2, st2 - vert, FColor::Red, false, 0.1f, 0.0f, 10.f);

				if (actorHit) {
					direction.Y *= -1.0;
					UE_LOG(LogTemp, Warning, TEXT("Colision Y"));
				}
			}
			
			if (Hit.GetActor() == p1 or Hit.GetActor() == p2) {
				if (Hit.GetActor() == p1) {
					lasthit = 1;
				}
				else if (Hit.GetActor() == p2) {
					lasthit = 2;
				}
				FVector Hpos = Hit.GetActor()->GetActorLocation();
				FVector bpos = GetActorLocation();
				double ydiff = -Hpos.Y + bpos.Y;
				double ang = (2 * ydiff / RacketLength) * 75;
				double rang = FMath::DegreesToRadians(ang);
				FVector ndir = FVector(FMath::Cos(rang), FMath::Sin(rang), 0.0);
				if (direction.X < 0) {
					ndir.X *= -1;
				}

				direction = ndir;
			}
			else {
				lasthit = 0;
			}

		}
		else {
			dist = 0;
		}
	}
	FVector pos = GetActorLocation();
	if (pos.X < x1) {
		goal = 2;
		p2goals++;
		Restart();
		UE_LOG(LogTemp, Warning, TEXT("Goal, current scoreboard: %d - %d"), p1goals, p2goals);
	}
	else if (pos.X > x2) {
		goal = 1;
		p1goals++;
		Restart();
		UE_LOG(LogTemp, Warning, TEXT("Goal, current scoreboard: %d - %d"), p1goals, p2goals);

	}
}

void AEnvironment::MoveRacket(AActor* p, float mod) {
	FVector dir = FVector(0.0, 1.0, 0.0);
	FHitResult hit;

	p->AddActorWorldOffset(dir * RacketSpeed*mod, 1, &hit);
}

void AEnvironment::Bot1(AActor* p) {
	FVector bpos = GetActorLocation();
	FVector pos = p->GetActorLocation();

	if (bpos.Y > pos.Y) {
		MoveRacket(p, 1.0);
	}
	else if (bpos.Y < pos.Y) {
		MoveRacket(p, -1.0);
	}
}

TArray<double> AEnvironment::GetState(int num) {
	TArray<double> ans;
	ans.SetNum(6); // Direction of ball (X,Y), position of ball (X,Y), height p1 (Y), height p2 (Y) , All coordinates normalized to the range (x2-x1) and (y2-y1)

	ans[0] = direction.X;
	ans[1] = direction.Y;

	FVector bpos = GetActorLocation();
	ans[2] = bpos.X / (x2 - x1);
	ans[3] = bpos.Y / (y2 - y1);

	FVector p1pos = p1->GetActorLocation();
	ans[4] = p1pos.Y / (y2 - y1);

	FVector p2pos = p2->GetActorLocation();
	ans[5] = p2pos.Y / (y2 - y1);

	if (num == 2) {
		ans[0] *= -1;
		ans[2] = 1.0 - ans[2];

		double h = ans[4];
		ans[4] = ans[5];
		ans[5] = h;
	}
	ans[2] = ans[2] * 2.0 - 1.0;
	ans[3] = ans[3] * 2.0 - 1.0;
	ans[4] = ans[4] * 2.0 - 1.0;
	ans[5] = ans[5] * 2.0 - 1.0;


	return ans;
}

bool AEnvironment::GetReward(int num, double &reward) {
	if (goal == 0) {
		reward = StepReward;
	}
	else if (goal == num) {
		reward = PositiveGoal;
		goal = 0;
		return 1;
	}
	else {
		reward = NegativeGoal;
		goal = 0;
		return 1;
	}

	if (lasthit == num) {
		reward+= BallHitReward;
	}
	
	return 0;
}

