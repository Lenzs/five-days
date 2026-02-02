// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/SanityComponent.h"
#include "Systems/SanityEffectManager.h"
#include "GameFramework/Actor.h"
#include "CoreSystem.generated.h"

UCLASS()
class FIVEDAYS_API ACoreSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoreSystem();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USanityComponent* SanityComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USanityEffectManager* SanityEffectManager;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
