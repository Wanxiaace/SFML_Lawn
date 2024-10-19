#include "../include/PauseDialog.h"
#include "../../include/Constant.h"
#include "../../include/LawnApp.h"

Lawn::PauseDialog::PauseDialog() : LawnDialog(LAWN_WIDGET_PAUSE_DIALOG)
{
	mEnabledModel = true;
	Resize(0,0,420,500);
	MoveToCenter(gLawnApp->mWidth, gLawnApp->mHeight);

	mContinueButton = new LawnImageButton(PAUSE_DIALOG_CONTINUE);
	mContinueButton->AttachToListener(this);
	mContinueButton->LoadLabel(_LS("Continue"));
	mContinueButton->mImageNormal = RES_IMAGE::IMAGE_OPTIONS_BACKTOGAMEBUTTON0;
	mContinueButton->mImageHover = RES_IMAGE::IMAGE_OPTIONS_BACKTOGAMEBUTTON2;
	mContinueButton->mImageDown = RES_IMAGE::IMAGE_OPTIONS_BACKTOGAMEBUTTON2;
	mContinueButton->Resize(25, 350, mContinueButton->mImageNormal->GetWidth(), mContinueButton->mImageNormal->GetHeight());
	
	AppendChild(mContinueButton);

	mMenuButton = new LawnStoneButton(PAUSE_DIALOG_MENU);
	
	mMenuButton->AttachToListener(this);
	mMenuButton->Resize(120, 300, 165, 50);
	mMenuButton->LoadLabel(_LS("Menu"));
	AppendChild(mMenuButton);

}

Lawn::PauseDialog::~PauseDialog()
{
	mWidgetManager->RemoveWidget(mContinueButton);
	mWidgetManager->RemoveWidget(mMenuButton);

	delete mContinueButton;
	delete mMenuButton;
}

void Lawn::PauseDialog::Draw(sgf::Graphics* g)
{
	LawnDialog::Draw(g);
}

void Lawn::PauseDialog::Update()
{
	LawnDialog::Update();
}

void Lawn::PauseDialog::OnClick(int buttonId)
{
	switch (buttonId)
	{
	case PAUSE_DIALOG_CONTINUE: 
	{
		gLawnApp->KillGamePause();
		break;
	}
	case PAUSE_DIALOG_MENU:
	{
		gLawnApp->KillGamePause();
		if (gLawnApp->mBoard)
			gLawnApp->KillBoard();
		gLawnApp->EnterGameSelector();
		break;
	}
	default:
		break;
	}
}
