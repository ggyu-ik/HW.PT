#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PTAttackBox.generated.h"

// PTAttackBox.h
UCLASS()
class POLICEANDTHIEF_API APTAttackBox : public AActor
{
	GENERATED_BODY()
    
public:
	APTAttackBox();
	
	virtual void BeginPlay() override;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> AttackBoxComponent;
    
	UFUNCTION()
	void OnAttackBoxOverlap(UPrimitiveComponent* OverlappedComp, 
						   AActor* OtherActor, 
						   UPrimitiveComponent* OtherComp, 
						   int32 OtherBodyIndex, 
						   bool bFromSweep, 
						   const FHitResult& SweepResult);
    
	// 공격을 시도한 캐릭터
	UPROPERTY()
	TObjectPtr<AActor> Attacker;
    
	// 이미 공격당한 대상들 (중복 방지)
	UPROPERTY()
	TArray<AActor*> HitActors;
    
	// 공격 박스 크기
	UPROPERTY(EditDefaultsOnly)
	FVector BoxExtent = FVector(50.0f, 50.0f, 50.0f);
    
	// 공격 박스가 존재하는 시간
	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 0.2f;
    
	FTimerHandle LifeTimeTimerHandle;
};