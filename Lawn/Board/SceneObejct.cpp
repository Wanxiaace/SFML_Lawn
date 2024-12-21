#include "SceneObejct.h"
#include "../Resources.h"

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
	mOriY = mBox.mY;
	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		mReanim = new Animator(RES_RAXML::RAXML_SUN);
		mBox.mWidth = 80;
		mBox.mHeight = 80;
		//mReanim->mFrameIndexEnd = RES_RAXML::RAXML_SUN->mTracks->at(0).mFrames.size();
		//mReanim->mSpeed = 0.7f;
		mReanim->Play();
		break;
	default:
		break;
	}
}

void Lawn::SceneObejct::Collect()
{
	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		mTargetX = 0;
		mTargetY = 0;
		break;
	default:
		break;
	}

	mMotionType = MOTION_MOVE_TO;
}

void Lawn::SceneObejct::Update()
{
	if (mIsCollect) {
		mIsMouseHover = false;

	}
	else {
		if (mBox.IsPointIn(gLawnApp->mMouseX, gLawnApp->mMouseY)) {
			mIsMouseHover = true;
			if (gLawnApp->mIsMouseLeftDown)
				Collect();
		}
		else {
			mIsMouseHover = false;
		}
	}
	

	GameObject::Update();
	switch (mMotionType)
	{
	case MOTION_NORMAL: 
	{
		break;
	}
	case MOTION_FALL:
	{
		if (mBox.mY <= mOriY)
		{
			mBox.mY += mTick.GetDeltaTickFloat() / 1000.0f * mVelZ;
			mVelZ += 200.0f * mTick.GetDeltaTickFloat() / 1000.0f;
		}
		else {
			mMotionType = MOTION_NORMAL;
		}
		break;
	}
	}

	if (mReanim)
		mReanim->Update();
}

void Lawn::SceneObejct::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({ 1,1,1,1 });

	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		g->MoveTo(40, 40);
		break;
	default:
		break;
	}
	if (mIsMouseHover)
	{
		g->SetCubeColor({ 1,0,0,1 });
	}
	mReanim->Present(g);

}
