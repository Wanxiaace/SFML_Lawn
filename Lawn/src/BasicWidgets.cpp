#include "../include/BasicWidgets.h"
#include <Font.h>
#include "../include/Constant.h"
#include "../include/LawnApp.h"


Lawn::LawnStoneButton::LawnStoneButton(int buttonId,sgf::GameApp* app) :sgf::TemplateButton(buttonId)
{
	mApp = app;
	mImageLeft = (sgf::SimpleImage*)app->mResourceManager.mResourcePool["IMAGE_BUTTON_LEFT"];
	mImageMiddle = (sgf::SimpleImage*)app->mResourceManager.mResourcePool["IMAGE_BUTTON_MIDDLE"];
	mImageRight = (sgf::SimpleImage*)app->mResourceManager.mResourcePool["IMAGE_BUTTON_RIGHT"];
}

Lawn::LawnStoneButton::~LawnStoneButton()
{
	if (mImageLabel)
		delete mImageLabel;
}

void Lawn::LawnStoneButton::LoadLabel(const sgf::String& label)
{
	sgf::Font* font = (sgf::Font*)mApp->mResourceManager.mResourcePool["FONT_FONT3"];
	if (!font)
		return;
	mLabel = label;
	font->SetFontSize(28);
	mImageLabel = font->GenTextImage(mLabel);
}

void Lawn::LawnStoneButton::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({ 1.0f,1.0f,1.0f,1.0f });
	int middleCount = (mRect.mWidth - (mImageLeft->mSurface->w + mImageRight->mSurface->w))/ mImageMiddle->mSurface->w;

	if (mIsMouseCaught) {
		g->Translate(1,1);
		
		g->DrawImage(mImageLeft);
		g->Translate(mImageLeft->mSurface->w, 0);
		for (int i = 0; i < middleCount; i++)
		{
			g->DrawImage(mImageMiddle);
			g->Translate(mImageMiddle->mSurface->w, 0);
		}
		g->DrawImage(mImageRight);
		if (mImageLabel) {
			g->MoveTo((mRect.mWidth - mImageLabel->mSurface->w) / 2, (mRect.mHeight - mImageLabel->mSurface->h) / 2);
			g->Translate(1, -1);

			g->SetCubeColor({ 0.0f,0.0f,0.0f,1.0f });
			g->DrawImage(mImageLabel);

			g->SetCubeColor({ 0.0f,1.0f,0.0f,1.0f });
			g->Translate(1, 1);	
			g->DrawImage(mImageLabel);
		}
	}
	else {
		g->DrawImage(mImageLeft);
		g->Translate(mImageLeft->mSurface->w, 0);
		for (int i = 0; i < middleCount; i++)
		{
			g->DrawImage(mImageMiddle);
			g->Translate(mImageMiddle->mSurface->w, 0);
		}
		g->DrawImage(mImageRight);
		if (mImageLabel) {
			
			g->MoveTo((mRect.mWidth - mImageLabel->mSurface->w) / 2, (mRect.mHeight - mImageLabel->mSurface->h) / 2);
			g->Translate(0, -2);

			g->SetCubeColor({ 0.0f,0.0f,0.0f,1.0f });
			g->DrawImage(mImageLabel);
			g->Translate(1, 1);

			if (mIsMouseHover) {
				g->SetCubeColor({ 0.0f,1.0f,0.0f,1.0f });
			}
			else {
				g->SetCubeColor({ 0.0f,0.8f,0.0f,1.0f });
			}
			g->DrawImage(mImageLabel);
		}
		
	}
	
	
}

void Lawn::LawnStoneButton::Update()
{

}


Lawn::TextButton::TextButton(int buttonId, int fontSize, sgf::GameApp* app) :sgf::TemplateButton(buttonId)
{
	mFontSize = fontSize;
	mApp = app;
}

Lawn::TextButton::~TextButton()
{
	if (mTextImage)
		delete mTextImage;
}

