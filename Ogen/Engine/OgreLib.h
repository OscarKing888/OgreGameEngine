#pragma once

#ifdef _DEBUG
#	pragma comment(lib, "OgreMain_d.lib")
#	pragma comment(lib, "OIS_D.lib")
#	pragma comment(lib, "OgreTerrain_d.lib")
#else
#	pragma comment(lib, "OgreMain.lib")
#	pragma comment(lib, "OIS.lib")
#	pragma comment(lib, "OgreTerrain.lib")
#endif