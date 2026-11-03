// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBossCharacter.generated.h"


//枚举BOSS行为状态
UENUM(BlueprintType)
enum class EBossState :uint8 {
	Idle UMETA(DisplayName="发呆"),
	Chasing UMETA(DisplayName="追击"),
	Strafing UMETA(DisplayName="周旋"),
	MeleeAttack UMETA(DisplayName="近战"),
	MidAttack UMETA(DisplayName="中距离攻击"),
	RangerAttack UMETA(DisplayName="远程攻击")

};



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
	float ChaseRadius = 800.0f;  //追击半径为800
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MaxHealth = 100;  //最大血量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss")
	float CurrentHealth;  //当前血量

	//中距离攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MidAttackRadius = 500.0f;  //攻击距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MidAttackDamage = 15.0f;  //攻击伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MidAttackInterval = 1.5f;  //攻击间隔

	//远程攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float RangedAttackRadius = 800.0f;  //攻击距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float RangedAttackDamage = 25.0f;  //攻击伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float RangedAttackInterval = 3.0f;  //攻击间隔

	//近战攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MeleeAttackRadius = 200.0f;  //攻击距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MeleeAttackDamage = 30.0f;  //攻击伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	float MeleeAttackInterval = 1.0f;  //攻击间隔
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss AI")
	EBossState CurrentState = EBossState::Chasing;

	//BOSS周旋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss AI")
	float StrafeMinRadius = 200;  //最小周旋距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss AI")
	float StrafeMaxRadius = 500;  //最大周旋距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss AI")
	float StrafeSpeed = 200.0f;  //周旋速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss AI")
	float StrafeSwitchInterval = 2.0f;  //多久换一次绕圈方向



	void MeleeAttack();  //近战攻击
	void MidAttack();  //中距离攻击
	void RangedAttack();  //远程攻击
	void ChasePlayer(FVector Direction);  //追击玩家
	void StrafeAroundPlayer(FVector Direction, float DeltaTime);  //声明周旋的函数


	float MeleeAttackCooldownTimer = 0.0f;  //近战攻击冷却计时器
	float MidAttackCooldownTimer = 0.0f;  //中距离攻击冷却计时器
	float RangedAttackCooldownTimer = 0.0f;  //远程攻击冷却计时器
	float StrafeDirection = 1.0f;  //当前绕圈方向，1是顺时针，-1是逆时针
	float StrafeSwitchTimer = 0.0f;  //定时换转圈方向的定时器



	//重写扣血逻辑的父类
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

};
