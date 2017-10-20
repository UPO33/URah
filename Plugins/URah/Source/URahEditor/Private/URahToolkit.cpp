#include "URahToolkit.h"

#if WITH_EDITOR
void URahEdModeToolkitProperties::PostEditChangeProperty(struct FPropertyChangedEvent& pce)
{
	Super::PostEditChangeProperty(pce);
}
#endif