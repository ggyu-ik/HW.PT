#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnBox.generated.h"

class USceneComponent;
class UBoxComponent;
class APTAICharacter;

UCLASS()
class POLICEANDTHIEF_API AAISpawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnBox();
	
	UFUNCTION()
	void SpawnAICharacter(int32 SpawnCount);
	
	FVector GetRandomSpawnLocation();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> SpawningBox;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APTAICharacter> AICharacterClass;
};
