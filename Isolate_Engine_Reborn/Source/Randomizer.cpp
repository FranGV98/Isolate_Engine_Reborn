
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "Randomizer.h"


// --- LCG ---
uint32 Randomizer::GetRandomUint()
{
	return (uint32)lcg_rand.Int();
}

uint32 Randomizer::GetBoundedRandomUint(uint32 min, uint32 max)
{
	return (uint32)lcg_rand.Int(min, max);
}

float Randomizer::GetBoundedRandomFloat(float min, float max)
{
	return lcg_rand.Float(min, max);
}