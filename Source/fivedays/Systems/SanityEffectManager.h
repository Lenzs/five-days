// SanityEffectManager.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SanityState.h"
#include "SanityEffectManager.generated.h"

class USanityComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIVEDAYS_API USanityEffectManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USanityEffectManager();

protected:
	virtual void BeginPlay() override;

private:
	// Reference to Sanity Component attached
	UPROPERTY()
	USanityComponent* SanityComponent;

	// Current State
	ESanityState CurrentState = ESanityState::Normal;

	// Event Handlers
	UFUNCTION()
	void HandleThresholdCrossed(ESanityState OldState, ESanityState NewState);

};