#include "../include/Zombie.h"
#include "../../include/LawnApp.h"


std::unordered_map<Lawn::ZombieType, Lawn::ZombieDefinition> Lawn::gZombiesDefinitions;

Lawn::Zombie::Zombie()
{
	mShadowImage = RES_IMAGE::IMAGE_PLANTSHADOW;
}

void Lawn::Zombie::Init()
{
	//mTickCache = sgf::TryGetTicks();
	auto& def = gZombiesDefinitions[mZombieType];
	mBodyReanim.Init((sgf::Reanimation*)gLawnApp->mResourceManager.mResourcePool[def.mReanimationName]);
	PlayTrack("anim_walk");
	mBodyReanim.Play();
	mHealth = 270;
	mHealthMax = 270;
	mBox.mWidth = 40;
	mBox.mHeight = 90;
	mReanimOffsetX = def.mReanimOffsetX;
	mReanimOffsetY = def.mReanimOffsetY;
	mSpeedMin = def.mSpeedMin;
	mSpeedMax = def.mSpeedMax;
	ResetZomSpeed();

	if (mZombieType == ZOMBIE_NORMAL ||
		mZombieType == ZOMBIE_TRAFFIC_CONE ||
		mZombieType == ZOMBIE_FLAG ||
		mZombieType == ZOMBIE_DOOR ||
		mZombieType == ZOMBIE_PAIL)
		InitNormalZombieReanim();


	switch (mZombieType)
	{
	case Lawn::ZOMBIE_NORMAL:
		break;
	case Lawn::ZOMBIE_TRAFFIC_CONE:
		mBodyReanim.SetTrackVisibleByTrackName("anim_cone", true);
		mHelmType = HELMTYPE_TRAFFIC_CONE;
		mHelmHealthMax = 370;
		break;
	case Lawn::ZOMBIE_PAIL:
		mBodyReanim.SetTrackVisibleByTrackName("anim_bucket", true);
		mHelmType = HELMTYPE_PAIL;
		mHelmHealthMax = 1100;
		break;
	case Lawn::ZOMBIE_DOOR:
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_upper", false);
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_lower", false);
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_hand", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm1", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm2", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm3", false);
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_screendoor", true);
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor_hand", true);
		mBodyReanim.SetTrackVisibleByTrackName("anim_screendoor", true);
		mScreenDoorLayerIndex = mBodyReanim.GetFirstTrackExtraIndexByName("anim_screendoor");
		mShieldType = SHIELDTYPE_DOOR;
		mShieldHealthMax = 1100;
		break;
	default:
		break;
	}

	mHelmHealth = mHelmHealthMax;
	mShieldHealth = mShieldHealthMax;
}

