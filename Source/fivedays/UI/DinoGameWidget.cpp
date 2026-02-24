#include "DinoGameWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UDinoGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Default Configuration
    Gravity = 2000.0f;
    JumpForce = -900.0f; // Negative because Y=0 is top in UI
    GroundY = 0.0f;      // Will be set dynamically
    ObstacleSpeed = 600.0f;
    bIsGameActive = false;

    if (GameOverText) GameOverText->SetVisibility(ESlateVisibility::Hidden);

    // Capture the initial ground position from the designer view
    if (DinoImage)
    {
        UCanvasPanelSlot* DinoSlot = Cast<UCanvasPanelSlot>(DinoImage->Slot);
        if (DinoSlot)
        {
            GroundY = DinoSlot->GetPosition().Y;
            CurrentDinoY = GroundY;
        }
    }
}

void UDinoGameWidget::StartGame()
{
    bIsGameActive = true;
    Score = 0.0f;
    VerticalVelocity = 0.0f;
    ObstacleX = 1000.0f; // Start off-screen right
    
    if (GameOverText) GameOverText->SetVisibility(ESlateVisibility::Hidden);
    if (DinoImage) DinoImage->SetRenderOpacity(1.0f);
}

void UDinoGameWidget::TriggerJump()
{
    if (!bIsGameActive) 
    {
        StartGame();
        return;
    }

    // Only jump if on the ground (simple check)
    if (FMath::IsNearlyEqual(CurrentDinoY, GroundY, 5.0f))
    {
        VerticalVelocity = JumpForce;
    }
}

void UDinoGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsGameActive) return;

    HandlePhysics(InDeltaTime);
    HandleObstacle(InDeltaTime);

    // Update Score
    Score += InDeltaTime * 10.0f;
    if (ScoreText) ScoreText->SetText(FText::AsNumber(FMath::FloorToInt(Score)));

    if (CheckCollision())
    {
        bIsGameActive = false;
        if (GameOverText) GameOverText->SetVisibility(ESlateVisibility::Visible);
    }
}

void UDinoGameWidget::HandlePhysics(float DeltaTime)
{
    if (!DinoImage) return;

    UCanvasPanelSlot* DinoSlot = Cast<UCanvasPanelSlot>(DinoImage->Slot);
    if (DinoSlot)
    {
        // Apply Gravity
        VerticalVelocity += Gravity * DeltaTime;
        CurrentDinoY += VerticalVelocity * DeltaTime;

        // Floor Clamp
        if (CurrentDinoY >= GroundY)
        {
            CurrentDinoY = GroundY;
            VerticalVelocity = 0.0f;
        }

        DinoSlot->SetPosition(FVector2D(DinoSlot->GetPosition().X, CurrentDinoY));
    }
}

void UDinoGameWidget::HandleObstacle(float DeltaTime)
{
    if (!ObstacleImage) return;

    UCanvasPanelSlot* ObsSlot = Cast<UCanvasPanelSlot>(ObstacleImage->Slot);
    if (ObsSlot)
    {
        ObstacleX -= ObstacleSpeed * DeltaTime;

        // Reset Obstacle if it goes off screen left
        // Use local geometry to find screen width for respawn
        float ScreenWidth = GetTickSpaceGeometry().GetLocalSize().X;
        
        if (ObstacleX < -100.0f)
        {
            ObstacleX = ScreenWidth + 50.0f; // Reset just off the right edge
            ObstacleSpeed += 20.0f; // Make it harder over time
        }

        ObsSlot->SetPosition(FVector2D(ObstacleX, ObsSlot->GetPosition().Y));
    }
}

bool UDinoGameWidget::CheckCollision()
{
    if (!DinoImage || !ObstacleImage) return false;

    FGeometry DinoGeom = DinoImage->GetTickSpaceGeometry();
    FGeometry ObsGeom = ObstacleImage->GetTickSpaceGeometry();

    // Get absolute positions and sizes
    FVector2D DinoPos = DinoGeom.GetAbsolutePosition();
    FVector2D DinoSize = DinoGeom.GetAbsoluteSize();

    FVector2D ObsPos = ObsGeom.GetAbsolutePosition();
    FVector2D ObsSize = ObsGeom.GetAbsoluteSize();

    // Hitbox Shrink: Make the collision box 80% of the visual size
    // to account for transparent pixels in images
    float ShrinkFactor = 0.8f;
    
    FVector2D DinoHitboxSize = DinoSize * ShrinkFactor;
    FVector2D DinoOffset = (DinoSize - DinoHitboxSize) * 0.5f;
    FVector2D DinoHitboxPos = DinoPos + DinoOffset;

    FVector2D ObsHitboxSize = ObsSize * ShrinkFactor;
    FVector2D ObsOffset = (ObsSize - ObsHitboxSize) * 0.5f;
    FVector2D ObsHitboxPos = ObsPos + ObsOffset;

    bool bOverlapX = (DinoHitboxPos.X < ObsHitboxPos.X + ObsHitboxSize.X) && (DinoHitboxPos.X + DinoHitboxSize.X > ObsHitboxPos.X);
    bool bOverlapY = (DinoHitboxPos.Y < ObsHitboxPos.Y + ObsHitboxSize.Y) && (DinoHitboxPos.Y + DinoHitboxSize.Y > ObsHitboxPos.Y);

    return bOverlapX && bOverlapY;
}