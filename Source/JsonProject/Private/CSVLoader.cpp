// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVLoader.h"

// Sets default values
ACSVLoader::ACSVLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FString dataPath = TEXT("DataTable'/Game/Blueprints/Item/ItemData.ItemData'");
	ConstructorHelpers::FObjectFinder<UDataTable>temptable(*dataPath);
	if (temptable.Succeeded())
	{
		UDataTable* data = temptable.Object;
		TArray<FName> rowNames = data->GetRowNames();
		for (int32 i = 0; i < rowNames.Num(); i++)
		{
			FUserInfos* userInfo = data->FindRow<FUserInfos>(rowNames[i], TEXT(""));
			//우리가 만든 array에 넣어주자
			userData.Add(*userInfo);
		}
	}
}

// Called when the game starts or when spawned
void ACSVLoader::BeginPlay()
{
	Super::BeginPlay();
	
// 	//FString path = FString::Printf(TEXT("%s/CSVData/UserData.csv"), *FPaths::GetProjectFilePath());
// 	FString path = FPaths::Combine(FPaths::ProjectDir(), TEXT("CSVData/UserData.csv"));
// 	//userData = Parse<FUserInfos>(path, FUserInfos::StaticStruct());
// 	userData = Parse(path);

	//SpawnActor<>는 템플릿 형태임 자료형을 넣음
}

// Called every frame
void ACSVLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FUserInfos> ACSVLoader::Parse(FString path)
{
	//최종데이터를 담을 array
	TArray<FUserInfos> data;

	//경로에 있는 CSV 파일을 읽어서 path 에 있는 파일을 읽어오자
	FString fileContent;
	//제대로 잘 읽어왓다면 true 를 반환해줌
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//로드가 잘 되어서 fileContent 라는 값이 들어옴
		//UE_LOG(LogTemp, Warning, TEXT("%s"),fileContent)
		//행(한출) 나누기
		TArray<FString> rows;
		//한줄한줄 잘라서 rows 에다 추가하겠다. 파싱하겠다
		fileContent.ParseIntoArrayLines(rows);

		//변수이름 나누기
		TArray<FString> variableName;
		//콤마 기준으로 잘라서 스트링으로 만들어서 variableName에다가 string 으로 추가하겠다..
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//실제 데이터를 나누기 여러개일수있기때문에 반복문 돌리기
		for (int32 i = 1; i < rows.Num(); i++)	//0은 데이터가 아니니까 i는 1부터
		{
			TArray<FString> values;	//진짜 데이타
			rows[i].ParseIntoArray(values, TEXT(","));

			//하나의 데이터를 구조체 넣자 각가의 변수값으로 바꿔줌
			FUserInfos info;
			info.name = values[0];
			info.age = FCString::Atoi(*values[1]);	//int32 형으로 바꿔줌
			info.isWoman = values[2].ToBool();	//bool 형을 string 값으로 던져줌
			info.height = FCString::Atof(*values[3]);
			info.mbti = values[4];

			data.Add(info);

			UE_LOG(LogTemp, Error, TEXT("----------------------"))
			for (int32 j = 0; j < values.Num(); j++)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *values[i])
			}
		}
	}
	return data;
}

template<typename T>
TArray<T> ACSVLoader::Parse(FString path, UScriptStruct* pScriptStruct)
{
	TArray<T> data;

	//경로에 있는 CSV 파일을 읽어서 path 에 있는 파일을 읽어오자
	FString fileContent;
	//제대로 잘 읽어왓다면 true 를 반환해줌
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//로드가 잘 되어서 fileContent 라는 값이 들어옴
		//UE_LOG(LogTemp, Warning, TEXT("%s"),fileContent)
		//행(한출) 나누기
		TArray<FString> rows;
		//한줄한줄 잘라서 rows 에다 추가하겠다. 파싱하겠다
		fileContent.ParseIntoArrayLines(rows);

		//변수이름 나누기
		TArray<FString> variableName;
		//콤마 기준으로 잘라서 스트링으로 만들어서 variableName에다가 string 으로 추가하겠다..
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//실제 데이터를 나누기 여러개일수있기때문에 반복문 돌리기
		for (int32 i = 1; i < rows.Num(); i++)	//0은 데이터가 아니니까 i는 1부터
		{
			TArray<FString> values;	//진짜 데이타
			rows[i].ParseIntoArray(values, TEXT(","));

			//하나의 데이터를 구조체 넣자 각가의 변수값으로 바꿔줌
			T info;
			for (int32 j = 0; i < variableName.Num(); j++)
			{
				//해당 변수 이름으로 되어있는 변수의 정보를 가져와서 컨트롤할 수 있게끔 pro 에 넣어줌 
				FProperty* pro = pScriptStruct->FindPropertyByName(*variableName[i]);

				//FString 이면 true 아니면 false 를 반환
				if (pro->IsA<FStrProperty>())
				{
					FString* valuePtr = pro->ContainerPtrToValuePtr<FString>(&info);
					*valuePtr = values[j];
				}
				else if (pro->IsA<FIntProperty>())
				{
					int32* valuePtr = pro->ContainerPtrToValuePtr<int32>(&info);
					*valuePtr = FCString::Atoi(*values[j]);
				}
				else if (pro->IsA<FBoolProperty>())
				{
					bool* valuePtr = pro->ContainerPtrToValuePtr<bool>(&info);
					*valuePtr = values[j].ToBool();
				}
				else if (pro->IsA<FFloatProperty>())
				{
					float* valuePtr = pro->ContainerPtrToValuePtr<float>(&info);
					*valuePtr = FCString::Atof(*values[j]);
				}
			}

			data.Add(info);

// 			UE_LOG(LogTemp, Error, TEXT("----------------------"))
// 				for (int32 j = 0; j < values.Num(); j++)
// 				{
// 					UE_LOG(LogTemp, Warning, TEXT("%s"), *values[i])
// 				}
		}
	}

	return data;
}
