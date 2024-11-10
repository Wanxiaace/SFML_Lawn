#include <iostream>
#include <fstream>

#include "LawnApp.h"
#include "Resources.h"
#include "Constant.h"
#include "LogoScreen.h"
#include "GamePacker/GamePacker.h"

#undef main

int main(int argc,char** argv) {

	std::ofstream out("log.txt");
#ifndef _DEBUG
	sgf::SetStdOutStream(out);
#endif

	gLawnApp = new Lawn::LawnApp(LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT, LAWN_GAME_WINDOW_CAPTAIN, true, true);
	if (argc > 1 && std::string(argv[1]) == "-wxdebug")
		gLawnApp->mShowDebugWindow = true;

	sgf::FileManager::TryToLoadPak("assets/en2pak/en2pak.pkg");

	gLawnApp->mResourceManager.AttachBasePath("assets/en2pak/");
	gLawnApp->LoadDict("LawnStrings.json");
	gLawnApp->SetWindowCaptain(_LS("GameTitle"));
	gLawnApp->WindowsEnhance();
	gLawnApp->LoadGraphics();
	gLawnApp->LoadPlayerInfo("archive/player1.sgfbin");

	gLawnApp->EnableMultThreadUpdate(Lawn::GameUpdateThread);

	sgf::LogoScreen* logoScreen = new sgf::LogoScreen(gLawnApp);

	logoScreen->SetNextScreenFunc([]() {
		gLawnApp->EnterLoadingPage();
		});

	sgf::SimpleImage* logo = new sgf::SimpleImage();
	logo->LoadFromFile("data/sgf.png");

	logoScreen->AppendLogoImage(logo,0.2f);

	gLawnApp->SafeAppendWidget(logoScreen);
	//gLawnApp->mWidgetManager->AddWidget

	new std::thread([]() {
		gLawnApp->LoadResources((gLawnApp->mResourceManager.mBasePath + "ResourceList.xml").c_str());
		LoadAllResourcesLink();		
		});

	gLawnApp->EnterMainLoop();

	gLawnApp->LockUpdate();

	delete gLawnApp;
	out.close();
}
