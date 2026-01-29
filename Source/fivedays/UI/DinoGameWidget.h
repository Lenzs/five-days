#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DinoGameWidget.generated.h"

/**
 * UDinoGameWidget
 * 
 * This class controls the logic for the 2D Dino Jump minigame.
 * It is a "UserWidget", which is the base class for any UI screen in Unreal.
 * 
 * UNITY EQUIVALENT:
 * Think of this as a C# script attached to the root "Canvas" or "Panel" of your UI.
 */
UCLASS()
class FIVEDAYS_API UDinoGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// -- UNITY EQUIVALENT: Start() --
	// Called when the widget is first created and added to the screen/viewport.
	virtual void NativeConstruct() override;

	// -- UNITY EQUIVALENT: Update() --
	// Called every frame. We use this to update positions and physics.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Call this function (e.g., from an input event) to make the character jump.
	UFUNCTION(BlueprintCallable, Category = "Dino Game")
	void TriggerJump();

	// Resets the game state.
	UFUNCTION(BlueprintCallable, Category = "Dino Game")
	void StartGame();

protected:
	// -- UI BINDINGS --
	// "meta = (BindWidget)" tells Unreal to automatically link this variable
	// to a widget in the Designer with the EXACT SAME NAME.
	//
	// UNITY EQUIVALENT:
	// public Image DinoImage; // Drag and drop in Inspector
	
	UPROPERTY(meta = (BindWidget))
	class UImage* DinoImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* ObstacleImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScoreText; 

	// -- GAMEPLAY VARIABLES --
	bool bIsGameOver;
	bool bIsPlaying;

	float VerticalVelocity;
	float Gravity;
	float JumpForce;
	float CurrentScore;

	// Position tracking
	float DinoYPos;
	float GroundYPos; // The Y position where dino stands

	float ObstacleXPos;
	float ObstacleSpeed;
	float ObstacleStartX; // Spawn point (right)
	float ObstacleResetX; // Despawn point (left)

	// -- HELPER FUNCTIONS --
	void UpdateDinoPosition();
	void UpdateObstaclePosition(float DeltaTime);
	bool CheckCollision();
};
