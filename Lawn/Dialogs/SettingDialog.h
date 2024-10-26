
#ifndef __LAWN_GAME_SETTING_PAGE__
#define __LAWN_GAME_SETTING_PAGE__
#include "Common.h"
#include "WidgetSystem.h"
#include "Graphics.h"
#include "GameApp.h"
#include "../BasicWidgets.h"

namespace Lawn {
	class SettingDialog :public Lawn::LawnDialog, sgf::WidgetListener {
	protected:
		enum {
			SLIDER_VOLUME = 10000,
			CHECKBOX_FULLSCREEN = 10001,
			SLIDER_SOUND_VOLUME = 10002,
		};
	public:
		LawnStoneButton* mOKButton = nullptr;
		LawnStoneButton* mContinueButton = nullptr;

		LawnLabel* mMusicVolumeLabel = nullptr;
		LawnLabel* mSoundVolumeLabel = nullptr;

		LawnCheckBox* mFullScreenCheckBox = nullptr;
		float mMusicVolume = 0;
		bool mUseFullScreen = false;

		LawnSlider* mVolumeSlider = nullptr;
		LawnSlider* mSoundVolumeSlider = nullptr;
		sgf::SimpleImage* mBackImage = nullptr;

	public:
		SettingDialog();
		~SettingDialog();


	public:

		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;

		virtual void OnClick(int buttonId) override;
		virtual void OnChange(int buttonId) override;
	};
}

#endif