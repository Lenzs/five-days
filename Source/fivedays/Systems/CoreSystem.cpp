// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CoreSystem.h"

ACoreSystem::ACoreSystem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SanityComponent = CreateDefaultSubobject<USanityComponent>(TEXT("SanityComponent"));
	SanityEffectManager = CreateDefaultSubobject<USanityEffectManager>(TEXT("SanityEffectManager"));
}

void ACoreSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoreSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

