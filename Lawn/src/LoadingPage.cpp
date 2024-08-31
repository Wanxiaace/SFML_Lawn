#include "../include/LoadingPage.h"
#include "../include/Constant.h"

Lawn::LoadingPage::LoadingPage(int id,sgf::GameApp* app) :Widget(id)
{
	mApp = app;
	Resize(0, 0, LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);
}

Lawn::LoadingPage::~LoadingPage()
{

}

void Lawn::LoadingPage::OnClick(int theId)
{

}

void Lawn::LoadingPage::Draw(sgf::Graphics* g)
{
	g->ActiveTextureShader();
	g->SetCubeColor({ 0.5f,0.5f,0.5f,1.0f });
	g->FillRect({ 0.0f,LAWN_GAME_WINDOW_HEIGHT - 30,LAWN_GAME_WINDOW_WIDTH,30.0f });

	g->SetCubeColor({ 1.0f,1.0f,1.0f,1.0f });
	g->FillRect({ 0.0f,LAWN_GAME_WINDOW_HEIGHT - 30,mApp->mResourceManager.mProcess * LAWN_GAME_WINDOW_WIDTH / 100.0f,30.0f });
}
