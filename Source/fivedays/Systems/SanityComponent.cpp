#include "Systems/SanityComponent.h"

USanityComponent::USanityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USanityComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentState = CalculateState(CurrentSanity);
}

void USanityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ApplyPassiveDecay(DeltaTime);
}

void USanityComponent::ModifySanity(float Delta)
{
	float OldSanity = CurrentSanity;

	CurrentSanity = FMath::Clamp(CurrentSanity + Delta, USanityComponent::MinSanity, USanityComponent::MaxSanity);

	CheckThresholdCrossed(OldSanity, CurrentSanity);
}

void USanityComponent::ApplyPassiveDecay(const float DeltaTime)
{
	// TODO: We might want to check if player is idle here
	// For now, always decays
	ModifySanity(-PassiveDecayRate * DeltaTime);
}

ESanityState USanityComponent::CalculateState(const float SanityValue) const
{
	if (SanityValue > UneasyThreshold)
		return ESanityState::Normal;
	if (SanityValue > ParanoidThreshold)
		return ESanityState::Uneasy;
	if (SanityValue > BreakingThreshold)
		return ESanityState::Paranoid;
	return ESanityState::Insane;
}

void USanityComponent::CheckThresholdCrossed(const float OldSanity, const float NewSanity)
{
	const ESanityState OldState = CalculateState(OldSanity);
	const ESanityState NewState = CalculateState(NewSanity);

	if (OldState != NewState)
	{
		CurrentState = NewState;
		OnSanityThresholdCrossed.Broadcast(OldState, NewState);
	}
}