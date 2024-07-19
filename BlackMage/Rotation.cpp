#include "Rotation.h"


void opener()
{
	int opener[]{ 1, 7, 8, 2, 3, 4, 5, 5, 5, 5 };

	MMRESULT timeBeginPeriod(10);

	for (int i = 0; i < _countof(opener); ++i)
	{
		castSpell(opener[i]);
	}

	MMRESULT timeEndPeriod(10);
}