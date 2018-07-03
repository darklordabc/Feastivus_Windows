// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Recipe.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FEASTIVUS_API FRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName RecipeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> IngredientList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> Dish;
};
