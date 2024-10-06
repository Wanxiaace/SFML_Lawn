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
		mLawnMoverAnim = new sgf::Animator(gLawnApp->mResourceManager.GetResourceFast<sgf::Reanimation>("RAXML_LAWNMOWER"),gLawnApp);
		mLawnMoverShadow = gLawnApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_PLANTSHADOW");
		break;
	default:
		break;
	}
}

void Lawn::LawnMover::Update()
{
	if (mIsRunning)
		mLawnMoverAnim->Update();
}

void Lawn::LawnMover::Draw(sgf::Graphics* g)
{
	if (mLawnMoverShadow)
	{
		g->Translate(10,40);
		g->DrawImage(mLawnMoverShadow);
	}
	g->MoveTo(0,0);
	mLawnMoverAnim->Present(g);
}
