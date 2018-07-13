// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBall.h"

#include "LoginInfo.h"
#include "Leaderboard.h"

#include "Components/InputComponent.h"

#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"

#define LOCTEXT_NAMESPACE "PlayerBall" 

const unsigned APlayerBall::s_totalBricks = 32;

// Sets default values
APlayerBall::APlayerBall() :
    m_gameStarted(false),
    m_score(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;    
}

// Called when the game starts or when spawned
void APlayerBall::BeginPlay()
{
	Super::BeginPlay();

    InputComponent->BindAction("TakeAction", IE_Released, this, &APlayerBall::StartGame);
    m_velocity = FVector(0.0f, 0.0f, 0.0f);

    // There must be a better way to get actors in a scene.
    for (TActorIterator<ATextRenderActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName().Equals(TEXT("ScoreText")))
        {
            m_scoreText = *ActorItr;
        }
        else if (ActorItr->GetName().Equals(TEXT("GameTemporaryText")))
        {
            m_temporaryText = *ActorItr;
        }
    }
}

// Called every frame
void APlayerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector newBallLocation = GetActorLocation();
    newBallLocation += m_velocity * DeltaTime;

    FBox boundingBox = GetComponentsBoundingBox();
    FVector newEdgeOfBall = FVector(
        m_velocity.X > 0.0f ? boundingBox.Max.X + m_velocity.X * DeltaTime :
        boundingBox.Min.X + m_velocity.X * DeltaTime,
        m_velocity.Y > 0.0f ? boundingBox.Max.Y + m_velocity.Y * DeltaTime :
        boundingBox.Min.Y + m_velocity.Y * DeltaTime,
        newBallLocation.Z
    );

    FHitResult outHit;

    // If the ball hits an object
    if (GetWorld()->LineTraceSingleByChannel(
        outHit, GetActorLocation(), newEdgeOfBall, ECC_WorldStatic))
    {
        UPrimitiveComponent* otherComponent = outHit.GetComponent();
        AActor* otherActor = outHit.GetActor();

        // Determine if the ball hit the ground.
        if (otherComponent->ComponentHasTag("BottomWall"))
        {
            // Update the leaderboard
            ULeaderboard::UpdateLeaderboard(LoginInfo::s_PlayFabId, m_score, this);

            return;
        }

        // Allow the paddle to influence the direction of the ball.
        if (otherActor->ActorHasTag("Paddle"))
        {
            m_velocity.Y = FGenericPlatformMath::Abs(m_velocity.Y);

            FVector ballLocation = GetActorLocation();
            FVector paddleLocation = otherActor->GetActorLocation();
            FVector diff = ballLocation - paddleLocation;

            const float paddleOffsetMultiplier = 5.0f;
            m_velocity.X = diff.X * paddleOffsetMultiplier;
        }
        // Determine if the ball hit the top wall
        else if (otherComponent->ComponentHasTag("TopWall"))
        {
            m_velocity.Y = -1000.0f;
        }
        // Change the direction of the ball
        else
        {
            FBox ballBox = GetComponentsBoundingBox();
            FBox otherActorBox = otherActor->GetComponentsBoundingBox();

            if (ballBox.Max.Y >= otherActorBox.Max.Y ||
                ballBox.Min.Y <= otherActorBox.Min.Y)
            {
                m_velocity.Y *= -1.0f;
            }

            else if (ballBox.Max.X >= otherActorBox.Max.X ||
                ballBox.Min.X <= otherActorBox.Min.X)
            {
                m_velocity.X *= -1.0f;
            }
        }

        // Determine if the ball hit a brick
        if (otherComponent->ComponentHasTag("Brick"))
        {
            otherActor->Destroy();
            IncreaseScore();

            if (m_score == s_totalBricks)
            {
                // Update the leaderboard
                ULeaderboard::UpdateLeaderboard(LoginInfo::s_PlayFabId, m_score, this);
            }
        }
    }


    SetActorLocation(newBallLocation);
}

void APlayerBall::IncreaseScore()
{
    ++m_score;
    UTextRenderComponent* textRenderComponent = m_scoreText->GetTextRender();
    if (textRenderComponent != nullptr)
    {
        FText text = FText::Format(LOCTEXT("ScoreText", "Score: {0}"), FText::AsNumber(m_score));
        textRenderComponent->SetText(text);
    }
}

void APlayerBall::StartGame()
{
    if (!m_gameStarted)
    {
        m_velocity = FVector(100.0f, 300.0f, 0.0f);
        m_gameStarted = true;

        // Reset the temporary text after the game starts.
        UTextRenderComponent* textRenderComponent = m_temporaryText->GetTextRender();
        if (textRenderComponent != nullptr)
        {
            FText text = FText::FromString(TEXT(""));
            textRenderComponent->SetText(text);
        }
    }
}

void APlayerBall::OnLeaderboardUpdate(bool success)
{
    if (success)
    {
        // Indicate the game is over - the blueprint will progress the game to the leaderboard.
        COnGameEnd.Broadcast(TEXT("Score successfully saved to PlayFab"));
    }
    else
    {
        // Indicate the game is over - the blueprint will progress the game to the leaderboard.
        COnGameEnd.Broadcast(TEXT("Score failed to save to PlayFab"));
    }

    // Destroy the ball
    Destroy();
}