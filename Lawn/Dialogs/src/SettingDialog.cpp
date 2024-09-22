#include "../include/SettingDialog.h"
#include "../../Lawn/include/Constant.h"
#include "../../include/LawnApp.h"
#include "../include/Font.h"

Lawn::SettingDialog::SettingDialog(sgf::GameApp* app) : LawnDialog(LAWN_DIALOG_SETTING,app)
{
	mEnabledModel = true;
	mUseFullScreen = app->mUseFullScreen;

	mOKButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_OK,mApp);
	mOKButton->Resize(130,430,170,50);
	mOKButton->AttachToListener(this);
	mOKButton->LoadLabel(_LS("OK"));
	AppendChild(mOKButton);

	mVolumeSlider = new LawnSlider(SLIDER_VOLUME, mApp);
	mVolumeSlider->MoveTo(100, 165);
	mVolumeSlider->AttachToListener(this);
	mVolumeSlider->SetRange(100,0);
	mVolumeSlider->BindToFloat(&gLawnApp->mMusicVolume);
	AppendChild(mVolumeSlider);

	mSoundVolumeSlider = new LawnSlider(SLIDER_SOUND_VOLUME, mApp);
	mSoundVolumeSlider->MoveTo(100, 235);
	mSoundVolumeSlider->AttachToListener(this);
	mSoundVolumeSlider->SetRange(100, 0);
	mSoundVolumeSlider->BindToFloat(&gLawnApp->mSoundVolume);
	AppendChild(mSoundVolumeSlider);

	mFullScreenCheckBox = new LawnCheckBox(CHECKBOX_FULLSCREEN, mApp);
	mFullScreenCheckBox->MoveTo(100, 260);
	mFullScreenCheckBox->AttachToListener(this);
	mFullScreenCheckBox->BindToBool(&mUseFullScreen);
	mFullScreenCheckBox->SetLabel(_LS("FullScreen"));
	mFullScreenCheckBox->mIsCheck = mUseFullScreen;
	AppendChild(mFullScreenCheckBox);

	auto font = mApp->mResourceManager.GetResourceFast<sgf::Font>("FONT_FONT2");
	mBackImage = mApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_OPTIONS_MENUBACK");

	mMusicVolumeLabel = new LawnLabel(mApp);
	mMusicVolumeLabel->MoveTo(100, 125);
	mMusicVolumeLabel->LoadLabel(_LS("MusicVolume"),25);
	mMusicVolumeLabel->SetColor({ 1.0f,0.73f,0.38f,1 });
	AppendChild(mMusicVolumeLabel);

	mSoundVolumeLabel = new LawnLabel(mApp);
	mSoundVolumeLabel->MoveTo(100, 195);
	mSoundVolumeLabel->LoadLabel(_LS("SoundVolume"), 25);
	mSoundVolumeLabel->SetColor({ 1.0f,0.73f,0.38f,1 });
	AppendChild(mSoundVolumeLabel);


	Resize(0,0,425,500);
	MoveToCenter(mApp->mWidth, mApp->mHeight);
}

Lawn::SettingDialog::~SettingDialog()
{
	Widget::~Widget();
	mWidgetManager->RemoveWidget(mOKButton);
	mWidgetManager->RemoveWidget(mVolumeSlider);
	mWidgetManager->RemoveWidget(mMusicVolumeLabel);
	mWidgetManager->RemoveWidget(mFullScreenCheckBox);
	mWidgetManager->RemoveWidget(mSoundVolumeLabel);
	mWidgetManager->RemoveWidget(mSoundVolumeSlider);

	delete mMusicVolumeLabel;
	delete mOKButton;
	delete mVolumeSlider;
	delete mFullScreenCheckBox;
	delete mSoundVolumeLabel;
	delete mSoundVolumeSlider;
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
	case CHECKBOX_FULLSCREEN:
		gLawnApp->mMusicManager.PlayChunk("CHUNK_BUTTONCLICK");
		mFullScreenCheckBox->UpdateState();
		if (mUseFullScreen)
			mApp->UseFullScreen();
		else
			mApp->ExitFullScreen();
		mApp->mUseFullScreen = mFullScreenCheckBox->mIsCheck;
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
