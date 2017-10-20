// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "URahEditor.h"

#include "ComponentVisualizer.h"
#include "UnrealEd.h"

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

#include "PropertyEditorDelegates.h"
#include "PropertyEditorModule.h"
#include "URahEdMode.h"
#include "MultiSelectionEdMode.h"

#define LOCTEXT_NAMESPACE "FURahEditorModule"

DEFINE_LOG_CATEGORY(LogURahEditor);




void FURahEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	check(GUnrealEd);
	
	//FComponentVisualizersModule& CVM = FModuleManager::LoadModuleChecked<FComponentVisualizersModule>("ComponentVisualizers");
	
	//CVM.RegisterComponentVisualizer(UPFSphericalGraphComponent::StaticClass()->GetFName(), MakeShareable(new FPFSphericalVisualizer));

	//Registering EdModes
	{
		FEditorModeRegistry::Get().RegisterMode<FURahEditorMode>(FURahEditorMode::ID, LOCTEXT("URahEditor", "URahEditor"), FSlateIcon(), true);
		FEditorModeRegistry::Get().RegisterMode<FMultiSelectionEdMode>("FMultiSelectionEdMode", LOCTEXT("FMultiSelectionEdMode", "FMultiSelectionEdMode"), FSlateIcon(), true);
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	//Custom properties
// 	PropertyModule.RegisterCustomPropertyTypeLayout(FName("FSubObject"), FOnGetPropertyTypeCustomizationInstance::CreateLambda([]() {
// 		return TSharedRef<IPropertyTypeCustomization>(new FSubObjectCustomization);
// 	}));

}

void FURahEditorModule::ShutdownModule()
{
	//unregistering EdModes
	FEditorModeRegistry::Get().UnregisterMode(FURahEditorMode::ID);
	FEditorModeRegistry::Get().UnregisterMode("FMultiSelectionEdMode");

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	//PropertyModule.UnregisterCustomPropertyTypeLayout(FName("FSubObject"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FURahEditorModule, FURahEditor)