#include "stdafx.h"

#include "utils.h"

bool CheckRequiredFiles()
{
	static char szBuffer[MAX_PATH];
	static char * _files[] ={"media/ground.tga",
		"media/ambient.wav",
		"media/explode.wav",
		"media/explosion.bmp",
		"models/Rocket/rocketair.MD2",
		"models/Rocket/Rocket.PCX",
		"models/Ogro/Ogrobase.pcx",
		"models/Ogro/Tris.MD2",
		//"models/Ogro/Weapon.md2",
		//"models/Ogro/Weapon.pcx",
		//"models/Ogro/igdosh.pcx",
		"models/Sodf8/Abarlith.pcx",
		"models/Sodf8/Tris.MD2",
		//"models/Sodf8/Weapon.PCX",
		//"models/Sodf8/Weapon.md2"
	};

	for (int i=0; i<sizeof(_files)/sizeof(_files[0]); i++)
	{
		if (_access(_files[i], 0) == -1)
		{
			sprintf_s(szBuffer, sizeof(szBuffer), "Required file \"%s\" is missing!", _files[i]);
			MessageBox(NULL, szBuffer, "Error", MB_ICONSTOP);
			return false;
		}
	}
	return true;
}