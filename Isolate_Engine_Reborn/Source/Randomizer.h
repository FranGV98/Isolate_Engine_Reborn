#ifndef __RANDOMIZE_H__
#define __RANDOMIZE_H__

#include "Globals.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"

namespace Randomizer
{
	static math::LCG lcg_rand;

	uint32	GetRandomUint();
	uint32	GetBoundedRandomUint(uint32 min = 0, uint32 max = 64);
	float	GetBoundedRandomFloat(float min, float max);
}

#endif
