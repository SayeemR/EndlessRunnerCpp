// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNERCPP_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* ContinueBtn;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* RestartBtn;

	UFUNCTION()
	void OnContinueClick();
	
	UFUNCTION()
	void OnRestartClick();
	
	
	virtual  void NativeConstruct() override;
};
