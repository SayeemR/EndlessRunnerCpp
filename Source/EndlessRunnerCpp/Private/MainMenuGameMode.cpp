// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(),0)->bShowMouseCursor = true;

	
	if(IsValid(MainMenuWidgetClass))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(),MainMenuWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
