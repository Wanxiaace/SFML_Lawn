#include "Graphics.h"
#include "GameApp.h"
#include "PopAnim.h"
#include <functional>

sgf::GameApp* gPamViewerApp;
sgf::Graphics* gGraphics;
sgf::PopAnim* gPopAnim;
sgf::PopAnim* gPopAnim2;

#undef main

static float counter = 0;

void Display(sgf::GameAppBase* app,int ms) {

	counter += app->mTick.GetDeltaTickFloat() / 30.0f;
	if (counter > 300)
		counter = 0;

	gGraphics->Clear();
	gGraphics->ModelMoveTo(200,200);
	gGraphics->MoveTo(0,0);
	gGraphics->SetCubeColor({1,1,1,1});


	gPopAnim2->Present(gGraphics, counter);
	gGraphics->Present();
}

int main() {
	gPamViewerApp = new sgf::GameApp(800,600,"PamViewer");
	gPopAnim = new sgf::PopAnim();
	gPopAnim2 = new sgf::PopAnim();
	gPopAnim->LoadFromPamFile("ZOMBIE_TUTORIAL.PAM");
	gPopAnim2->LoadFromJsonFile("ZOMBIE_TUTORIAL.json");

	std::function<void(sgf::GameAppBase*, int)> func = Display;

	gPamViewerApp->SetDisplayFunction(func);
	gGraphics = gPamViewerApp->LoadGraphics();

	gPamViewerApp->EnterMainLoop();


	delete gPamViewerApp;
	return EXIT_SUCCESS;
}