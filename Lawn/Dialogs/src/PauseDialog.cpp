#include "../include/PauseDialog.h"
#include "../../include/Constant.h"
#include "../../include/LawnApp.h"

Lawn::PauseDialog::PauseDialog(sgf::GameApp* app) : LawnDialog(LAWN_WIDGET_PAUSE_DIALOG, app)
{
	mEnabledModel = true;
	Resize(0,0,420,500);
	MoveToCenter(mApp->mWidth, mApp->mHeight);

	mContinueButton = new LawnStoneButton(PAUSE_DIALOG_CONTINUE,mApp);
	mContinueButton->AttachToListener(this);
	mContinueButton->Resize(120,400,200,50);
	mContinueButton->LoadLabel(_LS("Continue"));
	AppendChild(mContinueButton);

	mMenuButton = new LawnStoneButton(PAUSE_DIALOG_MENU, mApp);
	mMenuButton->AttachToListener(this);
	mMenuButton->Resize(120, 350, 200, 50);
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

		break;
	}
	default:
		break;
	}
}
