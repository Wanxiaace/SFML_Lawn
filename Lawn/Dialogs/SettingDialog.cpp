#include "SettingDialog.h"
#include "../Constant.h"
#include "../LawnApp.h"
#include "Font.h"
#include "../Resources.h"

Lawn::SettingDialog::SettingDialog() : LawnDialog(LAWN_DIALOG_SETTING)
{
	mEnabledModel = true;
	mUseFullScreen = gLawnApp->mUseFullScreen;

	mOKButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_OK);
	mOKButton->Resize(130,430,170,50);
	mOKButton->AttachToListener(this);
	mOKButton->LoadLabel(_LS("OK"));
	AppendChild(mOKButton);

	mVolumeSlider = new LawnSlider(SLIDER_VOLUME);
	mVolumeSlider->MoveTo(100, 165);
	mVolumeSlider->AttachToListener(this);
	mVolumeSlider->SetRange(100,0);
	mVolumeSlider->BindToFloat(&gLawnApp->mMusicVolume);
	AppendChild(mVolumeSlider);

	mSoundVolumeSlider = new LawnSlider(SLIDER_SOUND_VOLUME);
	mSoundVolumeSlider->MoveTo(100, 235);
	mSoundVolumeSlider->AttachToListener(this);
	mSoundVolumeSlider->SetRange(100, 0);
	mSoundVolumeSlider->BindToFloat(&gLawnApp->mSoundVolume);
	AppendChild(mSoundVolumeSlider);

	mFullScreenCheckBox = new LawnCheckBox(CHECKBOX_FULLSCREEN);
	mFullScreenCheckBox->MoveTo(100, 260);
	mFullScreenCheckBox->AttachToListener(this);
	mFullScreenCheckBox->BindToBool(&mUseFullScreen);
	mFullScreenCheckBox->SetLabel(_LS("FullScreen"));
	mFullScreenCheckBox->mIsCheck = mUseFullScreen;
	AppendChild(mFullScreenCheckBox);

	auto font = RES_FONT::FONT_FONT2;
	mBackImage = RES_IMAGE::IMAGE_OPTIONS_MENUBACK;

	mMusicVolumeLabel = new LawnLabel();
	mMusicVolumeLabel->MoveTo(100, 125);
	mMusicVolumeLabel->LoadLabel(_LS("MusicVolume"),25);
	mMusicVolumeLabel->SetColor({ 1.0f,0.73f,0.38f,1 });
	AppendChild(mMusicVolumeLabel);

	mSoundVolumeLabel = new LawnLabel();
	mSoundVolumeLabel->MoveTo(100, 195);
	mSoundVolumeLabel->LoadLabel(_LS("SoundVolume"), 25);
	mSoundVolumeLabel->SetColor({ 1.0f,0.73f,0.38f,1 });
	AppendChild(mSoundVolumeLabel);


	Resize(0,0,425,500);
	MoveToCenter(gLawnApp->mWidth, gLawnApp->mHeight);
}

Lawn::SettingDialog::~SettingDialog()
{

}


void Lawn::SettingDialog::Draw(sgf::Graphics* g)
{
	//LawnDialog::Draw(g);
	g->SetCubeColor({1,1,1,1});
	g->MoveTo(0,0);
	if (mBackImage)
		g->DrawImage(mBackImage);
}

void Lawn::SettingDialog::Update()
{
	LawnDialog::Update();
}

void Lawn::SettingDialog::OnClick(int buttonId)
{
	switch (buttonId)
	{
	case LAWN_WIDGET_BUTTON_OK:
		gLawnApp->mSettingDialog = nullptr;
		gLawnApp->SafeDeleteWidget(this);
		gLawnApp->mMusicManager.PlayChunk("CHUNK_BUTTONCLICK");
		break;
	case SLIDER_VOLUME:
		gLawnApp->mMusicManager.PlayChunk("CHUNK_BUTTONCLICK");
		break;
	case SLIDER_SOUND_VOLUME:
		gLawnApp->mMusicManager.PlayChunk("CHUNK_BUTTONCLICK");
		break;
	case CHECKBOX_FULLSCREEN:
		gLawnApp->mMusicManager.PlayChunk("CHUNK_BUTTONCLICK");
		mFullScreenCheckBox->UpdateState();
		if (mUseFullScreen)
			gLawnApp->UseFullScreen();
		else
			gLawnApp->ExitFullScreen();
		gLawnApp->mUseFullScreen = mFullScreenCheckBox->mIsCheck;
		break;
	default:
		break;
	}
}

void Lawn::SettingDialog::OnChange(int buttonId)
{
	switch (buttonId)
	{
	case SLIDER_VOLUME:
		gLawnApp->UpdateMusicVolume();
		break;
	case SLIDER_SOUND_VOLUME:
		gLawnApp->UpdateSoundVolume();
		break;
	default:
		break;
	}
}
