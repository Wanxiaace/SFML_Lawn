#include "Graphics.h"
#include "GameApp.h"
#include "PopAnim.h"
#include <functional>

sgf::GameApp* gPamViewerApp;
sgf::Graphics* gGraphics;
sgf::PopAnim* gPopAnim;

#undef main

static float counter = 0;

void Display(sgf::GameAppBase* app,int ms) {

	counter += app->mTick.GetDeltaTickFloat() / 30.0f;
	if (counter > 100)
		counter = 0;
	
	gGraphics->Clear();
	gGraphics->SetCubeColor({ 1,1,1,1 });
	gGraphics->ModelMoveTo(0, 0);
	gGraphics->MoveTo(0, 0);
	gGraphics->FillRect({ 0,0,1000,1000 });

	gGraphics->ModelMoveTo(0,0);
	gGraphics->MoveTo(0,0);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gGraphics->MoveTo(j * 100 - 100, i * 100 - 100);
			gPopAnim->PresentScale(gGraphics, counter, 0.5f);
		}
	}

	gGraphics->Present();
}

int main() {
	gPamViewerApp = new sgf::GameApp(1000, 1000,"PamViewer");
	gPopAnim = new sgf::PopAnim();
	gPopAnim->LoadFromJsonFile("CHERRYBOMB.json");
	//gPopAnim->LoadFromJsonFile("CELERYSTALKER.json");

	std::function<void(sgf::GameAppBase*, int)> func = Display;

	gPamViewerApp->SetDisplayFunction(func);
	gGraphics = gPamViewerApp->LoadGraphics();

	gPamViewerApp->EnterMainLoop();


	delete gPamViewerApp;
	return EXIT_SUCCESS;
}