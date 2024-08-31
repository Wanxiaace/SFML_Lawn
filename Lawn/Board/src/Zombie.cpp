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
	mBodyReanim.SetFrameRangeByTrackName("anim_walk");
	mBodyReanim.Play();
	mHealth = 270;
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
		mBodyReanim.SetFrameRangeByTrackName("anim_walk2");
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

void Lawn::Zombie::InitZombiesDefinitions()
{
	gZombiesDefinitions[ZOMBIE_NORMAL] = { ZOMBIE_NORMAL,"RAXML_ZOMBIE","NormalZombie","NormalZombie",1.0f,1.4f,1,-20,-40 };
}

void Lawn::Zombie::Update()
{
	unsigned int tickNow = sgf::TryGetTicks();
	unsigned int tickDelta = tickNow - mTickCache;
	mTickCache = tickNow;

	mBodyReanim.Update();
	if (mBodyReanim.GetTrackVisible("_ground")) {
		float speedX = mBodyReanim.GetTrackVelocity("_ground");
		mBox.mX -= speedX * 0.5f * float(tickDelta) / mBodyReanim.mDeltaRate * mBodyReanim.mSpeed;
	}
	
}

void Lawn::Zombie::Draw(sgf::Graphics* g)
{
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 0,0,1,1 });
		g->FillRect(mBox.mWidth, mBox.mHeight);
	}
	g->SetCubeColor({ 1,1,1,1 });

	if (mShadowImage)
	{
		g->Translate(-10, mBox.mHeight - 25);
		g->DrawImage(mShadowImage);
	}

	g->MoveTo(0,0);
	g->Translate(mReanimOffsetX, mReanimOffsetY);
	if (mBodyReanim.mReanim)
		mBodyReanim.Present(g);
}
