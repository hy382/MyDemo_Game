// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossCharacter.h"
#include "GameFramework/Character.h"  //使用Character里面的数据
#include "Kismet/GameplayStatics.h"  //寻找玩家
#include "GameFramework/CharacterMovementComponent.h"  //调整移动速度
#include "Engine/Engine.h"  //打印屏幕文字
#include "DrawDebugHelpers.h"  //使用DrawDebugline画线
#include "Engine/World.h"  //使用GetWorld()
#include "MyCharacter.h"
 

// Sets default values
AMyBossCharacter::AMyBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;  //设置最大移动速度

}

// Called when the game starts or when spawned
void AMyBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;  //开局当前血量等于最大血量
}


// Called every frame
void AMyBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);  //找到玩家
	if (!Player) return;  //提前返回防止代码越来越往右

	FVector PlayerLocation = Player->GetActorLocation();  //获取玩家位置
	FVector BossLocation = GetActorLocation();  //获取boss位置
	FVector Direction = PlayerLocation - BossLocation;  //获取朝向，为了让boss朝向玩家
	Direction.Z = 0.0f;  //把Z轴清零防止boos飞起来
	float Distance = Direction.Size();  //计算BOSS和玩家之间的距离
	if (Distance<=800.0f && Distance > 150.0f) {
		Direction.Normalize();  //把方向变为单位向量，防止速度因为距离变得忽快忽慢
		AddMovementInput(Direction, 1.0f);  //朝着Direction的方向以1.0的缩放力度移动
		SetActorRotation(Direction.Rotation());  //让BOSS面向玩家
	}
	if (AttackCooldownTimer > 0.0f) {  //冷却倒计时
		AttackCooldownTimer -= DeltaTime;
	}

	if (Distance <= AttackRadius && AttackCooldownTimer <= 0.0f) {
		SetActorRotation(Direction.Rotation());  //让BOSS面向玩家
		Attack();  //调用攻击函数
		AttackCooldownTimer = AttackInterval;  //攻击完立刻进入冷却
	}
}

// Called to bind functionality to input
void AMyBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//攻击
void AMyBossCharacter::Attack()
{
	FVector Start = GetActorLocation()+FVector(0,0,5);  //获取自己的当前位置
	FVector Forward = GetActorForwardVector();  //获取自己的朝向
	FVector End = Start + (Forward * AttackRadius);  //获取射线的长度

	FHitResult HitResult;  //获取命中结果

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);  //忽略自己
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, QueryParams);  //发射射线
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.0f, 0, 10.0f);
	if (bHit) {
		AActor* HitActor = HitResult.GetActor();  //获取被攻击的角色
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT(" %s"), *HitActor->GetName()));
		AMyCharacter* PlayerHit = Cast<AMyCharacter>(HitActor);  //检查被击中的角色是不是玩家=
		if (PlayerHit) {
			UGameplayStatics::ApplyDamage(PlayerHit, AttackDamage, GetController(), this, UDamageType::StaticClass());
			
		}
	}
}

//受到攻击
float AMyBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHealth <= 0) { return 0; }  //BOSS血量为零时不执行后面的程序，防止BOSS死亡后继续被攻击导致崩溃
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth -= DamageAmount;  //当前血量
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,FString::Printf(TEXT("BOSS收到 %.1f 点伤害，剩余血量 %.f"),DamageAmount,CurrentHealth));

	if (CurrentHealth <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BOSS死亡"));
		PrimaryActorTick.bCanEverTick = false;  //关闭tick，防止死后还在追玩家
		Destroy();  //销毁
	}
	return DamageAmount ;
}

