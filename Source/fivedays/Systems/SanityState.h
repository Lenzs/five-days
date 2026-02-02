#pragma once

UENUM(BlueprintType)
enum class ESanityState : uint8
{
    Normal UMETA(DisplayName = "Normal"),     // 100-70
    Uneasy UMETA(DisplayName = "Uneasy"),     // 70-40
    Paranoid UMETA(DisplayName = "Paranoid"),   // 40-20
    Insane UMETA(DisplayName = "Insane")    // 20-0
};