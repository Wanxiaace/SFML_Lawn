#include "LawnTip.h"
#include "LawnApp.h"

Lawn::ToolTip::ToolTip(int id) : Widget(id)
{

}

Lawn::ToolTip::~ToolTip()
{

	if (mLabelImage)
		delete mLabelImage;
}

void Lawn::ToolTip::Init(const sgf::String& label, unsigned int style, TipLocation location, float lifeTime)
{
	LoadLabel(label);
	mLocation = location;
	mLifeTime = lifeTime;
	InitCurrentLoc();
}

void Lawn::ToolTip::LoadLabel(const sgf::String& label)
{
	if (mLabelImage)
		delete mLabelImage;

	auto font = RES_FONT::FONT_FONT3;
	font->SetFontSize(30);
	mLabelImage = font->GenTextImage(label);
}

void Lawn::ToolTip::InitCurrentLoc()
{

}

void Lawn::ToolTip::Update()
{
	if (mCostLifeTime && mLifeTime > 0)
		if(mLifeTime > mTick.GetDeltaTick())
			mLifeTime -= mTick.GetDeltaTick();
		else
			mLifeTime = 0;
}

void Lawn::ToolTip::Draw(sgf::Graphics* g)
{
	g->SetCubeColor(mColor);
	if (mLabelImage)
		g->DrawImage(mLabelImage);
}
