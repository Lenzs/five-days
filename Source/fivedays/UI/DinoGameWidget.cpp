#include "DinoGameWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UDinoGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize Tweakable Values
	Gravity = 2500.0f;
	JumpForce = 1200.0f;
	ObstacleSpeed = 500.0f;
	
	// Determine Ground Level based on where the Dino is placed in the Designer.
	if (DinoImage)
	{
		// We cast the Slot to UCanvasPanelSlot because we assume the parent is a CanvasPanel.
		if (UCanvasPanelSlot* DinoSlot = Cast<UCanvasPanelSlot>(DinoImage->Slot))
		{
			GroundYPos = DinoSlot->GetPosition().Y;
			DinoYPos = GroundYPos;
		}
	}

	// Setup Obstacle Initial Positions
	if (ObstacleImage)
	{
		if (UCanvasPanelSlot* ObstacleSlot = Cast<UCanvasPanelSlot>(ObstacleImage->Slot))
		{
			// Start off-screen to the right (approx 800-1000 units depending on screen size)
			// We can use the current position if the user placed it far right, or force it.
			ObstacleStartX = 1000.0f; 
			ObstacleResetX = -100.0f; // Off-screen to the left
			ObstacleXPos = ObstacleStartX;
		}
	}

	StartGame();
}

void UDinoGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsPlaying || bIsGameOver)
	{
		return;
	}

	// --- 1. JUMP PHYSICS ---
	// Apply Gravity
	VerticalVelocity -= Gravity * InDeltaTime;
	DinoYPos -= VerticalVelocity * InDeltaTime;

	// Floor Check
	if (DinoYPos >= GroundYPos)
	{
		DinoYPos = GroundYPos;
		VerticalVelocity = 0.0f;
	}

	UpdateDinoPosition();

	// --- 2. OBSTACLE MOVEMENT ---
	UpdateObstaclePosition(InDeltaTime);

	// --- 3. SCORING ---
	CurrentScore += InDeltaTime * 10.0f;
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber((int32)CurrentScore));
	}

	// --- 4. COLLISION ---
	if (CheckCollision())
	{
		bIsGameOver = true;
		
		if (ScoreText)
		{
			ScoreText->SetText(FText::FromString("GAME OVER"));
		}
		
		// Optional: Log to console
		UE_LOG(LogTemp, Warning, TEXT("Dino Hit Obstacle!"));
	}
}

void UDinoGameWidget::TriggerJump()
{
	if (bIsGameOver)
	{
		StartGame();
		return;
	}

	// Allow jump only if we are effectively on the ground
	// Tolerance of 5.0 units handles floating point imprecision
	if (FMath::IsNearlyEqual(DinoYPos, GroundYPos, 5.0f))
	{
		VerticalVelocity = JumpForce;
	}
}

void UDinoGameWidget::StartGame()
{
	bIsPlaying = true;
	bIsGameOver = false;
	VerticalVelocity = 0.0f;
	CurrentScore = 0.0f;

	// Reset positions
	DinoYPos = GroundYPos;
	ObstacleXPos = ObstacleStartX;

	UpdateDinoPosition();
	
	// Force obstacle update to move it to start immediately
	UpdateObstaclePosition(0.0f);
}

void UDinoGameWidget::UpdateDinoPosition()
{
	if (DinoImage)
	{
		if (UCanvasPanelSlot* DinoSlot = Cast<UCanvasPanelSlot>(DinoImage->Slot))
		{
			FVector2D CurrentPos = DinoSlot->GetPosition();
			// Keep X the same, update Y
			DinoSlot->SetPosition(FVector2D(CurrentPos.X, DinoYPos));
		}
	}
}

void UDinoGameWidget::UpdateObstaclePosition(float DeltaTime)
{
	if (ObstacleImage)
	{
		ObstacleXPos -= ObstacleSpeed * DeltaTime;

		// Loop obstacle if it goes off screen
		if (ObstacleXPos < ObstacleResetX)
		{
			ObstacleXPos = ObstacleStartX;
			// Increase difficulty?
			ObstacleSpeed += 10.0f; 
		}

		if (UCanvasPanelSlot* ObstacleSlot = Cast<UCanvasPanelSlot>(ObstacleImage->Slot))
		{
			FVector2D CurrentPos = ObstacleSlot->GetPosition();
			ObstacleSlot->SetPosition(FVector2D(ObstacleXPos, CurrentPos.Y));
		}
	}
}

bool UDinoGameWidget::CheckCollision()
{
	if (!DinoImage || !ObstacleImage) return false;

	// Simple AABB (Axis-Aligned Bounding Box) Collision
	// We use the Slot Position as the Top-Left corner and Size as dimensions.
	
	UCanvasPanelSlot* DinoSlot = Cast<UCanvasPanelSlot>(DinoImage->Slot);
	UCanvasPanelSlot* ObSlot = Cast<UCanvasPanelSlot>(ObstacleImage->Slot);

	if (!DinoSlot || !ObSlot) return false;

	FVector2D DinoPos = DinoSlot->GetPosition();
	FVector2D DinoSize = DinoSlot->GetSize();

	FVector2D ObPos = ObSlot->GetPosition();
	FVector2D ObSize = ObSlot->GetSize();

	// Create Box representations
	FBox2D DinoBox(DinoPos, DinoPos + DinoSize);
	FBox2D ObBox(ObPos, ObPos + ObSize);

	// Returns true if boxes overlap
	return DinoBox.Intersect(ObBox);
}