void Lawn::Zombie::InitNormalZombieReanim()
{
	mBodyReanim.SetTrackVisibleByTrackName("anim_cone",false);
	mBodyReanim.SetTrackVisibleByTrackName("anim_bucket",false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_screendoor",false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor_hand",false);
	mBodyReanim.SetTrackVisibleByTrackName("anim_screendoor",false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_duckytube",false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_flaghand",false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor",false);
	mBodyReanim.SetTrackVisibleByTrackName("anim_tongue", false);
	mBodyReanim.SetTrackVisibleByTrackName("Zombie_mustache", false);

	int randomInteger = sgf::Rand(0,4);
	if (!randomInteger) {
		mBodyReanim.SetTrackVisibleByTrackName("anim_tongue", true);
		mSpeedMax += 0.2f;
		mSpeedMin += 0.2f;
		ResetZomSpeed();
	}

	randomInteger = sgf::Rand(0, 2);
	if (randomInteger) {
		PlayTrack("anim_walk2");
	}
}

void Lawn::Zombie::ResetZomSpeed()
{
	float speed = sgf::RandF(mSpeedMin,mSpeedMax);
	mBodyReanim.mSpeed = speed;
}

void Lawn::Zombie::DieNoLoot()
{
	mIsLive = false;
	mAvailable = false;
}

void Lawn::Zombie::TakeDamage(ZombieDamageType damageType, int damage)
{
	//mHealth -= damage;

	switch (damageType)
	{
	case Lawn::ZOMBIE_DAMAGE_NORMAL: 
	{
		
		if (mHelmType != HELMTYPE_NONE && mHelmHealth > 0) {
			if (mHelmHealth > damage)
			{
				mHelmHealth -= damage;
				damage = 0;
			}
			else {
				damage -= mHelmHealth;
				mHelmHealth = 0;
			}
		}

		if (mShieldType != SHIELDTYPE_NONE && mShieldHealth > 0) {
			if (mShieldHealth > damage)
			{
				mShieldHealth -= damage;
				damage = 0;
			}
			else {
				damage -= mShieldHealth;
				mShieldHealth = 0;
			}
			mScreenDoorShakeCounter = 300.0f;
		}
		else {
			mFlashCounter = 300.0f;
		}
		
		mHealth -= damage;
		break;
	}
	}

	CheckIsHelmDie();
	CheckIsShieldDie();
	CheckIsDie();
}

void Lawn::Zombie::CheckIsDie()
{
	if (mHasArm) {
		if (mHealth <= mHealthMax * 0.7 * 0.8) {
			DropArm();
		}
	}

	if (mHasHead) {
		if (mHealth <= mHealthMax * 0.3) {
			DropHead();
		}
	}
}

void Lawn::Zombie::CheckIsHelmDie()
{
	switch (mHelmType)
	{
	case Lawn::HELMTYPE_TRAFFIC_CONE:
	{
		if ((mHelmHealth / mHelmHealthMax) <= 0.0f) {
			mBodyReanim.SetTrackVisibleByTrackName("anim_cone", false);
			mHelmType = HELMTYPE_NONE;
			sgf::Point dropPoint = mBodyReanim.GetTrackPos("anim_cone");
			sgf::Particle* cone = mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMCONEDROP"),
				dropPoint.x + mBox.mX + mReanimOffsetX, dropPoint.y + mBox.mY + mReanimOffsetY + 70, -80);
		}
		else if (mHelmHealth / mHelmHealthMax < 0.3f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_cone",RES_IMAGE::IMAGE_REANIM_ZOMBIE_CONE3);
		}
		else if (mHelmHealth / mHelmHealthMax < 0.7f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_cone", RES_IMAGE::IMAGE_REANIM_ZOMBIE_CONE2);
		}
		break;
	}
	case Lawn::HELMTYPE_PAIL: {
		if ((mHelmHealth / mHelmHealthMax) <= 0.0f) {
			mBodyReanim.SetTrackVisibleByTrackName("anim_bucket", false);
			mHelmType = HELMTYPE_NONE;
			sgf::Point dropPoint = mBodyReanim.GetTrackPos("anim_bucket");
			sgf::Particle* cone = mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMBUCKETDROP"),
				dropPoint.x + mBox.mX + mReanimOffsetX, dropPoint.y + mBox.mY + mReanimOffsetY + 70, -80);
		}
		else if (mHelmHealth / mHelmHealthMax < 0.3f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_bucket", RES_IMAGE::IMAGE_REANIM_ZOMBIE_BUCKET3);
		}
		else if (mHelmHealth / mHelmHealthMax < 0.7f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_bucket", RES_IMAGE::IMAGE_REANIM_ZOMBIE_BUCKET2);
		}
		break;
	}
	}
}

void Lawn::Zombie::CheckIsShieldDie()
{
	switch (mShieldType)
	{
	case Lawn::SHIELDTYPE_DOOR:
	{
		if ((mShieldHealth / mShieldHealthMax) <= 0.0f) {
			mBodyReanim.SetTrackVisibleByTrackName("anim_screendoor", false);
			mShieldType = SHIELDTYPE_NONE;
			sgf::Point dropPoint = mBodyReanim.GetTrackPos("anim_screendoor");
			sgf::Particle* cone = mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMSCREENDOORDROP"),
				dropPoint.x + mBox.mX + mReanimOffsetX, dropPoint.y + mBox.mY + mReanimOffsetY + 70, -80);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_upper", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_lower", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_hand", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm1", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm2", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm3", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_screendoor", false);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor_hand", false);
		}
		else if (mShieldHealth / mShieldHealthMax < 0.3f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_screendoor", RES_IMAGE::IMAGE_REANIM_ZOMBIE_SCREENDOOR3);
		}
		else if (mShieldHealth / mShieldHealthMax < 0.7f) {
			mBodyReanim.TrackAttachImageByTrackName("anim_screendoor", RES_IMAGE::IMAGE_REANIM_ZOMBIE_SCREENDOOR2);
		}
		break;
	}
	}
}

