#include "SeedChooseScreen.h"
#include "SeedPack.h"
#include "../Constant.h"
#include "../Resources.h"

Lawn::SeedChooseScreen::SeedChooseScreen()
	:Widget(LAWN_WIDGET_SEED_CHOOSE_SCREEN)
{
	Resize(0,0,480,500);
	MoveToCenter(LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);
	Translate(0, 40);
	Translate(0, 800);

	for (int i = SEED_PEASHOOTER; i < NUM_SEED_TYPES; i++)
	{
		AddSeedView((SeedType)i,false);
	}
}

Lawn::SeedChooseScreen::~SeedChooseScreen()
{

}

void Lawn::SeedChooseScreen::ShowScreen()
{
	mHolder.SetSpeed(-500.0f);
	mHolder.BindSpot(&mRect.mY, mRect.mY, mRect.mY - 800,CURVE_EASE_IN_OUT);
	mHolder.Start();
}

void Lawn::SeedChooseScreen::AddSeedView(SeedType type, bool autoAddToManager)
{
	mSeedViewArray.push_back(new SeedView(type));
	SeedView* seed = mSeedViewArray.back();
	seed->BindScreen(this);

	float posY = (mSeedViewIndexCounter / 4) * 55 + 100;

	seed->Resize(
		(mSeedViewIndexCounter % 4) * 105 + 28, 
		posY + 100, 96, 55);
	seed->SetOriginY(posY);
	seed->AttachToListener(this);

	AppendChild(seed);
	
	if (autoAddToManager)
		gLawnApp->SafeAppendWidget(seed);

	mSeedViewIndexCounter++;
}

void Lawn::SeedChooseScreen::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({ 1,1,1,1 });
	g->DrawImage(RES_IMAGE::IMAGE_SEEDCHOOSER_BACKGROUND);

	int ctr = 0;
	for (auto& x :mSeeds)
	{
		g->MoveTo(-100, ctr * 55 - 5);
		DrawSeedPack(x,g,1.0f);

		ctr++;
	}
}

void Lawn::SeedChooseScreen::Update()
{
	mHolder.Update();

	if(gLawnApp->GetMouseWheelY())
		mSeedViewOffsetYChangeSpeed = gLawnApp->GetMouseWheelY();

	mSeedViewOffsetY += mSeedViewOffsetYChangeSpeed * mTick.GetDeltaTickFloat() / 4.0f;
	
	float edgeYTop = (-55 - (NUM_SEED_TYPES / 4 + 1) * 55 + 300);
	float edgeYBottom = -55;
	
	if (mSeedViewOffsetY < edgeYTop)
		mSeedViewOffsetY = edgeYTop;

	if (mSeedViewOffsetY > edgeYBottom)
		mSeedViewOffsetY = edgeYBottom;

	if (mSeedViewOffsetYChangeSpeed != 0) {
		if (abs(mTick.GetDeltaTickFloat() / 1000.0f) < abs(mSeedViewOffsetYChangeSpeed))
			if (mSeedViewOffsetYChangeSpeed > 0)
				mSeedViewOffsetYChangeSpeed -= mTick.GetDeltaTickFloat() / 1000.0f;
			else
				mSeedViewOffsetYChangeSpeed += mTick.GetDeltaTickFloat() / 1000.0f;
		else
			mSeedViewOffsetYChangeSpeed = 0;
	}

	for (auto& x : mSeedViewArray)
	{
		x->mRect.mY = x->mOriY + mSeedViewOffsetY;
	}
}

void Lawn::SeedChooseScreen::OnClick(int widgetId)
{
	if ((widgetId - 0x10000) >= 0 && (widgetId - 0x10000) < 500) {
		auto widgetc = mWidgetManager->mWidgetMap[widgetId];
		SeedView* card = dynamic_cast<SeedView*>(widgetc);
		card->ChooseSeed();
	}
}

Lawn::SeedView::SeedView(SeedType type):
	Widget(int(type + 0x10000))
{
	mSeedType = type;
}

Lawn::SeedView::~SeedView()
{

}

void Lawn::SeedView::SetOriginY(float y)
{
	mOriY = y;
}

void Lawn::SeedView::BindScreen(SeedChooseScreen* ptr)
{
	mScreen = ptr;
}

void Lawn::SeedView::ChooseSeed()
{
	if (mIsChosen)
	{
		mScreen->mSeedsNumber -= 1;
		mScreen->mSeeds.erase(
			std::find(mScreen->mSeeds.begin(), mScreen->mSeeds.end(), mSeedType));
		mIsChosen = false;
	}
	else if(mScreen->mSeedsNumber <= BOARD_SEED_BAND_SIZE_MAX)
	{
		mScreen->mSeedsNumber += 1;
		mScreen->mSeeds.push_back(mSeedType);
		mIsChosen = true;
	}

}

void Lawn::SeedView::Draw(sgf::Graphics* g)
{
	g->ModelMoveTo(mRect.mX, mRect.mY - 35);
	if (mScreen) {
		std::pair<int,int> p = mScreen->GetExactPosition();
		g->SetClipRect({ (float)p.first,(float)p.second + 35,600,422 });
	}
	if(mIsMouseHover)
		g->SetCubeColor({ 0.7f,0.7f,0.7f,1.0f });
	else
		g->SetCubeColor({ 1,1,1,1 });

	if(mIsChosen)
		g->SetCubeColor({ 0.5f,0.5f,0.5f,1.0f });
	DrawSeedPack(mSeedType,g,1.0f);

	if (mScreen)
	{
		g->ClearClipRect();
	}
}

void Lawn::SeedView::Update()
{

}
