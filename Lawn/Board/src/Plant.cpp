#include "../include/Plant.h"
#include "../include/BoardEnums.h"
#include "../../include/LawnApp.h"

std::unordered_map<Lawn::SeedType, Lawn::PlantDefinition> Lawn::gPlantsDefinitions;

Lawn::Plant::Plant()
{
	mShadowImage = (sgf::SimpleImage*)gLawnApp->mResourceManager.mResourcePool["IMAGE_PLANTSHADOW"];
}

Lawn::Plant::~Plant()
{
}

void Lawn::Plant::PlantInit()
{
	mHealth = 300;
	mDamage = 20;
	mBox.mWidth = 70;
	mBox.mHeight = 80;

	auto& def = gPlantsDefinitions[mSeedType];

	mBodyReanim.Init((sgf::Reanimation*)gLawnApp->mResourceManager.mResourcePool[def.mReanimationName], gLawnApp);
	mBodyReanim.SetFrameRangeByTrackName("anim_idle");
	mBodyReanim.Play();
	mTickCache = sgf::TryGetTicks();

	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
		mShootBox = { mBox.mX,mBox.mY,800,60 };
		mHeadReanimMatrix = glm::translate(glm::mat4x4(1.0f), glm::vec3(-37, -47, 0));
		mHeadReanim.Init((sgf::Reanimation*)gLawnApp->mResourceManager.mResourcePool[def.mReanimationName], gLawnApp);
		mBodyReanim.TrackAttachAnimator("anim_stem", &mHeadReanim);
		mBodyReanim.TrackAttachAnimatorMatrix("anim_stem", &mHeadReanimMatrix);
		mHeadReanim.SetFrameRangeByTrackName("anim_head_idle");
		mHeadReanim.Play();
		mHeadReanim.mSpeed = 1.5f;
		mBodyReanim.mSpeed = 1.5f;
		//mFireCD = 1000;
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
	Zombie* result = nullptr;
	for (auto& x : mBoard->mZombieVector)
	{
		if (mShootBox.IsOverlap(x->mBox)) {
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

void Lawn::Plant::InitPlantsDefinitions()
{
	gPlantsDefinitions[SEED_PEASHOOTER] = PlantDefinition{ SEED_PEASHOOTER ,"RAXML_PEASHOOTERSINGLE","PeaShooter","a useless plant",100,20 };
}

void Lawn::Plant::Update()
{
	mBodyReanim.Update();
	mHeadReanim.Update();

	unsigned int tick = sgf::TryGetTicks();
	int tickDelta = tick - mTickCache;
	mTickCache = tick;

	switch (mSeedType)
	{
	case Lawn::SEED_PEASHOOTER:
	{
		if (mCanShoot && IsOnBoard()) {
			mTargetZombie = TryToFindTarget();
			if (mTargetZombie) {
				if (mState == STATE_NOTREADY) {
					mHeadReanim.SetFrameRangeByTrackName("anim_shooting");
				}
				mState = STATE_READY;
			}
			else {
				if (mState == STATE_READY) {
					mHeadReanim.SetFrameRangeByTrackName("anim_head_idle");
				}
				mState = STATE_NOTREADY;
			}

			if (mState == STATE_READY) {
				if (int(mHeadReanim.mFrameIndexNow) == 65) {
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
		g->Translate(-2,mBox.mHeight-25);
		g->DrawImage(mShadowImage);
	}
	g->MoveTo(0,0);
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
