// Fill out your copyright notice in the Description page of Project Settings.

#include "Paddle.h"

#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"

const float APaddle::s_paddleSpeed = 500.0f;


// Sets default values
APaddle::APaddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    m_velocity = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();

    m_meshComponent = FindComponentByClass<UStaticMeshComponent>();
    if (InputComponent != nullptr)
    {
        InputComponent->BindAxis("MovePaddle", this, &APaddle::MovePaddle);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Failed to bind input component."));
    }
}

// Called every frame
void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (FGenericPlatformMath::Abs(m_velocity.X) > 0.0001f)
    {
        FVector position = GetActorLocation();
        FBox boundingBox = GetComponentsBoundingBox();
        FVector halfPaddleLength = FVector(boundingBox.Max.X - position.X, 0.0f, 0.0f);

        FVector newCenterOfPaddle = position + (m_velocity * DeltaTime);
        FVector endOfPaddle = newCenterOfPaddle +
            (m_velocity.X > 0.0f ? halfPaddleLength : -halfPaddleLength);

        FHitResult outHit;

        // Move the paddle if it does not collide with a side wall.
        if (!GetWorld()->LineTraceSingleByChannel(
            outHit, position, endOfPaddle, ECC_WorldStatic))
        {
            SetActorLocation(newCenterOfPaddle);
        }
    }
}

void APaddle::MovePaddle(float amount)
{
    m_velocity.X = amount * s_paddleSpeed;
}