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
		gLawnApp->mMusicManager.PlayChunk("CHUNK_POINTS");
		break;
	default:
		break;
	}
	mMotionType = MOTION_MOVE_TO;
}

void Lawn::SceneObejct::DoCollected()
{
	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		gLawnApp->mBoard->mSun += 25;
		gLawnApp->mBoard->mSeedBank->SunBankBounce();
		break;
	default:
		break;
	}

	mAvailable = false;
}

void Lawn::SceneObejct::Update()
{
	if (mIsCollect) {
		mIsMouseHover = false;

	}
	else {
		if (mBox.IsPointIn(gLawnApp->mMouseX, gLawnApp->mMouseY)) {
			mIsMouseHover = true;
			gLawnApp->ShowHandCursor();
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
	case MOTION_MOVE_TO:
	{
		float speedX = -(mBox.mX - mTargetX);
		float speedY = -(mBox.mY - mTargetY);
		mBox.mX += speedX * mTick.GetDeltaTickFloat() / 1000.0f * 2;
		mBox.mY += speedY * mTick.GetDeltaTickFloat() / 1000.0f * 2;

		if (abs((mBox.mX - mTargetX)) < 100.0f) {
			mAlpha = abs((mBox.mX - mTargetX)) / 100.0f;

			if (abs((mBox.mX - mTargetX)) < 20)
				DoCollected();
		}
		break;
	}
	}

	if (mReanim)
		mReanim->Update();
}

void Lawn::SceneObejct::Draw(sgf::Graphics* g)
{
	g->SetCubeColor({ 1,1,1,mAlpha });
	
	switch (mType)
	{
	case Lawn::SCENE_OBJECT_SUN:
		g->MoveTo(40, 40);
		break;
	default:
		break;
	}

	glm::mat4x4 matMatrix = glm::scale(glm::mat4x4(1.0f), { mAlpha * 0.5f + 0.5f,mAlpha * 0.5f + 0.5f,1.0f });
	mReanim->PresentMatrix(g, matMatrix);

}
