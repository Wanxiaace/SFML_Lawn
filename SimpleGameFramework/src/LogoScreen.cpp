#include "../include/LogoScreen.h"

sgf::LogoScreen::LogoScreen(GameApp* app):
	Widget(0x5500)
{
	mApp = app;
	mShowCounterMax = 2000;
	mShowCounter = mShowCounterMax;
}

sgf::LogoScreen::~LogoScreen()
{

}

void sgf::LogoScreen::AppendLogoImage(SimpleImage* logo,float scale)
{
	mScaleF = scale;
	mLogoImage = logo;
}

void sgf::LogoScreen::Update()
{
	/*static unsigned int tickBuffer = sgf::TryGetTicks();
	unsigned int tick = sgf::TryGetTicks();

	mTickDelta = tick - tickBuffer;
	tickBuffer = tick;
	*/
	mScaleF += 0.05f * float(mTickDelta) / mShowCounterMax;

	

	if (mShowCounter > mTickDelta)
		mShowCounter -= mTickDelta;
	else
	{
		if (!mIsEnterNextScreen)
		{
			mNextFunc();
			mShowCounter = 0;
			mApp->SafeDeleteWidget(this);
			mIsEnterNextScreen = true;
		}
	}
}

void sgf::LogoScreen::Draw(Graphics* g)
{
	float offsetX = (mApp->mWidth - mLogoImage->GetWidth() * mScaleF) / 2.0f;
	float offsetY = (mApp->mHeight - mLogoImage->GetHeight() * mScaleF) / 2.0f;

	g->MoveTo(offsetX, offsetY);
	g->SetCubeColor({1,1,1,float(mShowCounter) / mShowCounterMax });
	if (mLogoImage) {
		g->DrawImageScaleF(mLogoImage, mScaleF, mScaleF);
	}
	
}
