// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHudWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UGameHudWidget::NativeConstruct()
{
	if(PauseBtn)
	{
		PauseBtn->OnClicked.AddDynamic(this,&UGameHudWidget::OnPauseClick);
	}

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(GetWorld(),0),this);
	
}


void UGameHudWidget::InitializeHUD(AEndlessRunnerCppGameModeBase* RunGameMode)
{
	if(RunGameMode)
	{
		CoinsCount->SetText(FText::AsNumber(0));
		LivesCount->SetText(FText::AsNumber(RunGameMode->MaxLives));
		
		RunGameMode->OnCoinsCountChanged.AddDynamic(this, &UGameHudWidget::SetCoinsCount);
		RunGameMode->OnLivesCountChanged.AddDynamic(this, &UGameHudWidget::SetLivesCount);
	}
}

void UGameHudWidget::SetCoinsCount(int32 Count)
{
	CoinsCount->SetText(FText::AsNumber(Count));
}

void UGameHudWidget::SetLivesCount(int32 Count)
{
	LivesCount->SetText(FText::AsNumber(Count));
}

void UGameHudWidget::OnPauseClick()
{
	if(IsValid(PauseMenuWidgetClass))
	{
		UGameplayStatics::SetGamePaused(GetWorld(),true);
		UUserWidget* Widget = CreateWidget(GetWorld(),PauseMenuWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}


