#include "SeedPack.h"
#include "../Constant.h"

void Lawn::DrawSeedPack(SeedType type, sgf::Graphics* g, float scaleF)
{
	scaleF *= 0.4f;
	if (type == SEED_NONE) {
		DrawSeedBackgound(SEEDPACK_NONE, g, scaleF);
	}
	else {
		PlantDefinition& def = gPlantsDefinitions[type];
		SeedPackBackground bkg = GetSeedPackBackground(type);
		DrawSeedBackgound(bkg, g, scaleF);
		g->Translate(-2 * scaleF, -2 * scaleF);
		g->DrawImageGridAtlasScaleF(RES_IMAGE::IMAGE_PACKET_PLANTS_PLUS, 1, 56, 0, type, scaleF * 2.7, scaleF * 2.7);
	}
	
	
}


void Lawn::DrawSeedBackgound(SeedPackBackground type, sgf::Graphics* g, float scaleF)
{
	switch (type)
	{
	case Lawn::SEEDPACK_NORMAL:
		g->DrawImageGridAtlasScaleF(RES_IMAGE::IMAGE_SEEDPACKETS, 1, 5, 0, 2,scaleF,scaleF);
		break;
	case SEEDPACK_NONE:
		g->DrawImageGridAtlasScaleF(RES_IMAGE::IMAGE_SEEDPACKETS, 1, 5, 0, 1, scaleF, scaleF);
		break;
	default:
		break;
	}
}

Lawn::SeedPackBackground Lawn::GetSeedPackBackground(SeedType type)
{
	SeedPackBackground res = SEEDPACK_NORMAL;

	switch (type)
	{
	default:
		break;
	}

	return res;
}

Lawn::SeedPack::SeedPack()
{
	//Init(SEED_PEASHOOTER);
}

Lawn::SeedPack::~SeedPack()
{
	
}

void Lawn::SeedPack::Init(SeedType type,Board* board)
{
	mBoard = board;
	mSeedType = type;
	mRect.mWidth = 100 * mScaleF;
	mRect.mHeight = 55 * mScaleF;

	if (mSeedType == SEED_NONE) {
		mCost = 0;
	}
	else {
		PlantDefinition& def = gPlantsDefinitions[mSeedType];
		sgf::Font* font = RES_FONT::FONT_FONT3;
		mCost = def.mCost;
		mTextImage = font->GenTextImage(std::to_string(mCost));
	}

}

void Lawn::SeedPack::Draw(sgf::Graphics* g)
{
	if (mIsChose)
	{
		g->SetCubeColor({ 0.7, 0.7, 0.7, 1 });
		DrawSeedPack(mSeedType, g, mScaleF);
	}
	else
	{
		g->SetCubeColor({ 1, 1, 1, 1 });
		DrawSeedPack(mSeedType, g, mScaleF);
	}

	g->Translate(60 * mScaleF, 35 * mScaleF);
	if (mTextImage) {
		g->SetCubeColor({0,0,0,1});
		g->DrawImageScaleF(mTextImage, mScaleF * 0.7, mScaleF * 0.7);

		g->Translate(-1* mScaleF,-1* mScaleF);
		g->SetCubeColor({ 1,1,1,1 });
		g->DrawImageScaleF(mTextImage, mScaleF * 0.7, mScaleF * 0.7);
	}

	if (mIsMouseHover) {
		g->MoveTo(mRect.mX, mRect.mY);
		g->Translate(-3 * mScaleF, -3 * mScaleF);
		g->DrawImageScaleF(RES_IMAGE::IMAGE_SEEDCHOOSE, 0.4f, 0.4f);
	}

}

void Lawn::SeedPack::Update()
{
	if (!mParent) {
		if (mRect.IsPointIn(gLawnApp->mMouseX, gLawnApp->mMouseY)) {
			mIsMouseHover = true;
		}
		else {
			mIsMouseHover = false;
		}
	}
	else {
		auto x = mParent->GetExactPosition();
		if (mRect.IsPointIn(gLawnApp->mMouseX - x.first, gLawnApp->mMouseY - x.second)) {
			mIsMouseHover = true;
		}
		else {
			mIsMouseHover = false;
		}
	}

}

void Lawn::SeedPack::OnClick()
{
	if (mIsChose)
	{
		if (gLawnApp->mCursor->mSeedPack == this)
		{
			mIsChose = false;
			gLawnApp->mCursor->Reset();
		}
		else
			gLawnApp->mCursor->PickSeedPack(this);
	}
	else
	{
		gLawnApp->mCursor->PickSeedPack(this);
	}
}

Lawn::SeedBank::SeedBank(Board* board) : sgf::Widget(LAWN_SEED_BANK)
{
	mBoard = board;
	Resize(0, 60, 100, 500);
}

Lawn::SeedBank::~SeedBank()
{

}

void Lawn::SeedBank::AppendSeedPack(SeedType type)
{
	SeedPack pack;
	pack.Init(type,mBoard);
	pack.mRect.mY = mSeedPacks.size() * 60;
	pack.mParent = this;


	mSeedPacks.push_back(pack);
}

void Lawn::SeedBank::ClearSeedPack(SeedType type)
{
	mSeedPacks.clear();
}

void Lawn::SeedBank::ClickOn()
{
	for (auto& x : mSeedPacks)
	{
		if (x.mIsMouseHover)
			x.OnClick();
	}
}

Lawn::SeedPack& Lawn::SeedBank::operator[](int index)
{
	return mSeedPacks.at(index);
}

void Lawn::SeedBank::Draw(sgf::Graphics* g)
{
	g->MoveTo(0,0);
	g->SetCubeColor({1,1,1,1});
	int len = mSeedPacks.size();

	for (int i = 0;i< len;i++)
	{
		g->MoveTo(mSeedPacks[i].mRect.mX, mSeedPacks[i].mRect.mY);
		mSeedPacks[i].Draw(g);
	}
	g->SetCubeColor({ 1,1,1,1 });

	for (size_t i = len; i < BOARD_SEED_BAND_SIZE_MAX; i++)
	{
		
		g->MoveTo(0, 60*i);
		DrawSeedBackgound(SEEDPACK_NONE, g, 0.4f);
	}

}

void Lawn::SeedBank::Update()
{
	int len = mSeedPacks.size();
	for (int i = 0; i < len; i++)
	{
		mSeedPacks[i].Update();
	}
}
