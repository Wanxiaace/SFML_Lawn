#include "../include/Plant.h"
#include "../include/BoardEnums.h"
#include "../../include/LawnApp.h"

std::unordered_map<Lawn::SeedType, Lawn::PlantDefinition> Lawn::gPlantsDefinitions;

Lawn::Plant::Plant()
{
	mShadowImage = RES_IMAGE::IMAGE_PLANTSHADOW;
}

Lawn::Plant::~Plant()
{
}

void Lawn::Plant::PlantInit()
{
	mHealth = 300;
	mDamage = 20;
	mBox.mWidth = 40;
	mBox.mHeight = 80;
	mBox.mX += 20;

	auto& def = gPlantsDefinitions[mSeedType];

	mReanimOffsetX = def.mReanimOffsetX;
	mReanimOffsetY = def.mReanimOffsetY;

	mBodyReanim.Init((sgf::Reanimation*)gLawnApp->mResourceManager.mResourcePool[def.mReanimationName]);
	mBodyReanim.SetFrameRangeByTrackName("anim_idle");
	mBodyReanim.Play();

	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
		mShootBox = { mBox.mX,mBox.mY,800,60 };
		mBodyReanim.SetFrameRangeByTrackName("anim_head_idle");
		mBodyReanim.mSpeed = 1.5f;
		break;
	default:
		break;
	}
}

void Lawn::Plant::Fire()
{
	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
		auto pea = mBoard->SpawnProjectileAt(mBox.mX + 50, mBox.mY + 15, PROJECTILE_PEA);
		pea->mSpeedX = 35;
		gLawnApp->mMusicManager.PlayChunk("CHUNK_THROW2");
		mProduceCountDown = 1000;
		//std::cout << "shit" << std::endl;
		break;
	}
}

bool Lawn::Plant::IsOnBoard() const
{
	return mBoard;
}

Lawn::Zombie* Lawn::Plant::TryToFindTarget()
{
	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
	{
		if (mProduceCountDown > 0)
			return nullptr;
		break;
	}
	}

	Zombie* result = nullptr;
	for (auto& x : mBoard->mZombieVector)
	{
		if (x->mIsLive && mShootBox.IsOverlap(x->mBox)) {
			if (!result)
				result = x;
			else if (result->mBox.mX > x->mBox.mX)
				result = x;
		}
	}
	return result;
}

void Lawn::Plant::Die()
{
	mAvailable = false;
}

void Lawn::Plant::TakeDamage(float damage)
{
	mHealth -= damage;
}

void Lawn::Plant::PlayTrack(const sgf::String& trackName,int blendTime)
{
	mBodyReanim.mFrameIndexBlendBuffer = mBodyReanim.mFrameIndexNow;

	mBodyReanim.SetFrameRangeByTrackName(trackName);
	mBodyReanim.mReanimBlendCounterMax = blendTime;
	mBodyReanim.mReanimBlendCounter = blendTime;
}

void Lawn::Plant::InitPlantsDefinitions()
{
	gPlantsDefinitions[SEED_PEASHOOTER] = PlantDefinition{ SEED_PEASHOOTER ,"RAXML_PEASHOOTERSINGLE","PeaShooter","a useless plant",100,20,-20,0 };
}

void Lawn::Plant::Update()
{
	GameObject::Update();

	mBodyReanim.Update();
	mHeadReanim.Update();
	if (mProduceCountDown > 0)
		mProduceCountDown -= mTick.GetDeltaTick();

	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
	{
		if (mCanShoot && IsOnBoard()) {
			mTargetZombie = TryToFindTarget();
			if (mTargetZombie) {
				if (mState == STATE_NOTREADY && int(mBodyReanim.mFrameIndexEnd - mBodyReanim.mFrameIndexNow - 1) == 0) {
					PlayTrack("anim_shooting");
					mState = STATE_READY;
				}
			}
			else {
				if (mState == STATE_READY) {
					PlayTrack("anim_head_idle",200);
					mState = STATE_NOTREADY;
				}
			}

			if (mState == STATE_READY) {
				if (abs((mBodyReanim.mFrameIndexNow - mBodyReanim.mFrameIndexBegin) / (mBodyReanim.mFrameIndexEnd - mBodyReanim.mFrameIndexBegin) - 0.7f) <= 0.1f) {//65
					if (!mIsFire) {
						Fire();
						mIsFire = true;
					}
				}
				else {
					mIsFire = false;
				}
			}
		}
		break;
	}
	default:
		break;
	}

	if (mHealth < 0)
		mAvailable = false;
}

void Lawn::Plant::Draw(sgf::Graphics* g)
{
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 0,0,1,1 });
		g->FillRect(mBox.mWidth, mBox.mHeight);
	}

	g->SetCubeColor({ 1,1,1,1 });
	if (mShadowImage)
	{
		g->Translate(-2 + mReanimOffsetX, mBox.mHeight - 25 + mReanimOffsetY);
		g->DrawImage(mShadowImage);
	}
	g->MoveTo(mReanimOffsetX, mReanimOffsetY);
	if (mBodyReanim.mReanim)
		mBodyReanim.Present(g);
	
	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
		break;
	default:
		break;
	}
}

void Lawn::Plant::MoveTo(float x, float y)
{
	GameObject::MoveTo(x, y);
	mShootBox.mX = x;
	mShootBox.mY = y;
}
