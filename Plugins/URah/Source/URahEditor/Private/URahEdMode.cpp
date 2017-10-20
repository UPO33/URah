#include "URahEdMode.h"
#include "URah.h"

#include "SceneView.h"
#include "SceneManagement.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "UnrealEd.h"
#include "Toolkits/ToolkitManager.h"


#include "HitProxies.h"

#include "URahToolkit.h"

#include "Private/Editor/ActorPositioning.h"

#define LOCTEXT_NAMESPACE "URahEd"

struct HGraphNode : public HHitProxy
{
	DECLARE_HIT_PROXY()

	HGraphNode(FRahNode* node = nullptr, FRahGraph* graph = nullptr) : mNode(node), mGraph(graph)
	{}

	FRahGraph* mGraph;
	FRahNode*  mNode;
};

struct HNodeLink : public HHitProxy
{
	DECLARE_HIT_PROXY()

	HNodeLink(FRahNode* node = nullptr, FRahNode::FLink* link = nullptr) : mNode(node), mLink(link)
	{
	}

	FRahNode* mNode;
	FRahNode::FLink* mLink;
};

IMPLEMENT_HIT_PROXY(HGraphNode, HHitProxy)
IMPLEMENT_HIT_PROXY(HNodeLink, HHitProxy)


const FName FURahEditorMode::ID = FName("URahEditorMode");

FURahEditorMode::FURahEditorMode()
{
}

FURahEditorMode::~FURahEditorMode()
{
}

void FURahEditorMode::Enter()
{
	FEdMode::Enter();
	
	// Clear any selection
	GEditor->SelectNone(true, true);

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FURahEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FURahEditorMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit = nullptr;
	}
	FEdMode::Exit();
}

void FURahEditorMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	FEdMode::Render(View, Viewport, PDI);

	if (mSelectedGraph)
	{
		if (FRahGraph* graph = mSelectedGraph->GetGraph())
		{
			//draw links
			for (FRahNode& node : graph->mNodes)
			{
				for (FRahNode::FLink& link : node.mLinks)
				{
					FLinearColor linkColor = FLinearColor::Red;
					PDI->SetHitProxy(new HNodeLink(&node, &link));
					PDI->DrawLine(node.mPosition, link.mNode->mPosition, linkColor, SDPG_Foreground, 1, 0, true);
				}
			}
			//draw nodes
			for (FRahNode& node : graph->mNodes)
			{
				FLinearColor nodeColor = FLinearColor::Green;
				PDI->SetHitProxy(new HGraphNode(&node, graph));
				PDI->DrawPoint(node.mPosition, nodeColor, 3, SDPG_Foreground);
			}
		}
	}
}

void FURahEditorMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);
	FCanvasTextItem item(FVector2D(8, 8), FText::FromString("ttexxxxxt"), GEngine->GetLargeFont(), FLinearColor::White);
	Canvas->DrawItem(item);
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);
}

void FURahEditorMode::Initialize()
{
	FEdMode::Initialize();
}

bool FURahEditorMode::MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	return FEdMode::MouseEnter(ViewportClient, Viewport, x, y);
}

bool FURahEditorMode::MouseLeave(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	return FEdMode::MouseLeave(ViewportClient, Viewport);
}


bool FURahEditorMode::MouseMove(FEditorViewportClient* InViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	// Compute a world space ray from the screen space mouse coordinates
	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		InViewportClient->Viewport,
		InViewportClient->GetScene(),
		InViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(InViewportClient->IsRealtime()));

	FSceneView* View = InViewportClient->CalcSceneView(&ViewFamily);
	FViewportCursorLocation MouseViewportRay = FViewportCursorLocation(View, InViewportClient, x, y);

	 FActorPositionTraceResult traceResult = FActorPositioning::TraceWorldForPosition(MouseViewportRay, *View, nullptr);
	 if (traceResult.State == FActorPositionTraceResult::HitSuccess)
	 {
		 DrawDebugSphere(GetWorld(), traceResult.Location, 8, 8, FColor::Red, false, 3);
	 }

	

	{
		if (HHitProxy* hitUnderCursor = Viewport->GetHitProxy(x, y))
		{
			if (auto hitAsNode = HitProxyCast<HGraphNode>(hitUnderCursor))
			{

			}
			else if (auto hitAsLink = HitProxyCast<HNodeLink>(hitUnderCursor))
			{
				
			}
		}
	}
	return FEdMode::MouseMove(InViewportClient, Viewport, x, y);
}

