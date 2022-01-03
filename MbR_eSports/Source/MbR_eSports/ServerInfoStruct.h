#pragma once

#include "ServerInfoStruct.generated.h"

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