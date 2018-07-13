// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayFabLogin.h"

#include "Core/PlayFabServerAPI.h"
#include "LoginInfo.h"

UPlayFabLogin* UPlayFabLogin::RegisterPlayFabUser(
    FString titleId, FString username, FString password, FString displayName, FString devSecret)
{
    // We don't use the server api here, but we do need to capture the dev secret.
    PlayFabServerPtr serverAPI = IPlayFabModuleInterface::Get().GetServerAPI();
    serverAPI->SetDevSecretKey(devSecret);

    PlayFabClientPtr clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
    clientAPI->SetTitleId(titleId);

    PlayFab::ClientModels::FRegisterPlayFabUserRequest request;
    request.TitleId = titleId;
    request.Username = username;
    request.Password = password;
    request.DisplayName = displayName;

    // This demo only uses username.
    request.RequireBothUsernameAndEmail = false;

    // Automatically login.
    request.LoginTitlePlayerAccountEntity = true;

    UPlayFabLogin* uPlayFabLogin = NewObject<UPlayFabLogin>();

    clientAPI->RegisterPlayFabUser(request,
        PlayFab::UPlayFabClientAPI::FRegisterPlayFabUserDelegate::CreateUObject(uPlayFabLogin, &UPlayFabLogin::OnRegisterSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(uPlayFabLogin, &UPlayFabLogin::OnRegisterError)
    );

    return uPlayFabLogin;
}

UPlayFabLogin* UPlayFabLogin::LoginWithPlayFabUsername(
    FString titleId, FString username, FString password, FString devSecret)
{
    // We don't use the server api here, but we do need to capture the dev secret.
    PlayFabServerPtr serverAPI = IPlayFabModuleInterface::Get().GetServerAPI();
    serverAPI->SetDevSecretKey(devSecret);

    PlayFabClientPtr clientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
    clientAPI->SetTitleId(titleId);

    PlayFab::ClientModels::FLoginWithPlayFabRequest request;
    request.Username = username;
    request.Password = password;


    UPlayFabLogin* uPlayFabLogin = NewObject<UPlayFabLogin>();

    clientAPI->LoginWithPlayFab(request,
        PlayFab::UPlayFabClientAPI::FLoginWithPlayFabDelegate::CreateUObject(uPlayFabLogin, &UPlayFabLogin::OnLoginWithUsernameSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(uPlayFabLogin, &UPlayFabLogin::OnLoginError)
    );

    return uPlayFabLogin;
}

void UPlayFabLogin::OnRegisterSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& result) const
{
    UE_LOG(LogTemp, Log, TEXT("Congratulations, you made your first successful API call!"));
    LoginInfo::s_PlayFabId = result.PlayFabId;

    COnLogin.Broadcast(true);
}

void UPlayFabLogin::OnRegisterError(const PlayFab::FPlayFabError& ErrorResult) const
{
    UE_LOG(LogTemp, Error, TEXT("Registration Failed.\nHere's some debug information:\n%s"), *ErrorResult.GenerateErrorReport());
    COnLogin.Broadcast(false);
}

void UPlayFabLogin::OnLoginWithUsernameSuccess(const PlayFab::ClientModels::FLoginResult& result) const
{
    UE_LOG(LogTemp, Log, TEXT("Login with username succeeded."));
    LoginInfo::s_PlayFabId = result.PlayFabId;

    COnLogin.Broadcast(true);
}

void UPlayFabLogin::OnLoginError(const PlayFab::FPlayFabError& ErrorResult) const
{
    UE_LOG(LogTemp, Error, TEXT("Login with username failed.\nHere's some debug information:\n%s"), *ErrorResult.GenerateErrorReport());
    COnLogin.Broadcast(false);
}