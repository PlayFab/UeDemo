// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// TODO Can we just include the UObject header instead?
#include "GameFramework/Actor.h"
//#include "UObject/Object.h"

#include "PlayFab.h"
#include "Core/PlayFabClientDataModels.h"
#include "Core/PlayFabClientAPI.h"

#include "PlayFabLogin.generated.h"

UCLASS()
class UPlayFabLogin : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "PlayFabDemo")
        static UPlayFabLogin* RegisterPlayFabUser(
            FString titleId, FString username, FString password, FString displayName, FString devSecret);

    UFUNCTION(BlueprintCallable, Category = "PlayFabDemo")
        static UPlayFabLogin* LoginWithPlayFabUsername(
            FString titleId, FString username, FString password, FString devSecret);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogin, bool, loginSuccess);
    UPROPERTY(BlueprintAssignable, Category = "BreakoutDemo")
        FOnLogin COnLogin;

private:
    void OnRegisterSuccess(const PlayFab::ClientModels::FRegisterPlayFabUserResult& result) const;
    void OnRegisterError(const PlayFab::FPlayFabError& ErrorResult) const;

    void OnLoginWithUsernameSuccess(const PlayFab::ClientModels::FLoginResult& result) const;
    void OnLoginError(const PlayFab::FPlayFabError& ErrorResult) const;
};
