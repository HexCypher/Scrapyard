#pragma once
#include <Windows.h>
#include <WinUser.h>	

class Spells
{
public:
	int spellId;
	int vKey;
	DWORD timer;
};
void castSpell(int spell);

void sendEvent(int vKey, int timer);