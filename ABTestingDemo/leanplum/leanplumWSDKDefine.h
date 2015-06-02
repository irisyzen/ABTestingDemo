#pragma once

#include <string>

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