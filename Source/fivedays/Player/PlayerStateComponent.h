#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStateComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerActivity : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	ReadingEmail	UMETA(DisplayName = "ReadingEmail"),
	ReplyingToEmail	UMETA(DisplayName = "ReplyingToEmail"),
	PlayingGame		UMETA(DisplayName = "PlayingGame"),
	LookingAround	UMETA(DisplayName = "LookingAround"),
	StandingUp		UMETA(DisplayName = "StandingUp")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActityChanged, EPlayerActivity, OldActivity, EPlayerActivity, NewActivity);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIVEDAYS_API UPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Player State")
	EPlayerActivity CurrentActivity = EPlayerActivity::Idle;
	
public: 
	UPlayerStateComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Player State")
	void ChangeActive(EPlayerActivity NewActivity);
	
	UFUNCTION(BlueprintPure, Category = "Player State")
	EPlayerActivity GetCurrentActive() const { return CurrentActivity; }
	
	UFUNCTION(BlueprintPure, Category = "Player State")
	bool IsWorking();
	
	UPROPERTY(BlueprintAssignable)
	FOnActityChanged OnActivityChanged;
	
};