void Lawn::TextButton::LoadLabel(const sgf::String& text, const sgf::String& key)
{
	mLabel = text;
	if (mTextImage)
		delete mTextImage;
	sgf::Font* font = (sgf::Font*)mApp->mResourceManager.mResourcePool[key];
	if (!font)
		return;
	font->SetFontSize(mFontSize);
	mTextImage = font->GenTextImage(text);
	Resize(mRect.mX, mRect.mY, mTextImage->mSurface->w, mTextImage->mSurface->h);

}

void Lawn::TextButton::MoveTo(float x, float y)
{
	mRect.mX = x;
	mRect.mY = y;
}

void Lawn::TextButton::Draw(sgf::Graphics* g)
{
	if (mTextImage) {
		g->MoveTo(mFontSize * 0.05,mFontSize * 0.05);
		g->SetCubeColor({ 0,0,0,1.0f });
		g->DrawImage(mTextImage);

		if (mIsMouseHover) {
			g->SetCubeColor({ 1.0f,0.0f,0.0f,1.0f });
			g->MoveTo(0, 0);
			g->DrawImage(mTextImage);
		}
		else {
			g->SetCubeColor({ 1.0f,1.0f,1.0f,1.0f });
			g->MoveTo(0, 0);
			g->DrawImage(mTextImage);
		}

	}
}

void Lawn::TextButton::Update()
{
	if (mIsMouseHover) {
		if (!mIsPlayHoverSound) {
			gLawnApp->mMusicManager.PlayChunk("CHUNK_TAP2",-1);
			mIsPlayHoverSound = true;
		}
	}
	else {
		mIsPlayHoverSound = false;
	}
}

Lawn::LawnDialog::LawnDialog(int theId, sgf::GameApp* app) : Widget(theId)
{
	mApp = app;
	mDialogTopLeft = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_TOPLEFT"];
	mDialogTopMiddle = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_TOPMIDDLE"];
	mDialogTopRight = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_TOPRIGHT"];

	mDialogCenterLeft = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_CENTERLEFT"];
	mDialogCenterMiddle = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_CENTERMIDDLE"];
	mDialogCenterRight = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_CENTERRIGHT"];
	
	mDialogBottomLeft = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_BOTTOMLEFT"];
	mDialogBottomMiddle = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_BOTTOMMIDDLE"];
	mDialogBottomRight = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_BOTTOMRIGHT"];

	mDialogHeader = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_DIALOG_HEADER"];

}

Lawn::LawnDialog::~LawnDialog()
{

}

void Lawn::LawnDialog::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({1.0f,1.0f,1.0f,1.0f});
	sgf::SimpleImage* aBottomLeftImage = mDialogBottomLeft;
	sgf::SimpleImage* aBottomMiddleImage = mDialogBottomMiddle;
	sgf::SimpleImage* aBottomRightImage = mDialogBottomRight;

	int aRepeatX = (mRect.mWidth - mDialogTopRight->mSurface->w - mDialogTopLeft->mSurface->w) / mDialogTopMiddle->mSurface->w;
	int aRepeatY = (mRect.mHeight - mDialogTopLeft->mSurface->h - aBottomLeftImage->mSurface->h - DIALOG_HEADER_OFFSET) / mDialogCenterLeft->mSurface->h;

	int aPosX = 0;
	int aPosY = DIALOG_HEADER_OFFSET;
	g->MoveTo(aPosX, aPosY);
	g->DrawImage(mDialogTopLeft);
	aPosX += mDialogTopLeft->mSurface->w;
	for (int i = 0; i < aRepeatX; i++)
	{
		g->MoveTo(aPosX, aPosY);
		g->DrawImage(mDialogTopMiddle);
		aPosX += mDialogTopMiddle->mSurface->w;
	}
	g->MoveTo(aPosX, aPosY);
	g->DrawImage(mDialogTopRight);

	aPosY += mDialogTopRight->mSurface->h;
	for (int y = 0; y < aRepeatY; y++)
	{
		aPosX = 0;

		g->MoveTo(aPosX, aPosY);
		g->DrawImage(mDialogCenterLeft);
		aPosX += mDialogCenterLeft->mSurface->w;

		for (int x = 0; x < aRepeatX; x++)
		{
			g->MoveTo(aPosX, aPosY);
			g->DrawImage(mDialogCenterMiddle);
			aPosX += mDialogCenterMiddle->mSurface->w;
		}

		g->MoveTo(aPosX, aPosY);
		g->DrawImage(mDialogCenterRight);
		aPosY += mDialogCenterLeft->mSurface->h;
	}

	aPosX = 0;
	g->MoveTo(aPosX, aPosY);
	g->DrawImage(aBottomLeftImage);
	aPosX += aBottomLeftImage->mSurface->w;
	for (int i = 0; i < aRepeatX; i++)
	{
		g->MoveTo(aPosX, aPosY);
		g->DrawImage(aBottomMiddleImage);
		aPosX += aBottomMiddleImage->mSurface->w;
	}

	g->MoveTo(aPosX, aPosY);
	g->DrawImage(aBottomRightImage);
	g->MoveTo((mRect.mWidth - mDialogHeader->mSurface->w) / 2 - 5, 0);
	g->DrawImage(mDialogHeader);
}

