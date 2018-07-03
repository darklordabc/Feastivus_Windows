// Fill out your copyright notice in the Description page of Project Settings.

#include "IngredientChecker.h"

void UIngredientChecker::CheckIngredient(const FName Ingredient, const TArray<FName> IngredientList, const UDataTable *Recipes, bool &Success, FRecipe &Recipe)
{
    // 1. Check IngredientList against all recipes
    //  a) Store all overlaps
    //  b) Return true if IngredientList is empty
    // 2. Check Ingredient against all overlapping recipes
    //  a) Return true if Ingredient can be added to any of the overlapping recipes
    //  b) Otherwise, return false

    UE_LOG(LogTemp, Warning, TEXT("Checking ingredient: %s"), *Ingredient.ToString());
    UE_LOG(LogTemp, Warning, TEXT("Current count is %d"), IngredientList.Num());
    for (auto &IngredientToPrint : IngredientList)
    {
        UE_LOG(LogTemp, Warning, TEXT("One of the ingredients: %s"), *IngredientToPrint.ToString());
    }

    FString ContextString;
    TMap<FName, TArray<FName>> Overlaps;

    for (auto &RowName : Recipes->GetRowNames())
    {
        FRecipe *RecipeToCheck = Recipes->FindRow<FRecipe>(RowName, ContextString);
        if (RecipeToCheck->IngredientList.Num() <= IngredientList.Num())
            continue;

        UE_LOG(LogTemp, Warning, TEXT("Checking recipe: %s"), *RowName.ToString());

        auto RecipeIngredientListCopy = RecipeToCheck->IngredientList;
        RecipeIngredientListCopy.Sort();

        auto IngredientListCopy = IngredientList;
        IngredientListCopy.Sort();

        for (int32 IndexA = 0; IndexA != RecipeIngredientListCopy.Num(); ++IndexA)
        {
            auto RecipeIngredientToCheck = RecipeIngredientListCopy[IndexA];
            for (int32 IndexB = 0; IndexB != IngredientListCopy.Num(); ++IndexB)
            {
                auto IngredientToCheck = IngredientListCopy[IndexB];
                if (IngredientToCheck == RecipeIngredientToCheck)
                {
                    RecipeIngredientListCopy.RemoveAt(IndexA);
                    IngredientListCopy.RemoveAt(IndexB);
                    IndexA = 0;
                    break;
                }
            }
        }

        if (IngredientListCopy.Num() == 0 && RecipeIngredientListCopy.Num() > 0)
        {
            Overlaps.Add(RowName, RecipeIngredientListCopy);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Overlap count is %d"), Overlaps.Num());
    for (auto &Pair : Overlaps)
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap recipe: %s"), *Pair.Key.ToString());
    }

    if (Overlaps.Num() > 0)
    {
        for (auto &Pair : Overlaps)
        {
            if (Pair.Value.Contains(Ingredient))
            {
                Success = true;
                if (Pair.Value.Num() == 1)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Completed recipe: %s"), *Pair.Key.ToString());
                    Recipe = *Recipes->FindRow<FRecipe>(Pair.Key, ContextString);
                }
                UE_LOG(LogTemp, Warning, TEXT("Checking: success"));
                return;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Checking: failure"));
        Success = false;
        return;
    }
}