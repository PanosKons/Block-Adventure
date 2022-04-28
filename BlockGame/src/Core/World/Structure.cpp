#include <Engine.h>
#include "Structure.h"
#include "SavingData.h"
Structure::Structure(Vector3<int> Center, const char* path)
	:Center(Center)
{
	data = SavingData::LoadStructure(path);
}
Structure::Structure(Vector3<int> Center)
	: Center(Center)
{}