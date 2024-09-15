#include "Board.h"
#include "../include/Constant.h"
#include "../include/LawnApp.h"
#include "include/Plant.h"

Lawn::Board::Board(sgf::GameApp* app):Widget(LAWN_WIDGET_BOARD)
{
	mApp = app;
	mBackGroundType = BACKGROUND_FRONT_YARD_DAY;
	UpdateBoardBackground();
	Resize(0, 0, LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);

	mMenuButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_MENU,mApp);
	//std::cout << _LS("Menu") << std::endl;
	mMenuButton->LoadLabel(_LS("Menu"));
	mMenuButton->Resize(LAWN_GAME_WINDOW_WIDTH - 130, 0, 120, 50);
	mMenuButton->AttachToListener(this);

	mSeedBank = new SeedBank(this);

	AppendChild(mMenuButton);
	AppendChild(mSeedBank);
}

Lawn::Board::~Board()
{
	Widget::~Widget();
	mWidgetManager->RemoveWidget(mMenuButton);
	mWidgetManager->RemoveWidget(mSeedBank);
	delete mMenuButton;
	delete mSeedBank;

	for (auto& x : mPlantVector)
	{
		delete x;
	}
	for (auto& x : mZombieVector)
	{
		delete x;
	}

	
}

void Lawn::Board::UpdateBoardBackground()
{
	switch (mBackGroundType)
	{
	case Lawn::BACKGROUND_FRONT_YARD_DAY:
		mBackGroundImageCache = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_BACKGROUND1"];
		mGridOriPosX = 255;
		mGridOriPosY = 85;
		mGridWidth = 80;
		mGridHeight = 95;
		break;
	case Lawn::BACKGROUND_FRONT_YARD_NIGHT:
		mBackGroundImageCache = (sgf::SimpleImage*)mApp->mResourceManager.mResourcePool["IMAGE_BACKGROUND2"];
		mGridOriPosX = 255;
		mGridOriPosY = 85;
		mGridWidth = 80;
		mGridHeight = 95;
		break;
	default:
		break;
	}
	mBackgroundScaleF = float(LAWN_GAME_WINDOW_HEIGHT) / mBackGroundImageCache->mSurface->h;
	mApp->mMusicManager.PlayMusic("MUSIC_TH06_B3");

}

void Lawn::Board::DrawBackDrop(sgf::Graphics* g) const
{
	g->SetCubeColor({1,1,1,1});
	
	if(mBackGroundImageCache)
		g->DrawImage(mBackGroundImageCache);
}

int Lawn::Board::PointXtoGridX(int pointX) const
{
	return (pointX - mGridOriPosX) / mGridWidth;
}

int Lawn::Board::PointYtoGridY(int pointY) const
{
	return (pointY - mGridOriPosY) / mGridHeight;
}

int Lawn::Board::GridXtoPointX(int gridX) const
{
	return mGridOriPosX + gridX * mGridWidth;
}

int Lawn::Board::GridYtoPointY(int gridY) const
{
	return mGridOriPosY + gridY * mGridHeight;
}

Lawn::Plant * Lawn::Board::SpawnPlantAt(int gridX, int gridY,SeedType seedType)
{
	Lawn::Plant* plant = new Lawn::Plant();
	plant->mSeedType = seedType;
	plant->mBoard = this;
	plant->MoveTo(GridXtoPointX(gridX),GridYtoPointY(gridY));
	plant->PlantInit();
	
	mPlantVector.push_back(plant);

	return plant;
}

Lawn::Zombie* Lawn::Board::SpawnZombieAt(int gridX, int gridY, ZombieType zomType)
{
	Lawn::Zombie* zombie = new Lawn::Zombie();
	zombie->mZombieType = zomType;
	zombie->mBoard = this;
	zombie->MoveTo(GridXtoPointX(gridX), GridYtoPointY(gridY));
	zombie->Init();

	mZombieVector.push_back(zombie);

	return zombie;
}

Lawn::Projectile* Lawn::Board::SpawnProjectileAt(int x, int y, ProjectileType projectileType)
{
	Lawn::Projectile* projectile = new Lawn::Projectile();
	projectile->mProjectileType = projectileType;
	projectile->MoveTo(x, y);
	projectile->mBoard = this;
	projectile->Init();
	

	mProjectileVector.push_back(projectile);
	return projectile;
}

sgf::Particle* Lawn::Board::SpawnParticleAt(sgf::Emitter* emitter,int x, int y, int z)
{
	mParticleManager.MoveTo(x, y, z);
	return mParticleManager.EmittParticle(emitter);
}

void Lawn::Board::SpawnParticlesAt(sgf::Emitter* emitter, int number, int x, int y, int z)
{
	mParticleManager.MoveTo(x, y, z);
	mParticleManager.EmittParticles(emitter, number);
}

void Lawn::Board::Update()
{
	int length = mPlantVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mPlantVector[i]->mAvailable) {
			delete mPlantVector[i];
			mPlantVector.erase(mPlantVector.begin() + i);
			i--;
			length--;
			continue;
		}
		mPlantVector[i]->mTickDelta = mTickDelta;
		mPlantVector[i]->Update();
	}

	length = mZombieVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mZombieVector[i]->mAvailable) {
			delete mZombieVector[i];
			mZombieVector.erase(mZombieVector.begin() + i);
			i--;
			length--;
			continue;
		}

		mZombieVector[i]->mTickDelta = mTickDelta;
		mZombieVector[i]->Update();
	}

	length = mProjectileVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mProjectileVector[i]->mAvailable) {
			delete mProjectileVector[i];
			mProjectileVector.erase(mProjectileVector.begin() + i);
			i--;
			length--;
			continue;
		}
		mProjectileVector[i]->mTickDelta = mTickDelta;
		mProjectileVector[i]->Update();
	}


	mParticleManager.Update();
	mSeedBank->Update();
}

void Lawn::Board::Draw(sgf::Graphics* g)
{
	DrawBackDrop(g);
	if (gShowWidgetHitBoxAllowed) {
		g->SetCubeColor({ 1,0,0,0.5f });
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				g->MoveTo(GridXtoPointX(j), GridYtoPointY(i));
				g->FillRect(mGridWidth - 2, mGridHeight - 2);
			}
		}
	}

	auto boardPos = GetExactPosition();

	for (auto& x : mPlantVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
		g->MoveTo(0,0);
		x->Draw(g);
	}

	for (auto& x : mZombieVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
		g->MoveTo(0, 0);
		x->Draw(g);
	}

	for (auto& x : mProjectileVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
		g->MoveTo(0, 0);
		x->Draw(g);
	}

	for (auto& x : mParticleManager.mParticles)
	{
		g->ModelMoveTo(boardPos.first, boardPos.second);
		g->MoveTo(0, 0);
		x->Draw(g);
	}

}

void Lawn::Board::OnClick(int theId)
{
	switch (theId)
	{
	case LAWN_WIDGET_BUTTON_MENU:
		gLawnApp->KillBoard();
		gLawnApp->EnterGameSelector();

		break;
	default:
		break;
	}
}
