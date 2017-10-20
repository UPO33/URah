#include "URahClasses.h"
#include "URah.h"
#include "Components/BillboardComponent.h"


#include "Int2.h"
#include "Algo/Reverse.h"

struct FRahCell
{
	bool mIsBlock;

	bool mIsInOpenList;
	bool mIsInClosedList;
	int  mGScore;
	int  mFScore;
	FRahCell* mCameFrom;
};

struct FRahGridTest
{
	int	mNumCellInX = 0;
	int mNumCellInY = 0;

	TArray<FRahCell> mCells;

	TArray<Int2> mOpenList;
	TArray<FRahCell*> mResultPathPtr;

	FRahGridTest()
	{
		Reset(64, 64);
	}
	void Reset(int x, int y)
	{
		mNumCellInX = x;
		mNumCellInY = y;
		
		mCells.Reset();
		mCells.AddZeroed(mNumCellInX * mNumCellInY);

		for (FRahCell& cell : mCells)
		{
			cell.mIsBlock = false;
		}

	}
	FRahCell* GetCell(Int2 index)
	{
		return GetCell(index.x, index.y);
	}
	FRahCell* GetCell(int x, int y)
	{
		return &(mCells[y * mNumCellInX + x]);
	}
	void FillRnd()
	{
		Reset(mNumCellInX, mNumCellInY);

		unsigned nGroup = mNumCellInX / 2;

		for (unsigned i = 0; i < nGroup; i++)
		{
			unsigned sx = rand() % mNumCellInX;
			unsigned sy = rand() % mNumCellInY;

			unsigned count = 1 + (rand() % 6);
			bool bVertical = rand() % 2 == 0;

			for (unsigned iCount = 0; iCount < count; iCount++)
			{
				if (bVertical)
				{
					mCells[((sy + iCount) % mNumCellInY * mNumCellInX) + sx].mIsBlock = true;
				}
				else
				{
					mCells[sy * mNumCellInX + ((sx + iCount) % mNumCellInX)].mIsBlock = true;

				}
			}
		}
	}
	int HeuristicCostEstimate(Int2 from, Int2 to)
	{
		//returns square length
		Int2 d = from - to;
		return d.x * d.x + d.y * d.y;
	}
	bool OpenListIsEmpty() const
	{
		return mOpenList.Num() == 0;
	}
	Int2 OpenListTakeLowestF()
	{
		int lowestF = INT_MAX;
		size_t lowestFIndex = 0;

		for (size_t iIndex = 0; iIndex < mOpenList.Num(); iIndex++)
		{
			int fs = GetCell(mOpenList[iIndex])->mFScore;
			if (fs <= lowestF)
			{
				lowestF = fs;
				lowestFIndex = iIndex;
			}
		}

		Int2 ret = mOpenList[lowestFIndex];
		GetCell(ret)->mIsInOpenList = false;
		GetCell(ret)->mIsInClosedList = true;
		mOpenList.RemoveAtSwap(lowestFIndex);
		return ret;
	}
	void OpenListAdd(Int2 cell)
	{
		mOpenList.Add(cell);
	}

	void ReconstructPath(Int2 current, FRahCell* cameFrom)
	{
		this->mResultPathPtr.Reset();

		this->mResultPathPtr.Add(GetCell(current));

		while (cameFrom)
		{
			this->mResultPathPtr.Add(cameFrom);
			cameFrom = cameFrom->mCameFrom;
		}

		Algo::Reverse(this->mResultPathPtr);
	}
	bool IsResultPath(int x, int y)
	{
		return this->mResultPathPtr.Find(GetCell(x, y)) != INDEX_NONE;
	}
	int DistBetween(Int2 a, Int2 b)
	{
		return HeuristicCostEstimate(a, b);
	}
	void FindPath(Int2 srcIndex, Int2 dstIndex)
	{
		{
			for (FRahCell& cell : mCells)
			{
				cell.mCameFrom = nullptr;
				cell.mFScore = INT_MAX;
				cell.mGScore = INT_MAX;
				cell.mIsInClosedList = false;
				cell.mIsInOpenList = false;
			}
			mOpenList.Reset();
		}
		FRahCell* lastSet = nullptr;


		FRahCell* dstCell = GetCell(dstIndex);
		FRahCell* srcCell = GetCell(srcIndex);
		//the cost of going from start to start is zero
		srcCell->mGScore = 0;
		//for the first node the value is completely heuristic
		srcCell->mFScore = HeuristicCostEstimate(srcIndex, dstIndex);

		mOpenList.Add(srcIndex);

		while (!OpenListIsEmpty())
		{
			Int2 currentIndex = OpenListTakeLowestF();
			FRahCell* currentCell = GetCell(currentIndex);

			if (currentCell == dstCell)
			{
				ReconstructPath(currentIndex, lastSet);
				UE_LOG(LogTemp, Display, TEXT("Path Found"));
				return;
			}


			const Int2 neighbousOffset[] =
			{
				Int2(1, 0), Int2(-1, 0), Int2(0, 1), Int2(0, -1)
			};

			for (unsigned iNeighbour = 0; iNeighbour < 4; iNeighbour++)
			{
				Int2 neighbourIndex = currentIndex + neighbousOffset[iNeighbour];

				if (neighbourIndex >= Int2(0, 0) && neighbourIndex < Int2(mNumCellInX, mNumCellInY))
				{
					FRahCell* neighbor = GetCell(neighbourIndex);

					if (neighbor->mIsBlock)
						continue;

					if (neighbor->mIsInClosedList)
						continue;

					if (!neighbor->mIsInOpenList)
					{
						neighbor->mIsInOpenList = true;
						OpenListAdd(neighbourIndex);
					}
					// The distance from start to a neighbor
					int tentative_gScore = currentCell->mGScore + DistBetween(currentIndex, neighbourIndex);
					if (tentative_gScore >= neighbor->mGScore)
						continue;	// This is not a better path.

					lastSet = currentCell;

					neighbor->mCameFrom = currentCell;
					neighbor->mGScore = tentative_gScore;
					neighbor->mFScore = neighbor->mGScore + HeuristicCostEstimate(neighbourIndex, dstIndex);
				}

			}
		}
	}
};



