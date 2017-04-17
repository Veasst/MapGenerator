// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGenerator.h"
#include "Tree.h"
#include "Rock.h"
#include "MapGeneratorActor.h"


// Sets default values
AMapGeneratorActor::AMapGeneratorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapGeneratorActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Potato"));

	for (int x = 0; x < LevelGridSize; x++)
	{
		for (int y = 0; y < LevelGridSize; y++)
		{
			ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), FName("ProceduralTile01"));
			ULevelStreaming* Level = CreateInstance(StreamingLevel, FString(FString::FromInt(x) + "x" + FString::FromInt(y)));

			float posX = x*TileSize - ((LevelGridSize * TileSize) / 2);
			float posY = y*TileSize - ((LevelGridSize * TileSize) / 2);

			Level->LevelTransform = FTransform(FVector(posX, posY, 0));
			Level->bShouldBeLoaded = true;
			Level->bShouldBeVisible = true;

			ScatterTrees(GetWorld(), posX, posY, 5, 5);
		}
	}
	
}


// Called every frame
void AMapGeneratorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapGeneratorActor::ScatterTrees(UWorld *world, int x, int y, int rows, int cols)
{
	float xd = (TileSize-200) / rows;
	float yd = (TileSize-200) / cols;
	for (int ix = 0; ix <rows; ix++)
	{
		for (int iy = 0; iy<cols; iy++)
		{
			float p = 1.f;

			if (TreeBlueprint && RockBlueprint)
			{
				if (ShouldHappen(10))
				{
					world->SpawnActor<ARock>(RockBlueprint, FVector(x + xd*ix + FMath::RandRange(-p, p)*xd, y + yd*iy + FMath::RandRange(-p, p)*yd, 0), FRotator::ZeroRotator);
				}
				else
				{
					world->SpawnActor<ATree>(TreeBlueprint, FVector(x + xd*ix + FMath::RandRange(-p, p)*xd, y + yd*iy + FMath::RandRange(-p, p)*yd, 0), FRotator::ZeroRotator);
				}
			}
		}
	}
}

bool AMapGeneratorActor::ShouldHappen(int percentage)
{
	return (FMath::RandRange(1, 100 / percentage) == 1 ? true : false);
}


ULevelStreaming* AMapGeneratorActor::CreateInstance(ULevelStreaming* Level, FString InstanceUniqueName)
{
	ULevelStreaming* StreamingLevelInstance = nullptr;

	UWorld* InWorld = Level->GetWorld();
	if (InWorld)
	{
		// Create instance long package name 
		FString InstanceShortPackageName = InWorld->StreamingLevelsPrefix + FPackageName::GetShortName(InstanceUniqueName);
		FString InstancePackagePath = FPackageName::GetLongPackagePath(Level->GetWorldAssetPackageName()) + TEXT("/");
		FName	InstanceUniquePackageName = FName(*(InstancePackagePath + InstanceShortPackageName));

		// check if instance name is unique among existing streaming level objects
		const bool bUniqueName = (InWorld->StreamingLevels.IndexOfByPredicate(ULevelStreaming::FPackageNameMatcher(InstanceUniquePackageName)) == INDEX_NONE);

		if (bUniqueName)
		{
			StreamingLevelInstance = NewObject<ULevelStreaming>(InWorld, Level->GetClass(), NAME_None, RF_Transient, NULL);
			// new level streaming instance will load the same map package as this object
			StreamingLevelInstance->PackageNameToLoad = (Level->PackageNameToLoad == NAME_None ? Level->GetWorldAssetPackageFName() : Level->PackageNameToLoad);
			// under a provided unique name
			StreamingLevelInstance->SetWorldAssetByPackageName(InstanceUniquePackageName);
			StreamingLevelInstance->bShouldBeLoaded = false;
			StreamingLevelInstance->bShouldBeVisible = false;
			StreamingLevelInstance->LevelTransform = Level->LevelTransform;

			// add a new instance to streaming level list
			InWorld->StreamingLevels.Add(StreamingLevelInstance);
		}
		else
		{
			UE_LOG(LogStreaming, Warning, TEXT("Provided streaming level instance name is not unique: %s"), *InstanceUniquePackageName.ToString());
		}
	}

	return StreamingLevelInstance;
}