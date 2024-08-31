#include "../include/Projectile.h"
#include "../../include/LawnApp.h"

std::unordered_map<Lawn::ProjectileType, Lawn::ProjectileDefinition> Lawn::gProjectilesDefinitions;

Lawn::Projectile::Projectile()
{
	mShadowImage = (sgf::SimpleImage*)gLawnApp->mResourceManager.mResourcePool["IMAGE_PLANTSHADOW"];
}

Lawn::Projectile::~Projectile()
{

}

void Lawn::Projectile::Init()
{
	mTickCache = sgf::TryGetTicks();
	auto& def = gProjectilesDefinitions[mProjectileType];
	mProjectileImage = (sgf::SimpleImage*)gLawnApp->mResourceManager.mResourcePool[def.mImageID];
	mImageOffsetX = def.mImageOffsetX;
	mImageOffsetY = def.mImageOffsetY;
	mDamage = def.mDamage;
	mBox.mWidth = 30;
	mBox.mHeight = 30;

	switch (mProjectileType)
	{
	case Lawn::PROJECTILE_PEA:

		break;
	}
}

void Lawn::Projectile::DoEffect(Zombie* zom)
{
	switch (mProjectileType)
	{
	case Lawn::PROJECTILE_PEA: 
	{
		mBoard->SpawnParticlesAt((sgf::Emitter*)gLawnApp->mResourceManager.mResourcePool["PAXML_PEAPARTICLETHROW"], 10,mBox.mX + 10, mBox.mY+80, -80);
		mBoard->SpawnParticleAt((sgf::Emitter*)gLawnApp->mResourceManager.mResourcePool["PAXML_PEASPLATS"], mBox.mX + 10, mBox.mY + 80, -80);
		break;
	}
	}
	Die();
}

void Lawn::Projectile::Die()
{
	mAvailable = false;
}

void Lawn::Projectile::InitProjectilesDefinitions()
{
	gProjectilesDefinitions[PROJECTILE_PEA] = {
		PROJECTILE_PEA,
		20,
		15,0,
		"Pea",
		"IMAGE_PROJECTILEPEA",
	};
}

Lawn::Zombie* Lawn::Projectile::TryToFindTarget()
{
	Zombie* result = nullptr;
	for (auto& x : mBoard->mZombieVector)
	{
		if (mBox.IsOverlap(x->mBox)) {
			result = x;
			break;
		}
	}
	return result;
}

void Lawn::Projectile::Update()
{
	if (!mAvailable)
		return;
	unsigned int tick = sgf::TryGetTicks();
	int tickDelta = tick - mTickCache;
	mTickCache = tick;

	switch (mMotionType)
	{
	case Lawn::PROJECTILE_MOTION_PHYSIC:
		mZ += mSpeedZ * float(tickDelta) / 100.0f;
		mSpeedZ -= mGravity * float(tickDelta) / 100.0f;
	case Lawn::PROJECTILE_MOTION_NORMAl:
		mBox.mX += mSpeedX * float(tickDelta) / 100.0f;
		mBox.mY += mSpeedY * float(tickDelta) / 100.0f;
		break;
	default:
		break;
	}

	Zombie* target = TryToFindTarget();
	if (target) {
		DoEffect(target);
	}

}

void Lawn::Projectile::Draw(sgf::Graphics* g)
{
	if (!mAvailable)
		return;
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 0,0,1,1 });
		g->FillRect(mBox.mWidth, mBox.mHeight);
	}

	g->SetCubeColor({ 1,1,1,1 });
	if (mProjectileImage) {
		g->MoveTo(mImageOffsetX, mImageOffsetY);
		g->DrawImageScaleF(mProjectileImage, mScale, mScale);
	}
}
