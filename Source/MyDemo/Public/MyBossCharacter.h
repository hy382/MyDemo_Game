// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBossCharacter.generated.h"

class character;

UCLASS()
class MYDEMO_API AMyBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float ChaseRadius = 800.0f;  //×·»÷°ë¾¶Îª800
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MaxHealth = 100;  //×î´óÑªÁ¿
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	float CurrentHealth;  //µ±Ç°ÑªÁ¿
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float AttackRadius = 500.0f;  //¹¥»÷¾àÀë
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float AttackDamage = 15.0f;  //¹¥»÷ÉËº¦
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float AttackInterval = 1.5f;  //¹¥»÷¼ä¸ô

	void Attack();  //¹¥»÷
	float AttackCooldownTimer = 0.0f;  //ÀäÈ´¼ÆÊ±Æ÷

	//ÖØÐ´¿ÛÑªÂß¼­µÄ¸¸Àà
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

};
