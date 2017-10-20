#include "URahEdMode.h"

#include "SlateBasics.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/SPanel.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#include "EditorViewportClient.h"

#include "PropertyEditorModule.h"
#include "LevelEditor.h"

#include "IDetailsView.h"

#include "URahToolkit.h"

FURahEdModeToolkit::~FURahEdModeToolkit()
{
}

void FURahEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	UWorld* pWorld = this->GetEditorMode()->GetWorld();
	PropertiesObject = NewObject<URahEdModeToolkitProperties>(pWorld, "Properties");

	FPropertyEditorModule& propertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs detailViewArgs;
	detailViewArgs.bAllowSearch = false;
	detailViewArgs.bHideSelectionTip = true;
	detailViewArgs.bShowActorLabel = false;

	TSharedRef<IDetailsView> detailView = propertyEditorModule.CreateDetailView(detailViewArgs);
	detailView->SetObject(this->PropertiesObject);
	this->ToolkitWidget = detailView;

	FModeToolkit::Init(InitToolkitHost);
}


FName FURahEdModeToolkit::GetToolkitFName() const
{
	return FName("URah");
}

FText FURahEdModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("URah", "URah");
}

FEdMode* FURahEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FURahEditorMode::ID);
}

TSharedPtr<SWidget> FURahEdModeToolkit::GetInlineContent() const
{
	return ToolkitWidget;
}