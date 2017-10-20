// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FURahEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogURahEditor, Log, All);

#ifdef MODULE_LOG_WARNING
#undef  MODULE_LOG_WARNING
#endif
#ifdef MODULE_LOG_FUNC
#undef MODULE_LOG_FUNC
#endif

#define MODULE_LOG_WARNING(Format, ...) UE_LOG(LogURahEditor, Warning, Format, __VA_ARGS__)
#define MODULE_LOG_FUNC() UE_LOG(LogURahEditor, Warning, TEXT("%s"), __FUNCTIONW__)