void Lawn::LawnDialog::Update()
{

}

Lawn::LawnSlider::LawnSlider(int theId, sgf::GameApp* app) : sgf::Widget(theId)
{
	mRangeMax = 0;
	mRangeMin = 0;
	mRangeDistence = 0;
	mValue = 0;
	mApp = app;


	mImageSliderBar = (sgf::SimpleImage*)app->mResourceManager.mResourcePool["IMAGE_OPTIONS_SLIDERSLOT"];
	mImageSliderButton = (sgf::SimpleImage*)app->mResourceManager.mResourcePool["IMAGE_OPTIONS_SLIDERKNOB2"];

	Resize(0, 0, mImageSliderBar->mSurface->w, 50);

}

Lawn::LawnSlider::~LawnSlider()
{

}

void Lawn::LawnSlider::SetRange(float max, float min)
{
	if (max < min)
		return;
	mRangeMax = max;
	mRangeMin = min;
	mRangeDistence = max - min;
	mValue = mRangeMin;
}

void Lawn::LawnSlider::BindToFloat(float* target)
{
	mTargetFloat = target;
	SetValue(*target);
}

void Lawn::LawnSlider::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({1.0f,1.0f,1.0f,1.0f});
	g->DrawImage(mImageSliderBar);
	g->MoveTo((mValue - mRangeMin) / mRangeDistence * mImageSliderBar->mSurface->w * 0.85, 0);
	g->DrawImage(mImageSliderButton);
}

void Lawn::LawnSlider::Update()
{
	auto pos = GetExactPosition();
	if (mIsMouseCaught) {
		mValue = (static_cast<float>(mApp->mMouseX) - pos.first) / mImageSliderBar->mSurface->w * mRangeDistence + mRangeMin;
		if (mValue > mRangeMax)
			mValue = mRangeMax;
		if (mValue < mRangeMin)
			mValue = mRangeMin;
		if (mTargetFloat)
			*mTargetFloat = mValue;
		if (mListener)
			mListener->OnChange(mId);
	}
}

Lawn::LawnLabel::LawnLabel(sgf::GameApp* app) :sgf::Widget(20000)
{
	mApp = app;
	mFont = (sgf::Font*)mApp->mResourceManager.mResourcePool["FONT_FONT2"];
}

Lawn::LawnLabel::~LawnLabel()
{
	if (mLabelImage)
		delete mLabelImage;
}

void Lawn::LawnLabel::LoadLabel(const sgf::String& label, int fontSize)
{
	if (mFont) {
		mFont->SetFontSize(fontSize);
		mLabelImage = mFont->GenTextImage(label);
	}
}

void Lawn::LawnLabel::SetColor(const sgf::Color& srcColor)
{
	mTextColor = srcColor;
}

void Lawn::LawnLabel::Draw(sgf::Graphics* g)
{
	if (mLabelImage) {
		g->Translate(-1,-1);
		g->SetCubeColor({0,0,0,1});
		g->DrawImage(mLabelImage);
		g->Translate(1, 1);
		g->SetCubeColor(mTextColor);
		g->DrawImage(mLabelImage);
	}
}
