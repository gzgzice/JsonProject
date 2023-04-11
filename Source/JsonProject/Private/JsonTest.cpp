// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonTest.h"

// Sets default values
AJsonTest::AJsonTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJsonTest::BeginPlay()
{
	Super::BeginPlay();

	//SaveData();
	LoadData();
}

// Called every frame
void AJsonTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJsonTest::SaveData()
{
	//FString userName = TEXT("김민지");
	//int32 userAge = 27;
	TArray<FString> interests = { TEXT("게임"), TEXT("댄스"), TEXT("독서") };
	//bool isWoman = true;

	FUserInfo info;
	info.name = TEXT("김민지");
	info.age = 27;
	for (int32 i = 0; i < 3; i++)
	{
		info.interests.Add(interests[i]);
	}
	info.isWoman = true;

	//변수에 담겨있는 데이터를 Json Object 로 변경
	//여러개의 참조 가능, 널 값을 가질 수 있음
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject); //포인터임 새로운 제이슨 객체생성 
	jsonObject->SetStringField("userName", info.name); // 키, value
	jsonObject->SetNumberField("userAge", info.age);
	jsonObject->SetBoolField("isWoman", info.isWoman);

	//FString -> FJsonValue 로 변환해서 jsonArray 셋팅
	TArray<TSharedPtr<FJsonValue>> arrayInterests;
	for (int32 i = 0; i < interests.Num(); i++)
	{
		arrayInterests.Add(MakeShareable(new FJsonValueString(interests[i]))); //배열이 어떤 자료형인지에 따라 FJsonValueString이 바뀐다.ㄴ
	}
	jsonObject->SetArrayField("interests", arrayInterests);

	//key value 값으로 만든다음레 문자열(string)로 변경해줘야 함
	FString jsonString;
	//강한결속 한가지만 담을 수 있음 널값을 가질 수 없음
	TSharedRef<TJsonWriter<>> jsonWrite = TJsonWriterFactory<>::Create(&jsonString);	//아무것도 안넣으면 TCharacter 형으로 들어감
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), jsonWrite);	//ref 로 변경해줘야함 jsonObject를 문자열로 변경해줘

	UE_LOG(LogTemp, Warning, TEXT("%s"), *jsonString)
	//제이슨을 사용하기 위해서는 모듈을 추가해야함

	//Json 형태로 변경된 문자열(jsonString)을 파일로 저장
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
	FFileHelper::SaveStringToFile(jsonString, *filePath);
}

void AJsonTest::LoadData()
{
	//불러올 데이터을 담을 변수 생성
	FString jsonString;
	//파일을 읽어오자
	FString filePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("jsonTest.txt"));
	FFileHelper::LoadFileToString(jsonString, *filePath);	//경로를 읽어와서 jsonString에 넣어라
	UE_LOG(LogTemp, Warning, TEXT("load : %s"), *jsonString)

	//string -> jsonObject로 변환 
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))	//string 값이 Json 형태인 경우에만
	{
		FString userName = jsonObject->GetStringField("userName");
		int32 userAge = jsonObject->GetNumberField("userAge");
		
		TArray<FString> interests;
		TArray<TSharedPtr<FJsonValue>>arrayInterests = jsonObject->GetArrayField("interests");
		for (int32 i = 0; i < arrayInterests.Num(); i++)
		{
			interests.Add(arrayInterests[i]->AsString());
		}

		bool isWoman = jsonObject->GetBoolField("isWoman");

		UE_LOG(LogTemp, Warning, TEXT("name : %s"), *userName);
		UE_LOG(LogTemp, Warning, TEXT("age : %d"), userAge);
		UE_LOG(LogTemp, Warning, TEXT("isWoman : %d"), isWoman);
		for (int32 i = 0; i < interests.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("interests : %s"), *interests[i]);
		}
	}
;}

