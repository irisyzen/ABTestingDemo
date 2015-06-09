#pragma once

#include <string>

enum ActionType
{
	ActionType_None,
	ActionType_Start,
	ActionType_Stop,
	ActionType_SetVars,
	ActionType_GetVars,
	ActionType_SetUserAttributes,
	ActionType_Track
};

struct leanplumAPIKeys
{
	std::string appID;
	std::string productionKey;
	std::string developmentKey;
	std::string dataExportKey;
	std::string contentReadOnlyKey;

	leanplumAPIKeys()
	{
	}

	leanplumAPIKeys(const std::string& appid, const std::string& prodKey, const std::string& devKey, const std::string& dataKey, const std::string contentKey)
		: appID(appid), productionKey(prodKey), developmentKey(devKey), dataExportKey(dataKey), contentReadOnlyKey(contentKey)
	{
	}
};

enum eLeanplumVarType
{
	eLeanplumVarType_Int,
	eLeanplumVarType_String,
	eLeanplumVarType_Bool
};

struct leanplumVar
{
	std::string name;
	eLeanplumVarType type;

	std::string sValue;
	bool bValue;
	int nValue;

	leanplumVar()
	{
		bValue = false;
		nValue = 0;
	}
};