#pragma once
#include "imgui.h"

class ImguiLayer {
public:
	void* mApp = nullptr;
	
public:
	ImguiLayer(void* app);
	~ImguiLayer();
	void AppendApp(void* app);

public:
	virtual void Draw();
	virtual void Update();
};