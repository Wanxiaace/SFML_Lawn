#pragma once
#ifndef __RESOURCE_GEN_CONSOLE_PAGE__
#define __RESOURCE_GEN_CONSOLE_PAGE__

#include "ImguiLayer.h"

class ConsolePage : public ImguiLayer {
public:
	ConsolePage(void* app) :ImguiLayer(app) {};
	~ConsolePage() {};

public:
	virtual void Draw() override;
	virtual void Update() override;

};

#endif