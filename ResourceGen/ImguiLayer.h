#pragma once
#include "imgui.h"
#include "Graphics.h"

class ImguiLayer {
public:
	void* mApp = nullptr;
	
public:
	ImguiLayer(void* app);
	~ImguiLayer();
	void AppendApp(void* app);

public:
	virtual void Draw();
	virtual void SGFDraw(sgf::Graphics* g);
	virtual void Update();
};