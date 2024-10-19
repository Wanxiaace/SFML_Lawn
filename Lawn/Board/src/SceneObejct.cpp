#include "../include/SceneObejct.h"
#include "../../include/Resources.h"

Lawn::SceneObejct::SceneObejct()
{

}

Lawn::SceneObejct::~SceneObejct()
{
	if (mReanim)
		delete mReanim;
}

void Lawn::SceneObejct::Init()
{
	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		mReanim = new Animator(RES_RAXML::RAXML_SUN);
		mReanim->mFrameIndexEnd = RES_RAXML::RAXML_SUN->mTracks->at(0).mFrames.size();
		//mReanim->mSpeed = 0.7f;
		mReanim->Play();
		break;
	default:
		break;
	}
}

void Lawn::SceneObejct::Update()
{
	GameObject::Update();
	switch (mMotionType)
	{
	case MOTION_NORMAL: 
	{
		break;
	}
	case MOTION_FALL:
	{
		mBox.mY += mTick.GetDeltaTickFloat() / 1000.0f * 50.0f;
		break;
	}
	}

	if (mReanim)
		mReanim->Update();
}

void Lawn::SceneObejct::Draw(sgf::Graphics* g)
{
	mReanim->Present(g);
}
