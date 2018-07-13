// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"

#include "PlayerBall.generated.h"

UCLASS()
class APlayerBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void StartGame();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd, const FString&, TextToDisplay);
    UPROPERTY(BlueprintAssignable, Category = "BreakoutDemo")
        FOnGameEnd COnGameEnd;

    void OnLeaderboardUpdate(bool success);

private:
    void IncreaseScore();

    FVector m_velocity;
    bool m_gameStarted;
    ATextRenderActor* m_scoreText;
    ATextRenderActor* m_temporaryText;
    unsigned m_score;
    
    static const unsigned s_totalBricks;
};
