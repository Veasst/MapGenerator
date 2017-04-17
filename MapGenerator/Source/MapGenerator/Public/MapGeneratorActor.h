// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MapGeneratorActor.generated.h"

class ULevelStreaming;

UCLASS()
class MAPGENERATOR_API AMapGeneratorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGeneratorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	int LevelGridSize = 9;

	UPROPERTY(EditDefaultsOnly)
	float TileSize = 400;

	ULevelStreaming* CreateInstance(ULevelStreaming* Level, FString InstanceUniqueName);
	
};
