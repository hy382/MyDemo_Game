// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
//等需要用的时候在include
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
//增强输入
#include "EnhancedInputComponent.h"  //用于	BindAction
#include "EnhancedInputSubsystems.h"  //用于Subsystem->AddMappingContext
#include "InputAction.h"  //用于UInputAction的真实定义
#include "Engine/LocalPlayer.h"  //获取本地玩家
#include "GameFramework/CharacterMovementComponent.h"  //角色移动组件


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));  //创建弹簧臂
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));  //创建摄像机
	SpringArm->TargetArmLength = 300.0f; //弹簧臂默认长度为300
	SpringArm->bDoCollisionTest = true;  //开启碰撞测试，防止穿墙

	SpringArm->SetupAttachment(RootComponent);  //把弹簧臂挂载到根组件上
	CameraComponent->SetupAttachment(SpringArm);  //把摄像机挂载到弹簧臂上
	//让控制器的转动不影响角色的转动只影响摄像机的转动
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//让角色面朝我们加速度的方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//使用pawn的旋转
	SpringArm->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController())) {  //验证拿到的控制器是不是玩家控制器
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())){  //获取本地玩家的增强输入子系统
			Subsystem->AddMappingContext(DefaultMappingContext, 0);  //向子系统输入映射，告诉引擎现在按哪套规则读取玩家案件
		};
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)){  //验证输入类型是不是增强输入
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMyCharacter::Zoom);
}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();  //拿出二维向量
	if (Controller != nullptr) {
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);  //获取控制器旋转
		const FVector ForwarDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //把旋转转化为X轴的方向向量
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);  //把旋转转化为Y轴的方向向量

		AddMovementInput(ForwarDirection, MovementVector.Y);  //前后移动
		AddMovementInput(RightDirection, MovementVector.X);  //左右移动                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	}
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();  //拿出二维向量
	if (Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);  //左右转动视角  
		AddControllerPitchInput(LookAxisVector.Y);  //上下转动视角

	}
}

void AMyCharacter::Zoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();  //拿出浮点型的数
	if (ZoomValue != 0 && SpringArm != nullptr) {
		float NewLength = SpringArm->TargetArmLength - (ZoomValue * 50.0f);  //计算弹簧臂长度
		SpringArm->TargetArmLength = FMath::Clamp(NewLength, 100.0f, 800.0f);  //FMath::Clamp(我要检查的值，最小值，最大值）;
	}
}


