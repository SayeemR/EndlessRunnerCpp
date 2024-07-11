// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EndlessRunnerCpp/EndlessRunnerCppGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f,0.f,100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm,USpringArmComponent::SocketName);
	
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	const FVector Location = GetActorLocation();
	UWorld* World = GetWorld();
	
	if(World)
	{
		if(NotifySound)
		{
			UGameplayStatics::PlaySoundAtLocation(World,NotifySound,Location);
		}
	}
	
	bCanMove = true;

	RunGameMode = Cast<AEndlessRunnerCppGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);

	RunGameMode->OnLevelReset.AddDynamic(this,&ARunCharacter::ResetLevel);

	PlayerStart  = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(),APlayerStart::StaticClass()));
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValue[CurrentLane],RunGameMode->LaneSwitchValue[NextLane],Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::Death()
{
	
	if(!bIsDead)
	{
		const FVector Location = GetActorLocation();
		UWorld* World = GetWorld();
		
		if(World)
		{
			bCanMove = false;
			bIsDead = true;
			DisableInput(nullptr);
			if(DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World,DeathParticleSystem,Location);
			}
			if(DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World,DeathSound,Location);
			}
			GetMesh()->SetVisibility(false);
			World->GetTimerManager().SetTimer(RestartTimerHandle,this,&ARunCharacter::OnDeath,1.f);
		}
	}
}

void ARunCharacter::OnDeath()
{
	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	RunGameMode->PlayerDied();
	// UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(),TEXT("RestartLevel"));
}

void ARunCharacter::ResetLevel()
{

	bIsDead = false;
	bCanMove = true;
	EnableInput(nullptr);
	GetMesh()->SetVisibility(true);
	
	if(PlayerStart)
	{
		SetActorLocation(PlayerStart->GetActorLocation());
		SetActorRotation(PlayerStart->GetActorRotation());
	}
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane -1,0,2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane +1,0,2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	static  FVector Impulse = FVector(0,0,MoveDownImpulse);
	GetCharacterMovement()->AddImpulse(Impulse,true);
}


void ARunCharacter::AddCoin()
{
	if(bCanMove)
	{
		RunGameMode->AddCoin();
	}
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	if(bCanMove)
	{
		FRotator Rotator = GetControlRotation();
		Rotator.Pitch = 0.f;
		Rotator.Roll = 0.f;
		AddMovementInput(Rotator.Vector());
	}
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ARunCharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Left",IE_Pressed,this,&ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("Right",IE_Pressed,this,&ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("Down",IE_Pressed,this,&ARunCharacter::MoveDown);

}

