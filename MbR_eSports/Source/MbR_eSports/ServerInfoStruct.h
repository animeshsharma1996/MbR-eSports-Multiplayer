#pragma once

#include "ServerInfoStruct.generated.h"

/*
Struct File created to define Server Info and Passer Server Info as parameters to contain information of the server,
name, current/max players, lan check, ping and array index
*/

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString serverName;
	UPROPERTY(BlueprintReadOnly)
		int32 currentPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 maxPlayers;	
	UPROPERTY(BlueprintReadOnly)
		bool isLan;	
	UPROPERTY(BlueprintReadOnly)
		int32 ping;	
	UPROPERTY(BlueprintReadOnly)
		int32 serverArrayIndex;
}; 

USTRUCT(BlueprintType)
struct FPassedServerInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		FString serverName;
	UPROPERTY(BlueprintReadOnly)
		int32 maxPlayers;
	UPROPERTY(BlueprintReadOnly)
		bool isLan;
};