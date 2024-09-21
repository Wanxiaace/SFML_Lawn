#include "../include/LawnCursor.h"
#include "../Board/include/SeedPack.h"


Lawn::LawnCursor::LawnCursor()
{
	mSeed = SEED_NONE;
	mState = LAWN_CURSOR_NONE;
}

Lawn::LawnCursor::~LawnCursor()
{

}

void Lawn::LawnCursor::PickSeedPack(SeedPack* pack)
{
	gLawnApp->mMusicManager.PlayChunk("CHUNK_SEEDLIFT");
	mState = LAWN_CURSOR_SEED;
	if (mSeedPack)
		mSeedPack->mIsChose = false;

	mSeedPack = pack;
	mSeed = pack->mSeedType;
	pack->mIsChose = true;
}

void Lawn::LawnCursor::Reset()
{
	mState = LAWN_CURSOR_NONE;

	if (mIsOnBoard && mSeedPack)
		mSeedPack->mIsChose = false;
	mSeedPack = nullptr;
}

void Lawn::LawnCursor::MouseRightOn()
{
	Reset();
}

void Lawn::LawnCursor::SeedPlant()
{

	int gridX = mBoard->PointXtoGridX(mApp->mMouseX);
	int gridY = mBoard->PointYtoGridY(mApp->mMouseY);

	gLawnApp->mMusicManager.PlayChunk("CHUNK_PLANT");
	mBoard->SpawnPlantAt(gridX, gridY,mSeed);

	Reset();
}

void Lawn::LawnCursor::Update()
{
}

void Lawn::LawnCursor::Draw(sgf::Graphics* g)
{
	switch (mState)
	{
	case Lawn::LAWN_CURSOR_NONE:
		break;
	case Lawn::LAWN_CURSOR_SEED:
	{
		g->ModelMoveTo(0, 0);


		int gridX = mBoard->GridXtoPointX(mBoard->PointXtoGridX(mApp->mMouseX));
		int gridY = mBoard->GridYtoPointY(mBoard->PointYtoGridY(mApp->mMouseY));

		g->MoveTo(gridX, gridY);
		g->SetCubeColor({1,1,1,0.5f});
		DrawSeedPack(mSeed, g, 1.0f);

		g->MoveTo(mApp->mMouseX, mApp->mMouseY);
		g->SetCubeColor({ 1,1,1,1 });
		DrawSeedPack(mSeed, g, 1.0f);

		break;
	}
	default:
		break;
	}
}
