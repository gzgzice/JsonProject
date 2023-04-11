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
			//�츮�� ���� array�� �־�����
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

	//SpawnActor<>�� ���ø� ������ �ڷ����� ����
}

// Called every frame
void ACSVLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FUserInfos> ACSVLoader::Parse(FString path)
{
	//���������͸� ���� array
	TArray<FUserInfos> data;

	//��ο� �ִ� CSV ������ �о path �� �ִ� ������ �о����
	FString fileContent;
	//����� �� �о�Ӵٸ� true �� ��ȯ����
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//�ε尡 �� �Ǿ fileContent ��� ���� ����
		//UE_LOG(LogTemp, Warning, TEXT("%s"),fileContent)
		//��(����) ������
		TArray<FString> rows;
		//�������� �߶� rows ���� �߰��ϰڴ�. �Ľ��ϰڴ�
		fileContent.ParseIntoArrayLines(rows);

		//�����̸� ������
		TArray<FString> variableName;
		//�޸� �������� �߶� ��Ʈ������ ���� variableName���ٰ� string ���� �߰��ϰڴ�..
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//���� �����͸� ������ �������ϼ��ֱ⶧���� �ݺ��� ������
		for (int32 i = 1; i < rows.Num(); i++)	//0�� �����Ͱ� �ƴϴϱ� i�� 1����
		{
			TArray<FString> values;	//��¥ ����Ÿ
			rows[i].ParseIntoArray(values, TEXT(","));

			//�ϳ��� �����͸� ����ü ���� ������ ���������� �ٲ���
			FUserInfos info;
			info.name = values[0];
			info.age = FCString::Atoi(*values[1]);	//int32 ������ �ٲ���
			info.isWoman = values[2].ToBool();	//bool ���� string ������ ������
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

	//��ο� �ִ� CSV ������ �о path �� �ִ� ������ �о����
	FString fileContent;
	//����� �� �о�Ӵٸ� true �� ��ȯ����
	if (FFileHelper::LoadFileToString(fileContent, *path))
	{
		//�ε尡 �� �Ǿ fileContent ��� ���� ����
		//UE_LOG(LogTemp, Warning, TEXT("%s"),fileContent)
		//��(����) ������
		TArray<FString> rows;
		//�������� �߶� rows ���� �߰��ϰڴ�. �Ľ��ϰڴ�
		fileContent.ParseIntoArrayLines(rows);

		//�����̸� ������
		TArray<FString> variableName;
		//�޸� �������� �߶� ��Ʈ������ ���� variableName���ٰ� string ���� �߰��ϰڴ�..
		rows[0].ParseIntoArray(variableName, TEXT(","));

		//���� �����͸� ������ �������ϼ��ֱ⶧���� �ݺ��� ������
		for (int32 i = 1; i < rows.Num(); i++)	//0�� �����Ͱ� �ƴϴϱ� i�� 1����
		{
			TArray<FString> values;	//��¥ ����Ÿ
			rows[i].ParseIntoArray(values, TEXT(","));

			//�ϳ��� �����͸� ����ü ���� ������ ���������� �ٲ���
			T info;
			for (int32 j = 0; i < variableName.Num(); j++)
			{
				//�ش� ���� �̸����� �Ǿ��ִ� ������ ������ �����ͼ� ��Ʈ���� �� �ְԲ� pro �� �־��� 
				FProperty* pro = pScriptStruct->FindPropertyByName(*variableName[i]);

				//FString �̸� true �ƴϸ� false �� ��ȯ
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
