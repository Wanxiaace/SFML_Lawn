#include "../include/Zombie.h"
#include "../../include/LawnApp.h"


std::unordered_map<Lawn::ZombieType, Lawn::ZombieDefinition> Lawn::gZombiesDefinitions;

Lawn::Zombie::Zombie()
{
	mShadowImage = (sgf::SimpleImage*)gLawnApp->mResourceManager.mResourcePool["IMAGE_PLANTSHADOW"];
}

void Lawn::Zombie::Init()
{
	mTickCache = sgf::TryGetTicks();
	auto& def = gZombiesDefinitions[mZombieType];
	mBodyReanim.Init((sgf::Reanimation*)gLawnApp->mResourceManager.mResourcePool[def.mReanimationName]);
	PlayTrack("anim_walk");
	mBodyReanim.Play();
	mHealth = 270;
	mHealthMax = 270;
	mBox.mWidth = 60;
	mBox.mHeight = 90;
	mReanimOffsetX = def.mReanimOffsetX;
	mReanimOffsetY = def.mReanimOffsetY;
	mSpeedMin = def.mSpeedMin;
	mSpeedMax = def.mSpeedMax;
	ResetZomSpeed();

	if (mZombieType == Lawn::ZOMBIE_NORMAL ||
		mZombieType == Lawn::ZOMBIE_TRAFFIC_CONE ||
		mZombieType == ZOMBIE_FLAG ||
		mZombieType == ZOMBIE_PAIL)
		InitNormalZombieReanim();

	switch (mZombieType)
	{
	case Lawn::ZOMBIE_NORMAL:
		break;
	default:
		break;
	}
}

void Lawn::Zombie::InitNormalZombieReanim()
{
	mBodyReanim.SetTrackVisible("anim_cone",false);
	mBodyReanim.SetTrackVisible("anim_bucket",false);
	mBodyReanim.SetTrackVisible("Zombie_outerarm_screendoor",false);
	mBodyReanim.SetTrackVisible("Zombie_innerarm_screendoor_hand",false);
	mBodyReanim.SetTrackVisible("anim_screendoor",false);
	mBodyReanim.SetTrackVisible("Zombie_duckytube",false);
	mBodyReanim.SetTrackVisible("Zombie_flaghand",false);
	mBodyReanim.SetTrackVisible("Zombie_innerarm_screendoor",false);
	mBodyReanim.SetTrackVisible("anim_tongue", false);
	mBodyReanim.SetTrackVisible("Zombie_mustache", false);

	int randomInteger = sgf::Rand(0,4);
	if (!randomInteger) {
		mBodyReanim.SetTrackVisible("anim_tongue", true);
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
	mAvailable = false;
}

void Lawn::Zombie::TakeDamage(ZombieDamageType damageType, int damage)
{
	//mHealth -= damage;

	switch (damageType)
	{
	case Lawn::ZOMBIE_DAMAGE_NORMAL: 
	{
		mFlashCounter = 300.0f;
		mHealth -= damage;
		break;
	}
	}

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

void Lawn::Zombie::DropArm()
{
	mHasArm = false;
	switch (mZombieType)
	{
	case Lawn::ZOMBIE_TRAFFIC_CONE:
	case Lawn::ZOMBIE_PAIL:
	case Lawn::ZOMBIE_NORMAL: {
		mBodyReanim.SetTrackVisible("Zombie_outerarm_hand", false);
		mBodyReanim.SetTrackVisible("Zombie_outerarm_lower", false);
		mBodyReanim.TrackAttachImage("Zombie_outerarm_upper", gLawnApp->mResourceManager.GetResource<sgf::SimpleImage>("IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2"));
		sgf::Point dropPoint = mBodyReanim.GetTrackPos("Zombie_outerarm_upper");
		mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMARMDROP"), dropPoint.x + mBox.mX - 80, dropPoint.y + mBox.mY + 40, -40);
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
	case Lawn::ZOMBIE_NORMAL: {
		mBodyReanim.SetTrackVisible("anim_head1", false);
		mBodyReanim.SetTrackVisible("anim_head2", false);
		mBodyReanim.SetTrackVisible("anim_hair", false);
		sgf::Point dropPoint = mBodyReanim.GetTrackPos("Zombie_neck");
		sgf::Particle* head = mBoard->SpawnParticleAt(gLawnApp->mResourceManager.GetResource<sgf::Emitter>("PAXML_ZOMHEADDROP"), dropPoint.x + mBox.mX - 85, dropPoint.y + mBox.mY + 40, -80);
		break;
	}
	}
}

void Lawn::Zombie::DoDeathReanim()
{
	if (!sgf::Rand(0, 2))
		PlayTrack("anim_death", 0);
	else {
		if (!sgf::Rand(0, 3))
			PlayTrack("anim_superlongdeath", 200);
		else
			PlayTrack("anim_death2", 200);
	}
	
	mBodyReanim.Play(sgf::Animator::PlayState::PLAY_ONCE);
}

void Lawn::Zombie::PlayTrack(const sgf::String& trackName,int blendTime)
{
	mBodyReanim.mFrameIndexBlendBuffer = mBodyReanim.mFrameIndexNow;

	mBodyReanim.SetFrameRangeByTrackName(trackName);
	
	mBodyReanim.mReanimBlendCounter = blendTime;
}

void Lawn::Zombie::UpdateEating(Plant* target)
{
	if (!mIsEating) {
		PlayTrack("anim_eat");
		mBodyReanim.mSpeed = 2.5f;
	}
	target->TakeDamage(100.0f * float(mTickDelta) / 1000.0f);
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
		mEatingChunkCounter -= mTickDelta;
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
	gZombiesDefinitions[ZOMBIE_NORMAL] = { ZOMBIE_NORMAL,"RAXML_ZOMBIE","NormalZombie","NormalZombie",1.0f,1.4f,1,-20,-40 };
}

void Lawn::Zombie::Update()
{
	unsigned int tickNow = sgf::TryGetTicks();
	mTickDelta = tickNow - mTickCache;
	mTickCache = tickNow;

	mBodyReanim.Update();
	if (mBodyReanim.GetTrackVisible("_ground")) {
		float speedX = mBodyReanim.GetTrackVelocity("_ground");
		mBox.mX -= speedX * 0.5f * float(mTickDelta) / mBodyReanim.mDeltaRate * mBodyReanim.mSpeed;
	}

	if (mFlashCounter > mTickDelta) {
		mFlashCounter -= mTickDelta;
	}
	else {
		mFlashCounter = 0;
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
				PlayTrack("anim_walk");
			}
			mIsEating = false;
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
