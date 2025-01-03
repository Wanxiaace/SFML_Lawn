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

	for (int i = SEED_PEASHOOTER; i < gLawnApp->GetPlayerUsableSeedsTotalNum(); i++)
	{
		if(gLawnApp->IsPlayerSeedUsable((SeedType)i))
			AddSeedView((SeedType)i,false);
	}

	mStartButton = new LawnStoneButton(LAWN_WIDGET_START_GAME_BUTTON);
	mStartButton->Resize(165, 455, 165, 50);
	mStartButton->LoadLabel(_LS("LetsRock"));
	mStartButton->AttachToListener(this);
	AppendChild(mStartButton);
}

Lawn::SeedChooseScreen::~SeedChooseScreen()
{

}

void Lawn::SeedChooseScreen::ShowScreen()
{
	mHolder.SetSpeed(-500.0f);
	mHolder.BindSpot(&mRect.mY, mRect.mY, mRect.mY - 800,CURVE_EASE_IN_OUT);
	mHolder.Start();
	gLawnApp->mMusicManager.PlayChunk("CHUNK_ROLL_IN");
	mIsOnChoosing = true;
}

void Lawn::SeedChooseScreen::QuitScreen()
{
	mIsOnChoosing = false;
	mHolder.SetSpeed(500.0f);
	mHolder.BindSpot(&mRect.mY, mRect.mY + 800, mRect.mY, CURVE_EASE_IN_OUT);
	mHolder.Start();

	gLawnApp->mBoard->mSeedBank->ClearSeedPack();
	for (auto& x : mSeeds)
	{
		gLawnApp->mBoard->mSeedBank->AppendSeedPack(x.mType);
	}
	

	mHolder.SetNextFunction([] {
		gLawnApp->mBoard->mCutSenceHolder.SetSpeed(300);
		gLawnApp->mBoard->mCutSenceHolder.BindSpot(
			&gLawnApp->mBoard->mRect.mX, 0, -250, CURVE_EASE_IN_OUT);
		gLawnApp->mBoard->mSeedBank->mVisible = true;
		gLawnApp->mBoard->mCutSenceHolder.SetNextFunction([] {
			gLawnApp->mBoard->EndCutsence();
			});
		});
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

	g->SetCubeColor({ 1,1,1,1 });

	int ctr = 0;
	auto widgetRect = GetExactPosition();

	for (auto& x :mSeeds)
	{
		g->SetCubeColor({ 1,1,1,1 });
		g->MoveTo(-100, ctr * 55 - 5);
		
		FloatRect seedRect = { widgetRect.first - 100,widgetRect.second + ctr * 55 - 5,100,55 };

		bool isCover = seedRect.IsPointIn(gLawnApp->mMouseX, gLawnApp->mMouseY);
		if(isCover)
			g->SetCubeColor({ 0.5f,0.5f,0.5f,1.0f });
		DrawSeedPack(x.mType, g, 1.0f);

		ctr++;
	}

	for (int i = ctr; i < BOARD_SEED_BAND_SIZE_MAX; i++)
	{
		g->SetCubeColor({ 1,1,1,0.6f });
		g->MoveTo(-100, i * 55 - 5);
		DrawSeedPack(SEED_NONE, g, 1.0f);
	}

}

void Lawn::SeedChooseScreen::Update()
{
	mHolder.Update();

	if(gLawnApp->GetMouseWheelY())
		mSeedViewOffsetYChangeSpeed = gLawnApp->GetMouseWheelY();

	mSeedViewOffsetY += mSeedViewOffsetYChangeSpeed * mTick.GetDeltaTickFloat() / 4.0f;
	
	float edgeYTop = (-55 - (gLawnApp->GetPlayerUsableSeedsTotalNum() / 4 + 1) * 55 + 300);
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
		if (x->mRect.mY < -20 || x->mRect.mY > 470)
			x->mVisible = false;
		else
			x->mVisible = true;

		x->Update();
	}


}

void Lawn::SeedChooseScreen::OnClick(int widgetId)
{
	switch (widgetId)
	{
	case LAWN_WIDGET_START_GAME_BUTTON:
	{
		QuitScreen();
		break;
	}
	}

	if ((widgetId - 0x10000) >= 0 && (widgetId - 0x10000) < 500) {
		auto widgetc = mWidgetManager->mWidgetMap[widgetId];
		SeedView* card = dynamic_cast<SeedView*>(widgetc);
		card->ChooseSeed();
		card->Bounce();
		gLawnApp->mMusicManager.PlayChunk("CHUNK_TAP2");
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
			std::find_if(mScreen->mSeeds.begin(), mScreen->mSeeds.end(), 
				[this](const SeedPair& pair)->bool { 
					return pair.mViewPointer == this; }));
		mIsChosen = false;
	}
	else if(mScreen->mSeedsNumber < BOARD_SEED_BAND_SIZE_MAX)
	{
		mScreen->mSeedsNumber += 1;
		mScreen->mSeeds.push_back({ mSeedType ,this });
		mIsChosen = true;
	}
}

void Lawn::SeedView::Bounce()
{
	mBounceScale = 0.8f;
	mBounceHolder.SetSpeed(5);
	mBounceHolder.BindSpot(&mBounceScale, 1.0f, 0.5f);
	mBounceHolder.Start();
}

void Lawn::SeedView::Draw(sgf::Graphics* g)
{
	g->ModelMoveTo(mRect.mX, mRect.mY - 35);
	if (mScreen) {
		std::pair<int,int> p = mScreen->GetExactPosition();
		g->SetClipRect({ (float)p.first,
			(float)(p.second + 35.0f),
			600.0f,
			422.0f });
	}
	if(mIsMouseHover)
		g->SetCubeColor({ 0.7f,0.7f,0.7f,1.0f });
	else
		g->SetCubeColor({ 1,1,1,1 });

	if(mIsChosen)
		g->SetCubeColor({ 0.5f,0.5f,0.5f,1.0f });

	g->Translate(-100.0f * (mBounceScale - 1.0f) / 2.0f,
		-55.0f * (mBounceScale - 1.0f) / 2.0f);
	DrawSeedPack(mSeedType,g,1.0f * mBounceScale);


	if (mScreen)
	{
		g->ClearClipRect();
	}
}

void Lawn::SeedView::Update()
{
	mBounceHolder.Update();
}

