#pragma once
#ifndef __RESOURCE_GEN_LEFT_BAR__
#define __RESOURCE_GEN_LEFT_BAR__

#include "ImguiLayer.h"

class TreeBar : public ImguiLayer {
public:
	TreeBar(void* app) :ImguiLayer(app) {};
	~TreeBar() {};


	static void SelectRes(int index);
public:
	virtual void Draw() override;
	virtual void Update() override;

};

#endif