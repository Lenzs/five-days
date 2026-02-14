#include "Player/PlayerStateComponent.h"

UPlayerStateComponent::UPlayerStateComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStateComponent::ChangeActive(EPlayerActivity NewActive)
{
   if (CurrentActivity == NewActive) return;
       
    EPlayerActivity OldActive = CurrentActivity;
    CurrentActivity = NewActive;
    OnActivityChanged.Broadcast(OldActive, NewActive);    
}

bool UPlayerStateComponent::IsWorking()
{
    if (CurrentActivity == EPlayerActivity::ReadingEmail || CurrentActivity == EPlayerActivity::ReplyingToEmail)
    {
        return true;
    }
    return false;
}