void Lawn::Zombie::DropArm()
{
	mHasArm = false;
	switch (mZombieType)
	{
	case Lawn::ZOMBIE_TRAFFIC_CONE:
	case Lawn::ZOMBIE_PAIL:
	case Lawn::ZOMBIE_DOOR:
	case Lawn::ZOMBIE_NORMAL: {
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_hand", false);
		mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_lower", false);
		mBodyReanim.TrackAttachImageByTrackName("Zombie_outerarm_upper", gLawnApp->mResourceManager.GetResource<sgf::SimpleImage>("IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2"));
		sgf::Point dropPoint = mBodyReanim.GetTrackPos("Zombie_outerarm_upper");
		mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMARMDROP"), dropPoint.x + mBox.mX + mReanimOffsetX - 40, dropPoint.y + mBox.mY + mReanimOffsetY + 60, -40);
		break;
	}
	}
}

void Lawn::Zombie::DropHead()
{
	mHasHead = false;
	switch (mZombieType)
	{
	case Lawn::ZOMBIE_TRAFFIC_CONE:
	case Lawn::ZOMBIE_PAIL:
	case Lawn::ZOMBIE_DOOR:
	case Lawn::ZOMBIE_NORMAL: {
		mBodyReanim.SetTrackVisibleByTrackName("anim_head1", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_head2", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_hair", false);
		mBodyReanim.SetTrackVisibleByTrackName("anim_tongue", false);
		sgf::Point dropPoint = mBodyReanim.GetTrackPos("anim_head1");
		sgf::Particle* head = mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMHEADDROP"), dropPoint.x + mBox.mX + mReanimOffsetX, dropPoint.y + mBox.mY + mReanimOffsetY + 70, -80);
		break;
	}
	}
}

void Lawn::Zombie::DoDeathReanim()
{
	if (!sgf::Rand(0, 2))
		PlayTrack("anim_death", 0);
	else {
		PlayTrack("anim_death2", 200);
	}
	
	mBodyReanim.Play(sgf::Animator::PlayState::PLAY_ONCE);
}

void Lawn::Zombie::PlayTrack(const sgf::String& trackName,int blendTime)
{
	mBodyReanim.mFrameIndexBlendBuffer = mBodyReanim.mFrameIndexNow;

	mBodyReanim.SetFrameRangeByTrackName(trackName);
	mBodyReanim.mReanimBlendCounterMax = blendTime;
	mBodyReanim.mReanimBlendCounter = blendTime;
}

void Lawn::Zombie::UpdateEating(Plant* target)
{
	if (!mIsEating) {
		PlayTrack("anim_eat");

		if (mShieldType == SHIELDTYPE_DOOR) {
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_upper", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_lower", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_hand", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm1", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm2", true);
			mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm3", true);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_screendoor", false);
			mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor_hand", false);
		}

		mBodyReanim.mSpeed = 2.5f;
	}
	target->TakeDamage(100.0f * float(mTick.GetDeltaTick()) / 1000.0f);
	if (mEatingChunkCounter < 0) {
		mEatingChunkCounter = 800;
		if (sgf::Rand(0, 2)) {
			gLawnApp->mMusicManager.PlayChunk("CHUNK_CHOMP");
		}
		else {
			gLawnApp->mMusicManager.PlayChunk("CHUNK_CHOMP2");
		}
	}
	else {
		mEatingChunkCounter -= mTick.GetDeltaTick();
	}
}

Lawn::Plant* Lawn::Zombie::FindPlant()
{
	Lawn::Plant* plant = nullptr;
	for (auto& x : mBoard->mPlantVector)
	{
		if (mBox.IsOverlap(x->mBox))
		{
			plant = x;
			break;
		}
	}
	return plant;
}

void Lawn::Zombie::InitZombiesDefinitions()
{
	gZombiesDefinitions[ZOMBIE_NORMAL] = { ZOMBIE_NORMAL,"RAXML_ZOMBIE","NormalZombie","NormalZombie",1.0f,1.4f,100,-20,-40,0};
	gZombiesDefinitions[ZOMBIE_TRAFFIC_CONE] = { ZOMBIE_TRAFFIC_CONE,"RAXML_ZOMBIE","ConeZombie","ConeZombie",1.0f,1.4f,30,-20,-40,1 };
	gZombiesDefinitions[ZOMBIE_PAIL] = { ZOMBIE_PAIL,"RAXML_ZOMBIE","BucketZombie","BucketZombie",1.0f,1.4f,15,-20,-40,2 };
	gZombiesDefinitions[ZOMBIE_DOOR] = { ZOMBIE_DOOR,"RAXML_ZOMBIE","ScreenDoorZombie","ScreenDoorZombie",1.0f,1.4f,15,-20,-40,2 };
}

