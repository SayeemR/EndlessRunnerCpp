// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "CoinItem.h"
#include "Obstacle.h"
#include "RunCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EndlessRunnerCpp/EndlessRunnerCppGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AFloorTile::AFloorTile()
{
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyScene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyFloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("MyAttachPoint"));
	AttachPoint->SetupAttachment(SceneComponent);

	
	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("MyCenterLane"));
	CenterLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("MyLeftLane"));
	LeftLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("MyRightLane"));
	RightLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MyFloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f,500.f,200.f));

	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}



// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerCppGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);
	
	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this,&AFloorTile::OnTriggerBoxOverlap);
	
}



void AFloorTile::SpawnItems()
{
	if(IsValid(SmallObstacleClass) && IsValid(BigObstacleClass) && IsValid(CoinItemClass))
	{
		static int32 NumBigs = 0;
		SpawnLaneItems(CenterLane, NumBigs);
		SpawnLaneItems(LeftLane, NumBigs);
		SpawnLaneItems(RightLane, NumBigs);
	}
}

void AFloorTile::SpawnLaneItems(UArrowComponent* Lane,int32& NumBigs)
{
	const float RandVal = FMath::RandRange(0.f,1.f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();
	
	if(UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent1,SpawnPercent2,true,true))
	{
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass,SpawnLocation,SpawnParameters);
		ChildActors.Add(Obstacle);
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent2,SpawnPercent3,true,true))
	{
		if(NumBigs <= 2)
		{
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass,SpawnLocation,SpawnParameters);
			if(Obstacle)
			{
				NumBigs += 1;
			}
			
			ChildActors.Add(Obstacle);
		}
		else
		{
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass,SpawnLocation,SpawnParameters);
			ChildActors.Add(Obstacle);
			NumBigs = 0;
		}
	}
	else if(UKismetMathLibrary::InRange_FloatFloat(RandVal,SpawnPercent3,1.f,true,true))
	{
		ACoinItem* Coin = GetWorld()->SpawnActor<ACoinItem>(CoinItemClass,SpawnLocation,SpawnParameters);
		ChildActors.Add(Coin);
	}	
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	
	if(RunCharacter)
	{
		RunGameMode->AddFloorTile(true);
		if(DestroyHandle.IsValid())
		{
			GetWorldTimerManager().SetTimer(DestroyHandle,this, & AFloorTile::DestroyFloorTile,1.f,false);
		}
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	for (auto Child : ChildActors)
	{
		if(Child)
		{
			Child->Destroy();
		}
	}

	ChildActors.Empty();
	
	RunGameMode->RemoveTile(this);
	
	this->Destroy();
}
