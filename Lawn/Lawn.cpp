#include "include/LawnApp.h"
#include "include/Constant.h"

#undef main

int main(int argc,char** argv) {
	gLawnApp = new Lawn::LawnApp(LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT, LAWN_GAME_WINDOW_CAPTAIN, true, true);
	if (argc > 1 && std::string(argv[1]) == "-wxdebug")
		gLawnApp->mShowDebugWindow = true;
	

	gLawnApp->mResourceManager.AttachBasePath("assets/en/");
	gLawnApp->LoadDict("LawnStrings.json");
	gLawnApp->SetWindowCaptain(_LS("GameTitle"));
	gLawnApp->SetWindowIcon("data/WindowIcon.png");
	gLawnApp->LoadGraphics();
	gLawnApp->LoadPlayerInfo("archive/player1.sgfbin");

	gLawnApp->EnterLoadingPage();

	gLawnApp->LawnStart();
}
