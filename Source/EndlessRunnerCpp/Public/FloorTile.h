// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"


class ACoinItem;
class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class AObstacle;

UCLASS()
class ENDLESSRUNNERCPP_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<AObstacle> SmallObstacleClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	TSubclassOf<ACoinItem> CoinItemClass;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<AActor*> ChildActors;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	float SpawnPercent1 = 0.1f;


	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Config")
	float SpawnPercent3 = 0.5f;
	
	UFUNCTION(BlueprintCallable)
	void SpawnItems();
	
	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return  AttachPoint->GetComponentTransform();
	}

	UFUNCTION(BlueprintCallable)
	void DestroyFloorTile();
	
	// Sets default values for this actor's properties
	AFloorTile();

protected:

	UPROPERTY()
	FTimerHandle DestroyHandle;
	
	UPROPERTY(VisibleInstanceOnly)
	class AEndlessRunnerCppGameModeBase* RunGameMode;

	UFUNCTION()
	void SpawnLaneItems(UArrowComponent* Lane,int32& NumBigs);	
	
	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	

	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
