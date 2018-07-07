// Fill out your copyright notice in the Description page of Project Settings.

#include "IngredientChecker.h"

void UIngredientChecker::CheckIngredient(
    const TSubclassOf<class AActor> Ingredient,
    const TArray<TSubclassOf<class AActor>> Ingredients,
    const UDataTable *Recipes,
    bool &Success,
    FRecipe &Recipe)
{
    // 1. Check IngredientList against all recipes
    //  a) Store all overlaps
    //  b) Return true if IngredientList is empty
    // 2. Check Ingredient against all overlapping recipes
    //  a) Return true if Ingredient can be added to any of the overlapping recipes
    //  b) Otherwise, return false

    //UE_LOG(LogTemp, Warning, TEXT("Checking ingredient: %s"), *Ingredient.ToString());
    //UE_LOG(LogTemp, Warning, TEXT("Current count is %d"), IngredientList.Num());
    // for (auto &IngredientToPrint : IngredientList)
    // {
        //UE_LOG(LogTemp, Warning, TEXT("One of the ingredients: %s"), *IngredientToPrint.ToString());
    // }

    FString ContextString;
    TMap<FName, TArray<TSubclassOf<class AActor>>> Overlaps;

    for (auto &RowName : Recipes->GetRowNames())
    {
        FRecipe *RecipeToCheck = Recipes->FindRow<FRecipe>(RowName, ContextString);
        if (RecipeToCheck->Ingredients.Num() <= Ingredients.Num())
            continue;

        //UE_LOG(LogTemp, Warning, TEXT("Checking recipe: %s"), *RowName.ToString());

        auto RecipeIngredientsCopy = RecipeToCheck->Ingredients;
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
            Overlaps.Add(RowName, RecipeIngredientsCopy);
        }
    }

    //UE_LOG(LogTemp, Warning, TEXT("Overlap count is %d"), Overlaps.Num());

    if (Overlaps.Num() > 0)
    {
        for (auto &Pair : Overlaps)
        {
            if (Pair.Value.Contains(Ingredient))
            {
                Success = true;
                if (Pair.Value.Num() == 1)
                {
                    //UE_LOG(LogTemp, Warning, TEXT("Completed recipe: %s"), *Pair.Key.ToString());
                    Recipe = *Recipes->FindRow<FRecipe>(Pair.Key, ContextString);
                }
                //UE_LOG(LogTemp, Warning, TEXT("Checking: success"));
                return;
            }
        }
    }

    //UE_LOG(LogTemp, Warning, TEXT("Checking: failure"));
    Success = false;
    return;
}