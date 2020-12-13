
#include "Resource.h"
#include "Randomizer.h"

Resource::Resource(RESOURCE_TYPE type) : 
type	(type), 
uid		(Randomizer::GetBoundedRandomUint()) //Generate UID
{

}

Resource::~Resource()
{

}

void Resource::GenerateNewUID()
{
	uid = Randomizer::GetRandomUint();
}