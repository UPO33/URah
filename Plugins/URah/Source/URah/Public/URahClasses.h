#pragma once

#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/BillboardComponent.h"
#include "ProceduralMeshComponent.h"

#include "Engine/Note.h"
#include "Engine/PointLight.h"

#include "URahClasses.generated.h"




struct FRahGridTest;

//////////////////////////////////////////////////////////////////////////
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class URAH_API URahTestGridComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

	URahTestGridComponent();
	~URahTestGridComponent();

	
	UPROPERTY(EditAnywhere)
	int NumCellInX = 64;
	UPROPERTY(EditAnywhere)
	int NumCellInY = 64;
	UPROPERTY(EditAnywhere)
	int DstX = 0;
	UPROPERTY(EditAnywhere)
	int DstY = 0;
	UPROPERTY(EditAnywhere)
	int SrcX = 0;
	UPROPERTY(EditAnywhere)
	int SrcY = 0;

	
	UPROPERTY(EditAnywhere)
	float CellSize = 100;

	FRahGridTest* mGrid;

	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	void GenerateMesh();
public:
	virtual void InitializeComponent() override;
	virtual void OnRegister() override;

	virtual void OnComponentCreated() override;

};

UCLASS()
class URAH_API ARahTestGrid : public AActor
{
	GENERATED_BODY()
public:
	ARahTestGrid();
	~ARahTestGrid();

	UPROPERTY(VisibleAnywhere)
	URahTestGridComponent* Grid;
};

struct FRahGraph;

//////////////////////////////////////////////////////////////////////////
struct FRahNode
{
	struct FLink
	{
		FRahNode*	mNode;	//pointer to the node that we can traverse to
		int			mCost;  //cost of moving to this node
	};


	TArray<FLink>	mLinks;
	FVector			mPosition;

	unsigned mIsOpen : 1;
	unsigned mIsClosed : 1;
	unsigned mCameFrom : 30;
	unsigned mCost;
	unsigned mTotalCost;
};

//////////////////////////////////////////////////////////////////////////
struct FRahGraph
{
	TArray<FRahNode>	mNodes;

	FRahGraph()
	{
		mNodes.Reset(512);
	}
};
//////////////////////////////////////////////////////////////////////////
struct FRahPath
{
	TArray<FRahNode*>	Path;

	bool IsEmpty() const
	{
		return Path.Num() == 0;
	}
};

//////////////////////////////////////////////////////////////////////////
UCLASS(Blueprintable, BlueprintType)
class URAH_API URahGraphComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	URahGraphComponent();
	~URahGraphComponent();

	FRahGraph* mGrapth;
};

//////////////////////////////////////////////////////////////////////////
UCLASS()
class URAH_API ARahGraph : public AActor
{
	GENERATED_BODY()
public:

	ARahGraph();
	~ARahGraph();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URahGraphComponent* GraphComponent;

	FRahGraph* GetGraph() const;
};


