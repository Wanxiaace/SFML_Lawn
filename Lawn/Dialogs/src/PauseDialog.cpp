#include "../include/PauseDialog.h"
#include "../../include/Constant.h"
#include "../../include/LawnApp.h"

Lawn::PauseDialog::PauseDialog(sgf::GameApp* app) : LawnDialog(LAWN_WIDGET_PAUSE_DIALOG, app)
{
	mEnabledModel = true;
	Resize(0,0,420,500);
	MoveToCenter(mApp->mWidth, mApp->mHeight);

	mContinueButton = new LawnImageButton(PAUSE_DIALOG_CONTINUE,mApp);
	mContinueButton->AttachToListener(this);
	mContinueButton->LoadLabel(_LS("Continue"));
	mContinueButton->mImageNormal = mApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_OPTIONS_BACKTOGAMEBUTTON0");
	mContinueButton->mImageHover = mApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_OPTIONS_BACKTOGAMEBUTTON2");
	mContinueButton->mImageDown = mApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_OPTIONS_BACKTOGAMEBUTTON2");
	mContinueButton->Resize(25, 350, mContinueButton->mImageNormal->GetWidth(), mContinueButton->mImageNormal->GetHeight());
	
	AppendChild(mContinueButton);

	mMenuButton = new LawnStoneButton(PAUSE_DIALOG_MENU, mApp);
	
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
