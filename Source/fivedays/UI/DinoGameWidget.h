#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "DinoGameWidget.generated.h"

UCLASS()
class FIVEDAYS_API UDinoGameWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    // Lifecycle functions
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // Call this from your Character/PlayerController to trigger a jump
    UFUNCTION(BlueprintCallable, Category = "Dino Game")
    void TriggerJump();

    UFUNCTION(BlueprintCallable, Category = "Dino Game")
    void StartGame();

protected:
    // UI Elements - Must create these in the Widget Blueprint with these exact names
    UPROPERTY(meta = (BindWidget))
    class UImage* DinoImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* ObstacleImage;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GameOverText;

private:
    // Game Logic Variables
    bool bIsGameActive;
    float VerticalVelocity;
    float Gravity;
    float JumpForce;
    float CurrentDinoY;
    float GroundY; // The Y position where the dino sits
    
    float ObstacleX;
    float ObstacleSpeed;
    
    float Score;

    void HandlePhysics(float DeltaTime);
    void HandleObstacle(float DeltaTime);
    bool CheckCollision();
};