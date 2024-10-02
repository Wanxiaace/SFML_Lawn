#include "Board.h"
#include "../include/LawnCursor.h"
#include "../include/Constant.h"
#include "../include/LawnApp.h"
#include "include/Plant.h"

Lawn::Board::Board(LawnApp* app):Widget(LAWN_WIDGET_BOARD)
{
	mApp = app;
	mApp->mCursor->mIsOnBoard = true;
	mApp->mCursor->mBoard = this;
	mBackGroundType = BACKGROUND_FRONT_YARD_DAY;
	UpdateBoardBackground();
	Resize(0, 0, LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);

	mMenuButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_PAUSE,mApp);
	//std::cout << _LS("Menu") << std::endl;
	mMenuButton->LoadLabel(_LS("Menu"));
	mMenuButton->Resize(LAWN_GAME_WINDOW_WIDTH - 130, 0, 120, 50);
	mMenuButton->AttachToListener(this);

	mDefeatReturnToMenuButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_RETURN_TO_MENU, mApp);
	mDefeatReturnToMenuButton->LoadLabel(_LS("ReturnToMenu"));
	mDefeatReturnToMenuButton->Resize((LAWN_GAME_WINDOW_WIDTH - 210)/2, 500, 210, 50);
	mDefeatReturnToMenuButton->AttachToListener(this);
	mDefeatReturnToMenuButton->mVisible = false;

	mSeedBank = new SeedBank(this);
	mSeedBank->AttachToListener(this);

	AppendChild(mMenuButton);
	AppendChild(mSeedBank);
	AppendChild(mDefeatReturnToMenuButton);
	AttachToListener(this);
}

Lawn::Board::~Board()
{
	mApp->mCursor->mIsOnBoard = false;
	mApp->mCursor->Reset();
	Widget::~Widget();
	mWidgetManager->RemoveWidget(mMenuButton);
	mWidgetManager->RemoveWidget(mSeedBank);
	mWidgetManager->RemoveWidget(mDefeatReturnToMenuButton);
	delete mMenuButton;
	delete mSeedBank;
	delete mDefeatReturnToMenuButton;
	if (mZombieAnimator)
		delete mZombieAnimator;

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
	mApp->mMusicManager.FadeInMusic("MUSIC_LAWNBGM(13)",5000);

}

void Lawn::Board::DrawBackDrop(sgf::Graphics* g) const
{
	g->SetCubeColor({1,1,1,1});
	
	if(mBackGroundImageCache)
		g->DrawImage(mBackGroundImageCache);
}

int Lawn::Board::PointXtoGridX(int pointX) const
{
	auto rX = round(float(pointX - mGridOriPosX) / float(mGridWidth));
	if (rX < 0)
		rX = 0;
	if (rX > 9)
		rX = 9;
	return rX;
}

int Lawn::Board::PointYtoGridY(int pointY) const
{
	auto rY = round(float(pointY - mGridOriPosY) / float(mGridHeight));
	if (rY < 0)
		rY = 0;
	if (rY > 4)
		rY = 4;
	return rY;
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

void Lawn::Board::ZombieWin(Zombie* target)
{
	mWinZombie = target;
	mWinZombie->PlayTrack("anim_eat");
	mZombieAnimator = new sgf::Animator(gLawnApp->mResourceManager.GetResourceFast<sgf::Reanimation>("RAXML_ZOMBIESWON"),gLawnApp);
	mZombieAnimator->SetFrameRangeByTrackName("anim_screen");
	mZombieAnimator->Play(sgf::Animator::PlayState::PLAY_ONCE);
	mIsZombieWin = true;
	mBlackScreenCounter = 2000;
	mIsBoardRunning = false;
	mSeedBank->mVisible = false;
	mMenuButton->mVisible = false;
	mDefeatReturnToMenuButton->mVisible = true;
	mApp->mMusicManager.FadeOutMusic(2000);
	mApp->mMusicManager.PlayChunk("CHUNK_LOSEMUSIC");
}

void Lawn::Board::Update()
{
	if (mBlackScreenCounter > mTickDelta)
		mBlackScreenCounter -= mTickDelta;
	else
		mBlackScreenCounter = 0;

	if (!mIsBoardRunning) {
		mWinZombie->mTickDelta = mTickDelta;
		mWinZombie->Update();
		if(mBlackScreenCounter <= 0)
			mZombieAnimator->Update();
		return;
	}
		
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


	mParticleManager.Update(mTickDelta);
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

	if (mIsZombieWin) {
		g->ModelMoveTo(0, 0);
		g->MoveTo(0, 0);
		
		g->SetCubeColor({ 0,0,0,1.0f - mBlackScreenCounter / 2000.0f});
		g->FillRect({ 0,0,LAWN_GAME_WINDOW_WIDTH ,LAWN_GAME_WINDOW_HEIGHT });

		g->SetCubeColor({ 1,1,1,1 });
		
		if (mBlackScreenCounter <= 0)
		{
			g->Translate((LAWN_GAME_WINDOW_WIDTH - 800) / 2, 0);
			mZombieAnimator->Present(g);
		}

		g->ModelMoveTo(boardPos.first + mWinZombie->mBox.mX, boardPos.second + mWinZombie->mBox.mY);
		mWinZombie->Draw(g);

	}

}

void Lawn::Board::OnClick(int theId)
{
	if (theId == mId && mApp->mCursor->mState == LAWN_CURSOR_SEED) {
		mApp->mCursor->SeedPlant();
	}

	switch (theId)
	{
	case LAWN_WIDGET_BUTTON_RETURN_TO_MENU:
		gLawnApp->KillBoard();
		gLawnApp->EnterGameSelector();
		break;
	case LAWN_WIDGET_BUTTON_PAUSE:
		//gLawnApp->KillBoard();
		//gLawnApp->EnterGameSelector();

		gLawnApp->GamePause();
		break;
	case LAWN_SEED_BANK:
		mSeedBank->ClickOn();
		break;
	default:
		break;
	}
}
