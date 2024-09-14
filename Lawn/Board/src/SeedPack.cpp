#include "../include/SeedPack.h"
#include "../../include/Constant.h"

void Lawn::DrawSeedPack(SeedType type, sgf::Graphics* g, float scaleF)
{
	scaleF *= 0.4f;
	PlantDefinition& def = gPlantsDefinitions[type];
	SeedPackBackground bkg = GetSeedPackBackground(type);
	DrawSeedBackgound(bkg, g, scaleF);
	g->Translate(-2* scaleF,-2 * scaleF);
	g->DrawImageGridAtlasScaleF(gLawnApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_PACKET_PLANTS_PLUS"), 1, 56, 0, type, scaleF * 2.7, scaleF * 2.7);

}


void Lawn::DrawSeedBackgound(SeedPackBackground type, sgf::Graphics* g, float scaleF)
{
	switch (type)
	{
	case Lawn::SEEDPACK_NORMAL:
		g->DrawImageGridAtlasScaleF(gLawnApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_SEEDPACKETS"), 1, 5, 0, 2,scaleF,scaleF);
		break;
	default:
		break;
	}
}

Lawn::SeedPackBackground Lawn::GetSeedPackBackground(SeedType type)
{
	SeedPackBackground res = SEEDPACK_NORMAL;

	switch (type)
	{
	default:
		break;
	}

	return res;
}

Lawn::SeedPack::SeedPack()
{
	Init(SEED_PEASHOOTER);
}

Lawn::SeedPack::~SeedPack()
{
	
}

void Lawn::SeedPack::Init(SeedType type)
{
	PlantDefinition& def = gPlantsDefinitions[type];
	sgf::Font* font = gLawnApp->mResourceManager.GetResourceFast<sgf::Font>("FONT_FONT4");
	mCost = def.mCost;
	mTextImage = font->GenTextImage(std::to_string(mCost));

}

void Lawn::SeedPack::Draw(sgf::Graphics* g)
{
	DrawSeedPack(mSeedType,g,mScaleF);
	g->Translate(60 * mScaleF, 35 * mScaleF);
	if (mTextImage) {
		g->SetCubeColor({0,0,0,1});
		g->DrawImageScaleF(mTextImage, mScaleF * 0.7, mScaleF * 0.7);

		g->Translate(-1* mScaleF,-1* mScaleF);
		g->SetCubeColor({ 1,1,1,1 });
		g->DrawImageScaleF(mTextImage, mScaleF * 0.7, mScaleF * 0.7);
	}
}

Lawn::SeedBank::SeedBank() : sgf::Widget(LAWN_SEED_BANK)
{

}

Lawn::SeedBank::~SeedBank()
{
}
