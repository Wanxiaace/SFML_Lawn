#include "ImguiLayer.h"

ImguiLayer::ImguiLayer(void* app)
{
	AppendApp(app);
}

ImguiLayer::~ImguiLayer()
{

}

void ImguiLayer::AppendApp(void* app)
{
	mApp = app;
}

void ImguiLayer::Draw()
{

}

void ImguiLayer::SGFDraw(sgf::Graphics* g)
{
}

void ImguiLayer::Update()
{

}
