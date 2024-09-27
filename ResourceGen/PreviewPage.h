#pragma once
#ifndef __RESOURCE_GEN_PREVIEW_PAGE__
#define __RESOURCE_GEN_PREVIEW_PAGE__

#include "ImguiLayer.h"

class PreviewPage : public ImguiLayer {
public:
	PreviewPage(void* app) :ImguiLayer(app) {};
	~PreviewPage() {};

public:
	virtual void Draw() override;
	virtual void Update() override;

};

#endif