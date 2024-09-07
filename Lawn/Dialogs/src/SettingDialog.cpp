#include "../include/SettingDialog.h"
#include "../../Lawn/include/Constant.h"
#include "../../include/LawnApp.h"
#include "../include/Font.h"

Lawn::SettingDialog::SettingDialog(sgf::GameApp* app) : LawnDialog(LAWN_DIALOG_SETTING,app)
{

	mOKButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_OK,mApp);
	mOKButton->Resize(500,500,170,50);
	mOKButton->AttachToListener(this);
	mOKButton->LoadLabel(_LS("OK"));
	AppendChild(mOKButton);

	mVolumeSlider = new LawnSlider(SLIDER_VOLUME, mApp);
	mVolumeSlider->MoveTo(100, 200);
	mVolumeSlider->AttachToListener(this);
	mVolumeSlider->SetRange(100,0);
	mVolumeSlider->BindToFloat(&gLawnApp->mMusicVolume);
	AppendChild(mVolumeSlider);

	auto font = (sgf::Font*)mApp->mResourceManager.mResourcePool["FONT_FONT2"];


	font->SetFontSize(35);
	mCaptainImage = font->GenTextImage(_LS("Settings"));

	mMusicVolumeLabel = new LawnLabel(mApp);
	mMusicVolumeLabel->MoveTo(100, 150);
	mMusicVolumeLabel->LoadLabel(_LS("MusicVolmne"));
	mMusicVolumeLabel->SetColor({ 1.0f,0.73f,0.38f,1 });
	AppendChild(mMusicVolumeLabel);


	Resize(0,0,800,600);
	MoveToCenter(mApp->mWidth, mApp->mHeight);
}

Lawn::SettingDialog::~SettingDialog()
{
	Widget::~Widget();
	mWidgetManager->RemoveWidget(mOKButton);
	mWidgetManager->RemoveWidget(mVolumeSlider);
	mWidgetManager->RemoveWidget(mMusicVolumeLabel);

	delete mMusicVolumeLabel;
	delete mOKButton;
	delete mVolumeSlider;
}

void Lawn::SettingDialog::Draw(sgf::Graphics* g)
{
	LawnDialog::Draw(g);

	g->MoveTo((800 - mCaptainImage->mSurface->w) / 2, 100);
	g->Translate(-1, -1);
	g->SetCubeColor({ 0,0,0,1 });
	g->DrawImage(mCaptainImage);

	g->Translate(1,1);
	g->SetCubeColor({1.0f,0.73f,0.38f,1});
	g->DrawImage(mCaptainImage);

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
	default:
		break;
	}
}
