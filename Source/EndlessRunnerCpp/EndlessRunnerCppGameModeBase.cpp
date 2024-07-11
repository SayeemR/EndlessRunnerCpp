// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerCppGameModeBase.h"

#include "FloorTile.h"
#include "GameHudWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void AEndlessRunnerCppGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(),0)->bShowMouseCursor = true;

	GameHud = Cast<UGameHudWidget>(CreateWidget(GetWorld(),GameHudClass));

	check(GameHud);
	GameHud->AddToViewport();
	GameHud->InitializeHUD(this);
	
	// GameHud->CoinsCount->SetText(FText::AsNumber(0));
	NumberOfLives = MaxLives;
	
	CreateInitialFloorTiles();

}


void AEndlessRunnerCppGameModeBase::CreateInitialFloorTiles()
{

	AFloorTile* Tile = AddFloorTile(false);
	if (Tile)
	{
		LaneSwitchValue.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValue.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValue.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	
	
	AddFloorTile(false);
	AddFloorTile(false);
	AddFloorTile(false);
	
	for (int i=0;i<NumInitialFloorTile;i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* AEndlessRunnerCppGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();

	if(World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass,NextSpawnPoint);
		if (Tile)	
		{
			FloorTiles.Add(Tile);
			if(bSpawnItems)
			{
				Tile->SpawnItems();
			}
			NextSpawnPoint  = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}



void AEndlessRunnerCppGameModeBase::AddCoin()
{
	TotalCoins +=1 ;
	// GameHud->CoinsCount->SetText(FText::AsNumber(TotalCoins));
	OnCoinsCountChanged.Broadcast(TotalCoins);
	// UE_LOG(LogTemp,Warning,TEXT("Total Coins : %d"),TotalCoins);
}

void AEndlessRunnerCppGameModeBase::PlayerDied()
{
	NumberOfLives -= 1;
	OnLivesCountChanged.Broadcast(NumberOfLives);

	if(NumberOfLives>0)
	{
		//iterate Over all floor tiles and destroy all floor tiles
		for (AFloorTile* Tile: FloorTiles)
		{
			if (Tile)
			{
				Tile->DestroyFloorTile();
			}
		}
		
		//Empty array
		FloorTiles.Empty();
		
		//Next Spawn Point to Initial Value
		NextSpawnPoint = FTransform();
		
		//Create our initial floor tile
		CreateInitialFloorTiles();
		//Broadcast level reset event
		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void AEndlessRunnerCppGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}

void AEndlessRunnerCppGameModeBase::GameOver()
{
	if(IsValid(GameOverScreenClass))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(),GameOverScreenClass);
		if(Widget)
		{
			Widget->AddToViewport();
		}
	}
}



