// SanityEffectManager.cpp
#include "Systems/SanityEffectManager.h"
#include "Systems/SanityComponent.h"

USanityEffectManager::USanityEffectManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USanityEffectManager::BeginPlay()
{
	Super::BeginPlay();

	SanityComponent = GetOwner()->FindComponentByClass<USanityComponent>();

	if (SanityComponent)
	{
		SanityComponent->OnSanityThresholdCrossed.AddDynamic(this, &USanityEffectManager::HandleThresholdCrossed);
	}
}

void USanityEffectManager::HandleThresholdCrossed(ESanityState OldState, ESanityState NewState)
{
	CurrentState = NewState;

	// TODO: Delete this
	FString const OldStateName = UEnum::GetValueAsString(OldState);	
	FString const NewStateName = UEnum::GetValueAsString(NewState);	
	FString Message = FString::Printf(TEXT("OldState: %s, NewState: %s"), *OldStateName, *NewStateName);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
	}	

	// TODO: Stop previous effects first

	// Start new effects based on state
	switch (NewState)
	{
		case ESanityState::Normal:
			// No effects
			break;
		case ESanityState::Uneasy:
			break;
		case ESanityState::Paranoid:
			break;
		case ESanityState::Insane:
			break;
	}
}
