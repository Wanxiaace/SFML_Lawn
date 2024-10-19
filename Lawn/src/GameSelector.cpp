#include "../include/GameSelector.h"
#include "../include/Constant.h"
#include <Font.h>
#include "../include/LawnApp.h"

Lawn::GameSelector::GameSelector(int theId,bool useEntryAnimation) : Widget(theId)
{
	Resize(0, 0, LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);
	if (useEntryAnimation) {
		mPlayingEntryAnimation = true;
		mAnimeTicker = 3000;
		mTickerCache = sgf::TryGetTicks();
	}

	mStartButton = new TextButton(LAWN_WIDGET_START_BUTTON,50);//LAWN_WIDGET_START_BUTTON
	mStartButton->MoveTo(50,400);
	mStartButton->LoadLabel(_LS("Start"));

	mConfigButton = new TextButton(LAWN_WIDGET_CONFIG_BUTTON,50);
	mConfigButton->MoveTo(50, 450);
	mConfigButton->LoadLabel(_LS("Config"));

	mExitButton = new TextButton(LAWN_WIDGET_EXIT_BUTTON,50);
	mExitButton->MoveTo(50, 500);
	mExitButton->LoadLabel(_LS("Exit"));


	mStartButton->AttachToListener(this);
	mConfigButton->AttachToListener(this);
	mExitButton->AttachToListener(this);

	AppendChild(mStartButton);
	AppendChild(mConfigButton);
	AppendChild(mExitButton);

	
}

Lawn::GameSelector::~GameSelector()
{
	Widget::~Widget();
	mWidgetManager->RemoveWidget(mStartButton);
	mWidgetManager->RemoveWidget(mConfigButton);
	mWidgetManager->RemoveWidget(mExitButton);

	delete mStartButton;
	delete mConfigButton;
	delete mExitButton;
}

void Lawn::GameSelector::Update()
{
	unsigned int tick = sgf::TryGetTicks();
	if (mPlayingEntryAnimation && mAnimeTicker > 0) {
		if (mAnimeTicker < (tick - mTickerCache)) {
			mAnimeTicker = 0;
			mPlayingEntryAnimation = false;
		}
		else {
			mAnimeTicker -= (tick - mTickerCache);
		}
		mTickerCache = tick;
	}
}

void Lawn::GameSelector::Draw(sgf::Graphics* g)
{
	if (mPlayingEntryAnimation) {
		g->SetCubeColor({ 1.0f,1.0f,1.0f,float(mAnimeTicker) / 3000.0f });
		g->FillRect({ 0,LAWN_GAME_WINDOW_HEIGHT-30,LAWN_GAME_WINDOW_WIDTH,30 });

		sgf::SimpleImage* background = RES_IMAGE::IMAGE_TITLESCREEN2;
		if (background) {
			g->MoveTo(0,0);
			g->SetCubeColor({ 1.0f,1.0f,1.0f,1.0f - float(mAnimeTicker) / 3000.0f });
			g->DrawImageInRect(background, mRect.mWidth, mRect.mHeight);
		}
	}
	else {
		sgf::SimpleImage* background = RES_IMAGE::IMAGE_TITLESCREEN2;
		if (background) {
			g->MoveTo(0, 0);
			g->SetCubeColor({ 1.0f,1.0f,1.0f,1.0f });
			g->DrawImageInRect(background, mRect.mWidth, mRect.mHeight);
		}
	}

}

void Lawn::GameSelector::OnClick(int buttonId)
{
	switch (buttonId)
	{
	case LAWN_WIDGET_START_BUTTON:
		gLawnApp->KillGameSelector();
		gLawnApp->EnterGameBoard();
		break;
	case LAWN_WIDGET_CONFIG_BUTTON:
		gLawnApp->ShowSettingDialog();
		break;
	case LAWN_WIDGET_EXIT_BUTTON:
		gLawnApp->mIsOpen = false;
		break;
	}
}
