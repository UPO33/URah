#pragma once

#include "UObject/ObjectMacros.h"

#include "URahToolkit.generated.h"

class FURahEdModeToolkit;
class ARahGraph;

//////////////////////////////////////////////////////////////////////////
UCLASS()
class URahEdModeToolkitProperties : public UObject
{
public:
	GENERATED_BODY()

	FURahEdModeToolkit* OwnerToolkit;

	UPROPERTY(EditAnywhere)
	ARahGraph* TargetGraph;
	UPROPERTY(EditAnywhere)
		float Radius;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};