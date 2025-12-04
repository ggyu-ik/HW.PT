#include "PTAttackBox.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Character/PTPlayerCharacter.h"

APTAttackBox::APTAttackBox()
{
	PrimaryActorTick.bCanEverTick = false;
    
	AttackBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	SetRootComponent(AttackBoxComponent);
	
	AttackBoxComponent->SetBoxExtent(BoxExtent);
	// AttackBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// AttackBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	// AttackBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	// AttackBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    
	AttackBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APTAttackBox::OnAttackBoxOverlap);
}

void APTAttackBox::BeginPlay()
{
	Super::BeginPlay();
    
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimeTimerHandle,
		this,
		&APTAttackBox::Destroy,
		LifeTime,
		false
	);
}

void APTAttackBox::OnAttackBoxOverlap(UPrimitiveComponent* OverlappedComp, 
									  AActor* OtherActor, 
									  UPrimitiveComponent* OtherComp, 
									  int32 OtherBodyIndex, 
									  bool bFromSweep, 
									  const FHitResult& SweepResult)
{
	// 유효성 체크
	if (!IsValid(OtherActor) || OtherActor == Attacker || HitActors.Contains(OtherActor))
	{
		return;
	}
    
	// PTPlayerCharacter 또는 다른 공격 가능한 캐릭터인지 체크
	APTPlayerCharacter* HitCharacter = Cast<APTPlayerCharacter>(OtherActor);
	if (IsValid(HitCharacter))
	{
		// 이미 처리된 대상으로 추가
		HitActors.Add(OtherActor);
        
		// 캐릭터의 사망 처리 함수 호출
		HitCharacter->OnHitByAttack();
	}
}