void Lawn::Zombie::Update()
{
	/*unsigned int tickNow = sgf::TryGetTicks();
	mTickDelta = tickNow - mTickCache;
	mTickCache = tickNow;*/
	GameObject::Update();

	mBodyReanim.Update();
	if (mBodyReanim.GetTrackVisible("_ground")) {
		float speedX = mBodyReanim.GetTrackVelocity("_ground");
		mBox.mX -= speedX * 0.5f * float(mTick.GetDeltaTick()) / mBodyReanim.mDeltaRate * mBodyReanim.mSpeed;
	}

	if (mFlashCounter > mTick.GetDeltaTick()) {
		mFlashCounter -= mTick.GetDeltaTick();
	}
	else {
		mFlashCounter = 0;
	}

	if (mScreenDoorShakeCounter > mTick.GetDeltaTick()) {
		mBodyReanim.TrackAttachOffset(mScreenDoorLayerIndex,
			mScreenDoorShakeCounter / 100.0f, mScreenDoorShakeCounter / 100.0f);
		mBodyReanim.TrackAttachFlashSpot(mScreenDoorLayerIndex,
			mScreenDoorShakeCounter / 300.0f);
		mScreenDoorShakeCounter -= mTick.GetDeltaTick();
	}
	else {
		if (mScreenDoorShakeCounter > 0)
		{
			mBodyReanim.TrackAttachOffset(mScreenDoorLayerIndex, 0, 0);
			mBodyReanim.TrackAttachFlashSpot(mScreenDoorLayerIndex,0);
		}
		mScreenDoorShakeCounter = 0;
	}
	

	if (!mHasHead && mAvailable) {
		if (!mDoDeathReanim) {
			if (mHealth > 0) {
				mHealth -= 0.3;
			}
			else {
				if ((mBodyReanim.mFrameIndexEnd -= mBodyReanim.mFrameIndexNow) <= 1) {
					mDoDeathReanim = true;
					DoDeathReanim();
					mIsLive = false;
				}
			}
		}
		else {
			if ((mBodyReanim.mFrameIndexEnd - mBodyReanim.mFrameIndexNow) < 1.0f) {
				DieNoLoot();
			}
		}
	}
	else {
		Plant* plant = FindPlant();
		if (plant) {
			UpdateEating(plant);
			mIsEating = true;
		}
		else {
			if (mIsEating) {
				ResetZomSpeed();

				if (mShieldType == SHIELDTYPE_DOOR) {
					mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_upper", false);
					mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_lower", false);
					mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_hand", false);
					mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm1", false);
					mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm2", false);
					mBodyReanim.SetTrackVisibleByTrackName("anim_innerarm3", false);
					mBodyReanim.SetTrackVisibleByTrackName("Zombie_outerarm_screendoor", true);
					mBodyReanim.SetTrackVisibleByTrackName("Zombie_innerarm_screendoor_hand", true);
				}

				PlayTrack("anim_walk");
			}
			mIsEating = false;
		}
		if (mBox.mX < 150 && mBoard->mIsBoardRunning) {
			mBoard->ZombieWin(this);
		}
	}
	
}

void Lawn::Zombie::Draw(sgf::Graphics* g)
{
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 1,1,1,1 });
		g->FillRect(mBox.mWidth, mBox.mHeight);
	}
	g->SetCubeColor({ 1,1,1,1 });

	if (mShadowImage)
	{
		g->Translate(-10, mBox.mHeight - 25);
		g->DrawImage(mShadowImage);
	}

	g->SetCubeColor({ 1,1,1,1.0f + mFlashCounter / 300.0f });
	g->MoveTo(0,0);
	g->Translate(mReanimOffsetX, mReanimOffsetY);
	if (mBodyReanim.mReanim)
		mBodyReanim.Present(g);
}

void Lawn::Zombie::ParseToJson(nlohmann::json& dstJson)
{
}
