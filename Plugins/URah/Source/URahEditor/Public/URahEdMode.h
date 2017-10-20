#pragma once

#include "Editor.h"
#include "EditorModeRegistry.h"
#include "EdMode.h"
#include "Toolkits/BaseToolkit.h"
#include "URah.h"
#include "UObject/ObjectMacros.h"
#include "URahClasses.h"


//#include "URahEdMode.generated.h"

//////////////////////////////////////////////////////////////////////////
class FURahEdModeToolkit;



//////////////////////////////////////////////////////////////////////////
class AActor;


class FURahEdModeToolkit;

class FURahEditorMode : public FEdMode
{
public:
	static const FName ID;


	FURahEditorMode();
	~FURahEditorMode();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;


	virtual void Initialize() override;
	virtual bool MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool MouseLeave(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool ReceivedFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;
	virtual bool LostFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;
	virtual bool CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool InputAxis(FEditorViewportClient* InViewportClient, FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime) override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual bool ShowModeWidgets() const override;
	virtual bool AllowWidgetMove() override;
	virtual bool ShouldDrawBrushWireframe(AActor* InActor) const override;
	virtual FVector GetWidgetLocation() const override;
	virtual bool ShouldDrawWidget() const override;
	virtual bool HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click) override;
	virtual bool Select(AActor* InActor, bool bInSelected) override;
	virtual bool UsesToolkits() const override;
	virtual bool IsSelectionAllowed(AActor* InActor, bool bInSelection) const override;


	FURahEdModeToolkit* GetToolkit() const { return (FURahEdModeToolkit*) this->Toolkit.Get(); }
	

	virtual bool GetCursor(EMouseCursor::Type& OutCursor) const override;
	virtual bool GetCustomDrawingCoordinateSystem(FMatrix& InMatrix, void* InData) override;
	virtual bool GetCustomInputCoordinateSystem(FMatrix& InMatrix, void* InData) override;
	virtual EAxisList::Type GetWidgetAxisToDraw(FWidget::EWidgetMode InWidgetMode) const override;
	virtual bool UsesTransformWidget() const override;
	virtual bool UsesTransformWidget(FWidget::EWidgetMode CheckMode) const override;
	virtual bool UsesPropertyWidgets() const override;

	ARahGraph* mSelectedGraph = nullptr;	//the current chosen graph
};


//////////////////////////////////////////////////////////////////////////
class FURahEdModeToolkit : public FModeToolkit
{
public:
	FURahEdModeToolkit()
	{

	}
	~FURahEdModeToolkit();

	TSharedPtr<SWidget>	ToolkitWidget;

	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	//return the editor mode as is is FURahEdMode
	virtual FEdMode* GetEditorMode() const override;


	virtual TSharedPtr<SWidget> GetInlineContent() const override;

	class URahEdModeToolkitProperties* PropertiesObject;

	float						BrushRadius = 0;


};


