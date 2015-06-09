#pragma once

#ifdef LEANPLUMWSDK_EXPORTS
#define LEANPLUMWSDK_API __declspec(dllexport)
#elif LEANPLUMWSDK_IMPORTS
#define LEANPLUMWSDK_API __declspec(dllimport)
#else
#define LEANPLUMWSDK_API
#endif

#include "leanplumWSDKDefine.h"
#include <vector>

typedef void (*fnStatusCallBack)(ActionType type, bool success, std::string errorMsg, void* extraInfo, void* context);

class ILeanplumWSDK
{
public:
	virtual void Initialize(const leanplumAPIKeys& apiKeys, unsigned int userID, unsigned int deviceID) = 0;
	virtual void EnableAsyncMode(bool enable, fnStatusCallBack statusCBFn, void* context) = 0; 

	virtual void SetAPIKeys(const leanplumAPIKeys& apiKeys) = 0;
	virtual void SetUserID(unsigned int userID) = 0;
	virtual void SetDeviceID(unsigned int deviceID) = 0;

	virtual bool Start() = 0;
	virtual bool Stop() = 0;

	virtual bool SetVars(const std::vector<leanplumVar>& varList) = 0;
	virtual bool GetVars(std::vector<leanplumVar>& varList) = 0;

	virtual bool SetUserAttributes(const std::vector<leanplumVar>& varList) = 0;

	virtual bool Track(const std::string eventName, int value) = 0;

	virtual void GetLastErrorMsg(std::string& errMsg) = 0;
};

typedef ILeanplumWSDK* (*pfnGetLeanplumWSDK) (void);

extern "C"
{
    LEANPLUMWSDK_API ILeanplumWSDK* GetLeanplumWSDK();
}