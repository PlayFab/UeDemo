// Fill out your copyright notice in the Description page of Project Settings.

#include "Leaderboard.h"

#include "PlayFab.h"
#include "Core/PlayFabServerDataModels.h"
#include "Core/PlayFabServerAPI.h"

#include "PlayerBall.h"

const FString ULeaderboard::s_StatName = TEXT("Score");

void ULeaderboard::UpdateLeaderboard(FString playFabId, unsigned score, APlayerBall* playerBall)
{
    PlayFabServerPtr serverAPI = IPlayFabModuleInterface::Get().GetServerAPI();

    PlayFab::ServerModels::FStatisticUpdate statisticsUpdate;
    statisticsUpdate.StatisticName = s_StatName;
    statisticsUpdate.Value = score;

    PlayFab::ServerModels::FUpdatePlayerStatisticsRequest request;
    request.Statistics.Add(statisticsUpdate);
    request.PlayFabId = playFabId;

    ULeaderboard* uleaderboard = NewObject<ULeaderboard>();
    uleaderboard->m_playerBall = playerBall;

    serverAPI->UpdatePlayerStatistics(request, 
        PlayFab::UPlayFabServerAPI::FUpdatePlayerStatisticsDelegate::CreateUObject(uleaderboard, &ULeaderboard::OnLeaderboardUpdateSuccess), 
        PlayFab::FPlayFabErrorDelegate::CreateUObject(uleaderboard, &ULeaderboard::OnLeaderboardUpdateFailure)
    );
}

void ULeaderboard::OnLeaderboardUpdateFailure(const PlayFab::FPlayFabError& ErrorResult) const
{
    UE_LOG(LogTemp, Log, TEXT("UpdateStat failed."));
    m_playerBall->OnLeaderboardUpdate(false);
}


void ULeaderboard::OnLeaderboardUpdateSuccess(const PlayFab::ServerModels::FUpdatePlayerStatisticsResult& result) const
{
    UE_LOG(LogTemp, Log, TEXT("UpdateStat succeeded."));
    m_playerBall->OnLeaderboardUpdate(true);
}

void ULeaderboard::OnGetLeaderboardError(const PlayFab::FPlayFabError& ErrorResult) const
{
    // We should display a message and load the next level after this call fails.
    UE_LOG(LogTemp, Log, TEXT("Leaderboard call failed."));

    FString displayNamesText;
    FString scoresText;
    COnGetLeaderboard.Broadcast(false, displayNamesText, scoresText);
}

void ULeaderboard::OnGetLeaderboardSuccess(const PlayFab::ClientModels::FGetLeaderboardResult& result) const
{
    const int displayNameFieldLength = 12;
    FString displayNamesText = TEXT("");
    FString scoresText = TEXT("");

    for (PlayFab::ClientModels::FPlayerLeaderboardEntry leaderboardEntry : result.Leaderboard)
    {
        FString displayName = leaderboardEntry.DisplayName;
        int score = leaderboardEntry.StatValue;
        displayNamesText += displayName + TEXT("\n");
        scoresText += FString::FromInt(score) + TEXT("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("GetLeaderboard succeeded."));

    COnGetLeaderboard.Broadcast(true, displayNamesText, scoresText);
}

ULeaderboard* ULeaderboard::GetLeaderboardData()
{
    PlayFabClientPtr clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

    PlayFab::ClientModels::FGetLeaderboardRequest request;
    request.StartPosition = 0;
    request.StatisticName = s_StatName;

    ULeaderboard* uleaderboard = NewObject<ULeaderboard>();

    clientAPI->GetLeaderboard(request,
        PlayFab::UPlayFabClientAPI::FGetLeaderboardDelegate::CreateUObject(uleaderboard, &ULeaderboard::OnGetLeaderboardSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(uleaderboard, &ULeaderboard::OnGetLeaderboardError)
    );

    return uleaderboard;
}