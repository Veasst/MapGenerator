// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MapGeneratorActor.generated.h"

class ULevelStreaming;
class ATree;
class ARock;

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

	UPROPERTY(EditDefaultsOnly)
	int TreeChance = 97;

	UPROPERTY(EditDefaultsOnly)
	int RockChance = 10;

	UPROPERTY(EditDefaultsOnly)
	int RockyAreaChance = 5;

	UPROPERTY(EditDefaultsOnly)
	int RockDensity = 10;

	UPROPERTY(EditDefaultsOnly)
	int TreeDensity = 5;

	ULevelStreaming* CreateInstance(ULevelStreaming* Level, FString InstanceUniqueName);
	void ScatterObjects(UWorld *world, int x, int y, int rows, int cols);
	void ScatterRocks(UWorld * world, int x, int y, int rows, int cols);
	bool ShouldHappen(int percentage);

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ATree> TreeBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<ARock> RockBlueprint;
};
