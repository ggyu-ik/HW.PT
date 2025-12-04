#include "Spawn/AISpawnBox.h"

#include "Character/PTAICharacter.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"

AAISpawnBox::AAISpawnBox()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SpawningBox->SetupAttachment(SceneComponent);
	SpawningBox->SetBoxExtent(FVector(1400.0f, 1600.0f, 100.0f)); // 기본 크기 설정
	SpawningBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 스폰 박스는 충돌 불필요	
}

void AAISpawnBox::SpawnAICharacter(int32 SpawnCount)
{
	if (!IsValid(AICharacterClass) || !IsValid(SpawningBox))
	{
		return;
	}
	
	for (int32 i = 0; i < SpawnCount; i++)
	{
		FVector SpawnLocation = GetRandomSpawnLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		APTAICharacter* SpawnedCharacter = GetWorld()->SpawnActor<APTAICharacter>(
			AICharacterClass,
			SpawnLocation,
			SpawnRotation);
		
		if (IsValid(SpawnedCharacter))
		{
			SpawnedCharacter->SetReplicates(true);
			SpawnedCharacter->SetReplicateMovement(true);
		}
	}
}

FVector AAISpawnBox::GetRandomSpawnLocation()
{
	if (!IsValid(SpawningBox))
	{
		return GetActorLocation();
	}
	
	const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	const FVector BoxCenter = SpawningBox->GetComponentLocation();
	
	FVector RandomOffset = FVector(
		FMath::RandRange(-BoxExtent.X, BoxExtent.X),
		FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
		-10.0f
	);
	
	const FQuat BoxRotation = SpawningBox->GetComponentQuat();
	FVector WorldSpawnLocation = BoxCenter + BoxRotation.RotateVector(RandomOffset);
	
	return WorldSpawnLocation;
}