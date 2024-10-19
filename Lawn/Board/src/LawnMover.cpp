#include "../include/LawnMover.h"
#include "../../Lawn/include/LawnApp.h"

Lawn::LawnMover::LawnMover()
{

}

Lawn::LawnMover::~LawnMover()
{
	delete mLawnMoverAnim;
}

void Lawn::LawnMover::Init(LawnMoverType type)
{
	mType = type;
	switch (mType)
	{
	case Lawn::LAWN_MOVER_NORMAL:
		mLawnMoverAnim = new sgf::Animator(RES_RAXML::RAXML_LAWNMOWER);
		mLawnMoverShadow = RES_IMAGE::IMAGE_PLANTSHADOW;
		break;
	default:
		break;
	}
	mBox = {0,0,80,70};

}

Lawn::Zombie* Lawn::LawnMover::TryToFindZombie()
{
	Lawn::Zombie* zom = nullptr;
	for (auto&  x : gLawnApp->mBoard->mZombieVector)
	{
		if (!x->mIsLive || !x->mHasHead || !x->mAvailable)
			continue;
		if (mBox.IsOverlap(x->GetHitBox())) {
			zom = x;
			break;
		}
	}
	return zom;
}

void Lawn::LawnMover::Update()
{
	GameObject::Update();
	if (mIsRunning)
	{
		mLawnMoverAnim->Update();
		mBox.mX += 250.0f * mTick.GetDeltaTickFloat() / 1000.0f;

		for (auto& x : gLawnApp->mBoard->mZombieVector)
		{
			if (!x->mIsLive || !x->mHasHead || !x->mAvailable)
				continue;
			if (mBox.IsOverlap(x->GetHitBox())) {
				x->TakeDamage(ZOMBIE_DAMAGE_NORMAL,10000000);
			}
		}

		if (mBox.mX > LAWN_GAME_WINDOW_WIDTH)
			mAvailable = false;
	}
	else {
		Zombie* zom = TryToFindZombie();
		if (zom) {
			mIsRunning = true;
		}
	}
}

void Lawn::LawnMover::Draw(sgf::Graphics* g)
{
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 1,1,1,1 });
		g->FillRect(mBox.mWidth, mBox.mHeight);
	}

	g->SetCubeColor({ 1,1,1,1 });
	if (mLawnMoverShadow)
	{
		g->Translate(10,40);
		g->DrawImage(mLawnMoverShadow);
	}
	g->MoveTo(0,0);
	mLawnMoverAnim->Present(g);

	
}
