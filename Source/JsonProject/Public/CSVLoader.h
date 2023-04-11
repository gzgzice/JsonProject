// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "CSVLoader.generated.h"

USTRUCT(BlueprintType)
struct FUserInfos /*: public FTableRowBase*/
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString name;

	UPROPERTY()
		int32 age;

	UPROPERTY()
		bool isWoman;

	UPROPERTY()
		float height;

	UPROPERTY()
		FString mbti;
};

// USTRUCT(BlueprintType)
// struct FProductInfo
// {
// 	GENERATED_BODY()
// 
// public:
// 	UPROPERTY()
// 		FString name;
// 
// 	UPROPERTY()
// 		int32 price;
// 
// 	UPROPERTY()
// 		bool sale;
// 
// 	UPROPERTY()
// 		float wight;
// };

UCLASS()
class JSONPROJECT_API ACSVLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSVLoader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FUserInfos> Parse(FString path);

	template<typename T>
	TArray<T> Parse(FString path, UScriptStruct* pScriptStruct);

	UPROPERTY(EditAnywhere)
	TArray<FUserInfos> userData;	
	
// 	UPROPERTY(EditAnywhere)
// 	TArray<FUserInfos> productData;
};


