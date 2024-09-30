#ifndef __RESOURCE_GEN_APP
#define __RESOURCE_GEN_APP

#define _LS(x) gApp->mDictionary.GetString(x)
#define _LS_C(x) gApp->mDictionary.GetString(x).c_str()

#include "GameApp.h"
#include "LeftBar.h"
#include "ImguiLayer.h"
#include "PreviewPage.h"
#include "ConsolePage.h"

#include "Animator.h"

class ImguiLayer;

class ResGenApp : public sgf::GameApp {
public:
	std::vector<ImguiLayer*> mLayers;
	TreeBar* mLeftBar = nullptr;
	PreviewPage* mPreviewPage = nullptr;
	ConsolePage* mConsolePage = nullptr;

	sgf::ResourceList* mResourceList = nullptr;

	bool* mSelectedBoolList = nullptr;
	
	int mCurentBoolIndex = -1;
	int mCurentTrackIndex = -1;

	sgf::SimpleImage* mCurentImagePointer = nullptr;

	bool mSelectedTrackBoolList[100] = {false};
	sgf::Animator* mCurentAnimatorPointer = nullptr;
	bool mIsAtlas = false;

public:
	ResGenApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync = true, bool resiziable = false);
	~ResGenApp();

	void AppendLayer(ImguiLayer* layer);
	void RenderMenuBar();
	void LoadAllResource();

public:
	virtual void DrawImgui() override;
	virtual void Draw() override;
};

extern ResGenApp* gApp;

#endif // !__RESOURCE_GEN_APP
