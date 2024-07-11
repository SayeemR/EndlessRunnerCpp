// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"

#include "RunCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACoinItem::ACoinItem()
{
 	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("My Scene"));
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Collider"));
	SphereCollider->SetupAttachment(SceneComponent);

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Coin Mesh"));
	CoinMesh->SetupAttachment(SphereCollider);

	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
	RotatingMovement->RotationRate = FRotator(0,180,0);
}

// Called when the game starts or when spawned
void ACoinItem::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&ACoinItem::OnSphereOverlap);
}

void ACoinItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

	if(RunCharacter)
	{
		if(OverlapSound)
		{
			if(RunCharacter->bCanMove)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),OverlapSound,GetActorLocation());
			}
		}
		RunCharacter->AddCoin();

		this->Destroy();
	}
}


