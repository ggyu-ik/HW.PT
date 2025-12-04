#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PTPlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class POLICEANDTHIEF_API APTPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
#pragma region Component
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCameraComponent> CameraComponent;
	
#pragma endregion
	
#pragma region Input
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
private:
	void MoveInput(const FInputActionValue& Value);
	
	void LookInput(const FInputActionValue& Value);
	
	void AttackInput(const FInputActionValue& Value);
#pragma endregion
	
#pragma region Attack
	
public:
	UFUNCTION(Server, Reliable)
	void ServerRPCCheckAttackHit();
	
	void CheckAttackHit();
	
	void OnHitByAttack();
private:
	UFUNCTION(Server, Reliable)
	void ServerRPCAttack();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCAttack();
	
	UFUNCTION()
	void OnRep_CanAttack();
	
	void PlayAttackMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCDie();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;
	
	bool bIsDead;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	float AttackMontagePlayTime;
#pragma endregion
};
