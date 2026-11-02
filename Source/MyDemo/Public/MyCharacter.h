// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"  //因为用到了FInputActionValue
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
//向前声明可以提高编译效率
class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;



UCLASS()
class MYDEMO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Component")
	TObjectPtr<USpringArmComponent> SpringArm;  //弹簧臂声明
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCameraComponent> CameraComponent;  //摄像机声明


	//增强输入声明,暴露给蓝图
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Input")
	TObjectPtr<class UInputMappingContext>DefaultMappingContext;  //输入映射上下文
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>MoveAction;  //上下左右移动
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>LookAction;  //上下左右转动视角
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>ZoomAction;  //缩放视角
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>JumpAction;  //跳跃
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>AttackAction;  //射线检测（攻击）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction>DodgeAction;  //闪避

	//闪避
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeStrength = 1500.0f;  //冲量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeCooldown = 0.8f;  //冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float InvincibleDuration = 0.4;  //无敌帧持续时间

	//攻击
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage = 20.0f;  //攻击伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRadius = 1000.0f;  //攻击距离

	//血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;  //最大血量
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;  //当前血量


	bool bIsInvincible = false;  //当前是否处于无敌状态
	float DodgeTimer = 0.0f;  //计算冷却的计时器
	FTimerHandle DodgeInvincibleTimerHandle;  //专门用来结束无敌状态的定时器句柄


	//增强输入的函数声明
	void Move(const FInputActionValue& Value);  //移动
	void Look(const FInputActionValue& Value);  //视角
	void Zoom(const FInputActionValue& Value);  //视角缩放
	void StartJump();  //开始跳跃
	void StopJump();  //结束跳跃
	void Attack();  //攻击
	void StartDodge();  //闪避
	void EndDogeInvincibility();  //结束无敌状态

	//重写扣血逻辑的父类
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
};
