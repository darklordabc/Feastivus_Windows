// Fill out your copyright notice in the Description page of Project Settings.

#include "IngredientChecker.h"

void UIngredientChecker::CheckIngredient(
    const TSubclassOf<class AActor> Ingredient,
    const TArray<TSubclassOf<class AActor>> Ingredients,
    const TArray<FRecipe> Recipes,
    bool &bSuccess,
    int32 &DishIndex)
{
    // 1. Check IngredientList against all recipes
    //  a) Store all overlaps
    //  b) Return true if IngredientList is empty
    // 2. Check Ingredient against all overlapping recipes
    //  a) Return true if Ingredient can be added to any of the overlapping recipes
    //  b) Otherwise, return false
    // Dish index is only set when recipe is complete

    DishIndex = -1;
    TMap<int32, TArray<TSubclassOf<class AActor>>> Overlaps;

    for (int32 RecipeIndex = 0; RecipeIndex < Recipes.Num(); RecipeIndex++)
    {
        FRecipe RecipeToCheck = Recipes[RecipeIndex];
        if (RecipeToCheck.Ingredients.Num() <= Ingredients.Num())
            continue;

        auto RecipeIngredientsCopy = RecipeToCheck.Ingredients;
        RecipeIngredientsCopy.Sort();

        auto IngredientsCopy = Ingredients;
        IngredientsCopy.Sort();

        int32 IndexA = 0;
        while (IndexA != RecipeIngredientsCopy.Num())
        {
            auto RecipeIngredientToCheck = RecipeIngredientsCopy[IndexA];

            int32 IndexB = 0;
            while (IndexB != IngredientsCopy.Num())
            {
                auto IngredientToCheck = IngredientsCopy[IndexB];
                if (IngredientToCheck == RecipeIngredientToCheck)
                {
                    RecipeIngredientsCopy.RemoveAt(IndexA);
                    IngredientsCopy.RemoveAt(IndexB);
                    IndexA = -1;
                    break;
                }
                IndexB++;
            }

            IndexA++;
        }

        if (IngredientsCopy.Num() == 0 && RecipeIngredientsCopy.Num() > 0)
        {
            Overlaps.Add(RecipeIndex, RecipeIngredientsCopy);
        }
    }

    if (Overlaps.Num() > 0)
    {
        for (auto &Pair : Overlaps)
        {
            if (Pair.Value.Contains(Ingredient))
            {
                bSuccess = true;
                if (Pair.Value.Num() == 1)
                {
                    DishIndex = Pair.Key;
                }
                return;
            }
        }
    }

    bSuccess = false;
    return;
}