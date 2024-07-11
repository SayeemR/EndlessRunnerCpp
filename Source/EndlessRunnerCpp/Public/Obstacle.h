// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class ENDLESSRUNNERCPP_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	class UStaticMeshComponent* StaticMesh;

	
	UFUNCTION()
	void OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

};
