// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "Core/PlayFabServerDataModels.h"
#include "Core/PlayFabServerAPI.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"

#include "Leaderboard.generated.h"

class APlayerBall;

UCLASS()
class ULeaderboard : public UObject
{
	GENERATED_BODY()
	
public:	
    UFUNCTION(BlueprintCallable, Category = "PlayFabDemo")
        static ULeaderboard* GetLeaderboardData();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGetLeaderboard, bool, getSuccess, FString, displayNames, FString, scores);
    UPROPERTY(BlueprintAssignable, Category = "BreakoutDemo")
        FOnGetLeaderboard COnGetLeaderboard;

    static void UpdateLeaderboard(FString playerId, unsigned score, APlayerBall* playerBall);

    void OnLeaderboardUpdateSuccess(const PlayFab::ServerModels::FUpdatePlayerStatisticsResult& result) const;
    void OnLeaderboardUpdateFailure(const PlayFab::FPlayFabError& ErrorResult) const;

    void OnGetLeaderboardSuccess(const PlayFab::ClientModels::FGetLeaderboardResult& result) const;
    void OnGetLeaderboardError(const PlayFab::FPlayFabError& ErrorResult) const;	

private:
    static const FString s_StatName;
    APlayerBall* m_playerBall;
};
