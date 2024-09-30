#include <iostream>
#include "GameApp.h"
#include "ResGenApp.h"
#undef main;


int main() {
	gApp = new ResGenApp(1280,720,"ResourceGen",true, true);
	gApp->mResourceManager.AttachBasePath("assets/");
	gApp->LoadDict("lang_zh.json");
	gApp->LoadGraphics();
	gApp->EnterMainLoop();

	delete gApp;
	return 0;
}