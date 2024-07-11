// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

#include "RunCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AObstacle::AObstacle()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("My Scene"));
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh"));
	StaticMesh->SetupAttachment(SceneComponent);

	
	StaticMesh->SetSimulatePhysics(true);
	
	
}


void AObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter)
	{
		RunCharacter->Death();
	}
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentHit.AddDynamic(this, &AObstacle::OnObstacleHit);
}



