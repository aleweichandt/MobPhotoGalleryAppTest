// TestHOF.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include "Model\PhotoGallery.h"
#include "Windows.h"


int _tmain(int argc, _TCHAR* argv[])
{
	PhotoGallery* app = new PhotoGallery();

	unsigned int t = GetTickCount();
	while(app->getState() != PhotoGallery::G_STATE_END)
	{
		unsigned int dt = GetTickCount() - t;
		t = GetTickCount();
		app->update(dt);
	}
	return 0;
}

