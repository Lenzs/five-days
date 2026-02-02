// SanityComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/SanityState.h"
#include "SanityComponent.generated.h"

// Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityThresholdCrossed, ESanityState, OldState, ESanityState, NewState);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIVEDAYS_API USanityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USanityComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Events
	UPROPERTY(BlueprintAssignable)
	FOnSanityThresholdCrossed OnSanityThresholdCrossed;

	UFUNCTION(BlueprintCallable)
	void ModifySanity(const float Delta);

	UFUNCTION(BlueprintPure)
	float GetCurrentSanity() const { return CurrentSanity; }

	UFUNCTION(BlueprintPure)
	ESanityState GetCurrentState() const { return CurrentState; }

protected:
	virtual void BeginPlay() override;

private:
	// Sanity Value
	static constexpr float MinSanity = 0.0f;
	static constexpr float MaxSanity = 100.0f;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float CurrentSanity = MaxSanity;

	UPROPERTY(VisibleAnywhere)
	ESanityState CurrentState = ESanityState::Normal;

	// Passive Decay
	UPROPERTY(EditAnywhere)
	float PassiveDecayRate = 0.5f;  // Sanity lost per second while idle

	// Thresholds
	UPROPERTY(EditAnywhere)
	float UneasyThreshold = 70.0f;

	UPROPERTY(EditAnywhere)
	float ParanoidThreshold = 40.0f;

	UPROPERTY(EditAnywhere)
	float BreakingThreshold = 20.0f;

	// Internal Functions
	void ApplyPassiveDecay(const float DeltaTime);
	ESanityState CalculateState(const float SanityValue) const;
	void CheckThresholdCrossed(const float OldSanity, const float NewSanity);
};