URahTestGridComponent::URahTestGridComponent()
{
	mGrid = new FRahGridTest;
	mGrid->FillRnd();

	DstX = rand() % NumCellInX;
	DstY = rand() % NumCellInY;
	SrcX = rand() % NumCellInX;
	SrcY = rand() % NumCellInY;

	mGrid->GetCell(DstX, DstY)->mIsBlock = false;
	mGrid->GetCell(SrcX, SrcY)->mIsBlock = false;
}

URahTestGridComponent::~URahTestGridComponent()
{
	delete mGrid;
	mGrid = nullptr;
}

void URahTestGridComponent::BeginPlay()
{
	Super::BeginPlay();
}
// void URahTestGridComponent::GetCellUnderMouse()
// {
// 	GetWorld()->GetFirstPlayerController()->GetHitResultAtScreenPosition(mp, ECC_Vehicle, true, hitResult)
// }
void URahTestGridComponent::GenerateMesh()
{
	TArray<FVector> verts;
	TArray<int> tris;
	TArray<FVector> norms;
	TArray<FVector2D> uvs;
	TArray<FColor> colors;
	TArray<FProcMeshTangent> tangs;

	float cellW = CellSize;
	float cellH = CellSize;

	for (int x = 0; x < mGrid->mNumCellInX; x++)
	{
		for (int y = 0; y < mGrid->mNumCellInY; y++)
		{
			float xs = (mGrid->mNumCellInX * cellW * 0.5) - x * cellW;
			float ys = (mGrid->mNumCellInY * cellH * 0.5) - y * cellH;

			float xe = xs + cellW;
			float ye = ys + cellH;

			float zz = 0;
			//adding 4 vertices clock wise
			int indexStart =  verts.Add(FVector(xs, ys, zz));
			verts.Add(FVector(xe, ys, zz));
			verts.Add(FVector(xe, ye, zz));
			verts.Add(FVector(xs, ye, zz));

			tris.Append({ indexStart + 2, indexStart + 1, indexStart + 0 });
			tris.Append({ indexStart + 3, indexStart + 2, indexStart + 0 });

			//tris.Append({indexStart + 0, indexStart + 1, indexStart + 2});
			//tris.Append({indexStart + 0, indexStart + 2, indexStart + 3});
			
			FColor quadColor = FColor::White;
			
			if (mGrid->GetCell(x, y)->mIsBlock)
				quadColor = FColor::Red;
			if (DstX == x && DstY == y) //is Dst?
				quadColor = FColor::Green;
			if (SrcX == x && SrcY == y) //is Src?
				quadColor = FColor::Blue;

			if (mGrid->mResultPathPtr.Find(mGrid->GetCell(x, y)) != INDEX_NONE)
				quadColor = FColor::Yellow;
			
			colors.Append({ quadColor, quadColor, quadColor, quadColor });
		}
	}

	this->CreateMeshSection(0, verts, tris, norms, uvs, colors, tangs, false);
}

void URahTestGridComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URahTestGridComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
}
void URahTestGridComponent::OnRegister()
{
	Super::OnRegister();
	GenerateMesh();
	//this->SetMaterial(0, LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/M_GridTest.M_GridTest")));
}

void URahTestGridComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		FName prpName = PropertyChangedEvent.Property->GetFName();
		if (GET_MEMBER_NAME_CHECKED(URahTestGridComponent, DstX) == prpName
			|| GET_MEMBER_NAME_CHECKED(URahTestGridComponent, DstY) == prpName
			|| GET_MEMBER_NAME_CHECKED(URahTestGridComponent, SrcX) == prpName
			|| GET_MEMBER_NAME_CHECKED(URahTestGridComponent, SrcY) == prpName)
		{
			
		}
		if (prpName == GET_MEMBER_NAME_CHECKED(URahTestGridComponent, NumCellInX)
			|| prpName == GET_MEMBER_NAME_CHECKED(URahTestGridComponent, NumCellInY))
		{
			mGrid->Reset(NumCellInX, NumCellInY);
		}

	}
	
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
	mGrid->FindPath(Int2(SrcX, SrcY), Int2(DstX, DstY));
	GenerateMesh();
}

ARahTestGrid::ARahTestGrid()
{
	Grid = CreateDefaultSubobject<URahTestGridComponent>("Grid");

}

ARahTestGrid::~ARahTestGrid()
{

}




URahGraphComponent::URahGraphComponent()
{
	mGrapth = new FRahGraph;
}

URahGraphComponent::~URahGraphComponent()
{
	delete mGrapth;
	mGrapth = nullptr;
}

ARahGraph::ARahGraph()
{
	GraphComponent = CreateDefaultSubobject<URahGraphComponent>("Graph");
}

ARahGraph::~ARahGraph()
{

}

FRahGraph* ARahGraph::GetGraph() const
{
	if (GraphComponent && GraphComponent->mGrapth)
		return GraphComponent->mGrapth;
	return nullptr;
}
