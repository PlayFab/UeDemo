// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Paddle.generated.h"

UCLASS()
class APaddle : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
        void MovePaddle(float amount);

    FVector m_velocity;

    UStaticMeshComponent* m_meshComponent;

    static const float s_paddleSpeed;
};
