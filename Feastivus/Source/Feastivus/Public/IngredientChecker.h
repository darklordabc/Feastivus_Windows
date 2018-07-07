// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Recipe.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IngredientChecker.generated.h"

/**
 * 
 */
UCLASS()
class FEASTIVUS_API UIngredientChecker : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void CheckIngredient(
		const TSubclassOf<class AActor> Ingredient,
		const TArray<TSubclassOf<class AActor>> Ingredients,
		const UDataTable *Recipes,
		bool &Success,
		FRecipe &Recipe);
};
