#include "DinoTestPlayerController.h"
#include "DinoGameWidget.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ADinoTestPlayerController::ADinoTestPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;

    // Find the Widget Blueprint asset
    static ConstructorHelpers::FClassFinder<UDinoGameWidget> WidgetClassFinder(TEXT("/Game/FiveDays/UI/DinoGame/WBP_DinoScreen"));
    if (WidgetClassFinder.Succeeded())
    {
        DinoWidgetClass = WidgetClassFinder.Class;
    }
}

void ADinoTestPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (DinoWidgetClass)
    {
        DinoWidgetInstance = CreateWidget<UDinoGameWidget>(this, DinoWidgetClass);
        if (DinoWidgetInstance)
        {
            DinoWidgetInstance->AddToViewport();
            DinoWidgetInstance->StartGame();
        }
    }
}

void ADinoTestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAction("Jump", IE_Pressed, this, &ADinoTestPlayerController::OnJumpPressed);
        // Also bind Left Mouse Button for convenience
        InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ADinoTestPlayerController::OnJumpPressed);
        InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &ADinoTestPlayerController::OnJumpPressed);
    }
}

void ADinoTestPlayerController::OnJumpPressed()
{
    if (DinoWidgetInstance)
    {
        DinoWidgetInstance->TriggerJump();
    }
}