#include "MyForm.h"
#include "Debug.h"


using namespace BlackMageRota;
[STAThreadAttribute]


void main()
{


	enableDebug();	

	float x = getX(L"REDACTED");

	MyForm form;
	form.ShowDialog();

	return;
}