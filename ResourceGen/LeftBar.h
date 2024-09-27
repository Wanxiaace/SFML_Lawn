#pragma once
#include "ImguiLayer.h"

class TreeBar : public ImguiLayer {
public:
	TreeBar(void* app) :ImguiLayer(app) {};
	~TreeBar() {};


	void SelectRes(int index);
public:
	virtual void Draw() override;
	virtual void Update() override;

};