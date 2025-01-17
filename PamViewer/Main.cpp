#include "Graphics.h"
#include "GameApp.h"

sgf::GameApp* gPamViewerApp;

#undef main

int main() {
	gPamViewerApp = new sgf::GameApp(800,600,"PamViewer");

	gPamViewerApp->LoadGraphics();
	gPamViewerApp->EnterMainLoop();


	delete gPamViewerApp;
	return EXIT_SUCCESS;
}