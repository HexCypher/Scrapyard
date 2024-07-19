#include "Spells.h"
#include <iostream>

Spells fire;
Spells fire3;
Spells fire4;
Spells blizz3;
Spells blizz4;
Spells thunder;
Spells enoch;
Spells leyLines;
Spells sharpCast;

DWORD timer_inst = 600;
DWORD timer_169 = 1690;
DWORD timer_242 = 2420;
DWORD timer_271 = 2710;
DWORD timer_338 = 3380;

void sendEvent(int vKey, int timer)
{
	
	//if (vKey != blizz3.vKey && timer != timer_inst)
	//	timer = timer - (timer / 100 * 15);

	keybd_event(vKey, 0, 0, 0);
	keybd_event(vKey, 0, KEYEVENTF_KEYUP, 0);
	Sleep(timer);
	std::cout << timer << " " << vKey << std::endl;
	return;
}


void castSpell(int spell)
{

	switch (spell)
	{
	case 1: 
		blizz3.timer = timer_338;
		blizz3.vKey = 0x31;
		sendEvent(blizz3.vKey, blizz3.timer);
		break;
	case 2:
		thunder.timer = timer_242;
		thunder.vKey = 0x32;
		sendEvent(thunder.vKey, thunder.timer);
		break;
	case 3:
		blizz4.timer = timer_271;
		blizz4.vKey = 0x33;
		sendEvent(blizz4.vKey, blizz4.timer);
		break;
	case 4:
		fire3.timer = timer_169;
		fire3.vKey = 0x34;
		sendEvent(fire3.vKey, fire3.timer);
		break;
	case 5:
			fire4.timer = timer_242;
			fire4.vKey = 0x35;
			sendEvent(fire4.vKey, fire4.timer);
		break;
	case 6:
		fire.timer = timer_242;
		fire.vKey = 0x36;
		sendEvent(fire.vKey, fire.timer);
		break;
	case 7:
		leyLines.timer = timer_inst;
		leyLines.vKey = 0x37;
		sendEvent(leyLines.vKey, leyLines.timer);
		break;
	case 8:
		enoch.timer = timer_inst;
		enoch.vKey = 0x38;
		sendEvent(enoch.vKey, enoch.timer);
		break;
	case 9:
		sharpCast.timer = timer_inst;
		sharpCast.vKey = 0x39;
		sendEvent(sharpCast.vKey, sharpCast.timer);
		break;
	default:
		break;
	}
	
	return;
}






















/**************************************************************************************************



	keybd_event(vKey, 0, 0, 0);
	keybd_event(vKey, 0, KEYEVENTF_KEYUP, 0);
	Sleeptimer);




***************************************************************************************************/