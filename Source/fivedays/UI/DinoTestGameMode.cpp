#include "DinoTestGameMode.h"
#include "DinoTestPlayerController.h"

ADinoTestGameMode::ADinoTestGameMode()
{
    PlayerControllerClass = ADinoTestPlayerController::StaticClass();
}