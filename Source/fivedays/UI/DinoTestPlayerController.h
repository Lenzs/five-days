#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DinoTestPlayerController.generated.h"

UCLASS()
class FIVEDAYS_API ADinoTestPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ADinoTestPlayerController();

    // The widget class to spawn (set this in a Blueprint child or via code)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    TSubclassOf<class UDinoGameWidget> DinoWidgetClass;

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY()
    class UDinoGameWidget* DinoWidgetInstance;

    void OnJumpPressed();
};