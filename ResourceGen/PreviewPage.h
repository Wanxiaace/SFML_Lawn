#pragma once
#ifndef __RESOURCE_GEN_PREVIEW_PAGE__
#define __RESOURCE_GEN_PREVIEW_PAGE__

#include "ImguiLayer.h"

class ImagePreview : public ImguiLayer {
public:
	bool mVisible = false;
	bool mFocused = false;
	ImVec2 mWindowSize;
	ImVec2 mWindowPos;
	float mScale = 1.0f;
	ImVec2 mDragDelta;
	ImVec2 mMapPos;

public:
	ImagePreview(void* app) :ImguiLayer(app) {};
	~ImagePreview() {};

public:
	virtual void Draw() override;
	virtual void SGFDraw(sgf::Graphics* g) override;
	virtual void Update() override;
};

class ReanimPreview : public ImguiLayer {
public:
	bool mVisible = false;
	bool mFocused = false;
	ImVec2 mWindowSize;
	ImVec2 mWindowPos;
	float mScale = 1.0f;
	ImVec2 mDragDelta;
	ImVec2 mMapPos;

public:
	ReanimPreview(void* app) :ImguiLayer(app) {};
	~ReanimPreview() {};

	void SelectTrack(int index);

public:
	virtual void Draw() override;
	virtual void SGFDraw(sgf::Graphics* g) override;
	virtual void Update() override;
};



class PreviewPage : public ImguiLayer {
public:
	ImagePreview* mImagePreview = nullptr;
	ReanimPreview* mReanimPreview = nullptr;
public:
	PreviewPage(void* app) :ImguiLayer(app) {
		mImagePreview = new ImagePreview(app);
		mReanimPreview = new ReanimPreview(app);
	};
	~PreviewPage() {
		delete mImagePreview;
		delete mReanimPreview;
	};

public:
	virtual void Draw() override;
	virtual void Update() override;
	virtual void SGFDraw(sgf::Graphics* g) override;

};

#endif