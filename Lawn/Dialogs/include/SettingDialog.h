#pragma once
#ifndef __LAWN_GAME_SETTING_PAGE__
#define __LAWN_GAME_SETTING_PAGE__
#include "Common.h"
#include "WidgetSystem.h"
#include "Graphics.h"
#include "GameApp.h"
#include "../../Lawn/include/BasicWidgets.h"

namespace Lawn {
	class SettingDialog :public Lawn::LawnDialog, sgf::WidgetListener {
	protected:
		enum {
			SLIDER_VOLUME = 10000,
			CHECKBOX_FULLSCREEN = 10001,
		};
	public:
		LawnStoneButton* mOKButton = nullptr;
		LawnStoneButton* mCancelButton = nullptr;
		sgf::SimpleImage* mCaptainImage = nullptr;

		LawnLabel* mMusicVolumeLabel = nullptr;
		LawnCheckBox* mFullScreenCheckBox = nullptr;
		float mMusicVolume = 0;
		bool mUseFullScreen = false;

		LawnSlider* mVolumeSlider = nullptr;

	public:
		SettingDialog(sgf::GameApp* app);
		~SettingDialog();


	public:

		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;

		virtual void OnClick(int buttonId) override;
		virtual void OnChange(int buttonId) override;
	};
}

#endif