bool FURahEditorMode::ReceivedFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	return FEdMode::ReceivedFocus(ViewportClient, Viewport);
}

bool FURahEditorMode::LostFocus(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
	return FEdMode::LostFocus(ViewportClient, Viewport);
}




bool FURahEditorMode::CapturedMouseMove(FEditorViewportClient* InViewportClient, FViewport* InViewport, int32 InMouseX, int32 InMouseY)
{
	return FEdMode::CapturedMouseMove(InViewportClient, InViewport, InMouseX, InMouseY);
}

bool FURahEditorMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FURahEditorMode::InputAxis(FEditorViewportClient* InViewportClient, FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime)
{
	return FEdMode::InputAxis(InViewportClient, Viewport, ControllerId, Key, Delta, DeltaTime);
}

bool FURahEditorMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	return false;
}

void FURahEditorMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	FEdMode::Tick(ViewportClient, DeltaTime);
}

bool FURahEditorMode::ShowModeWidgets() const
{
	return true;
}

bool FURahEditorMode::AllowWidgetMove()
{
	return false;
}

bool FURahEditorMode::ShouldDrawBrushWireframe(AActor* InActor) const
{
	return FEdMode::ShouldDrawBrushWireframe(InActor);
}

FVector FURahEditorMode::GetWidgetLocation() const
{
	return FEdMode::GetWidgetLocation();
}

bool FURahEditorMode::ShouldDrawWidget() const
{
	return false;
}
bool FURahEditorMode::HandleClick(FEditorViewportClient* InViewportClient, HHitProxy* HitProxy, const FViewportClick& Click)
{
	return FEdMode::HandleClick(InViewportClient, HitProxy, Click);
}




bool FURahEditorMode::Select(AActor* InActor, bool bInSelected)
{
	return FEdMode::Select(InActor, bInSelected);
}

bool FURahEditorMode::UsesToolkits() const
{
	//true means this edmode has toolkit, we create toolkit in Enter()
	return true;
}

bool FURahEditorMode::IsSelectionAllowed(AActor* InActor, bool bInSelection) const
{
	return true;
}

bool FURahEditorMode::GetCursor(EMouseCursor::Type& OutCursor) const
{
	return FEdMode::GetCursor(OutCursor);
}

bool FURahEditorMode::GetCustomDrawingCoordinateSystem(FMatrix& InMatrix, void* InData)
{
	return FEdMode::GetCustomDrawingCoordinateSystem(InMatrix, InData);
}

bool FURahEditorMode::GetCustomInputCoordinateSystem(FMatrix& InMatrix, void* InData)
{
	return FEdMode::GetCustomInputCoordinateSystem(InMatrix, InData);
}

EAxisList::Type FURahEditorMode::GetWidgetAxisToDraw(FWidget::EWidgetMode InWidgetMode) const
{
	return FEdMode::GetWidgetAxisToDraw(InWidgetMode);
}

bool FURahEditorMode::UsesTransformWidget() const
{
	return FEdMode::UsesTransformWidget();
}

bool FURahEditorMode::UsesTransformWidget(FWidget::EWidgetMode CheckMode) const
{
	return FEdMode::UsesTransformWidget(CheckMode);
}

bool FURahEditorMode::UsesPropertyWidgets() const
{
	return FEdMode::UsesPropertyWidgets();
}

