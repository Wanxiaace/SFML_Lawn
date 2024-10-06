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


	AutoSpawnZombieWaves();
	mCurrentWaveIndex = 0;
	mNextWaveCounts = 1000;
	mStartSpawningZombie = true;


	LoadZombieFromJsonFile("level0.json");
	InitLawnMover();
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

Lawn::LawnMover* Lawn::Board::SpawnLawnMoverAt(int gridX, int gridY, LawnMoverType carType)
{
	LawnMover* result = new LawnMover();
	result->Init(carType);
	result->MoveTo(GridXtoPointX(gridX) - 80, GridYtoPointY(gridY) + 20);
	mLawnMoverVector.push_back(result);
	return result;
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

int Lawn::Board::GetCurrentZombieNum() const
{
	return mZombieVector.size();
}

static float GetZombieWaveCurve(float x, float waveMax) {
	return sinf(x * 50.0f / waveMax * 3.14159f / 100.0f) * 10 + 1;
};

static float GetZombeiSleepTimeCurve(float x, float waveMax) {
	return cosf(x * 50.0f / waveMax * 3.14159f / 300.0f) * 100 - 70;
};

//TODO 完成读取文件
void Lawn::Board::AutoSpawnZombieWaves()
{
	for (size_t i = 0; i < 10; i++)
	{
		int num = GetZombieWaveCurve(i,10);
		ZombieWave zombieWave;
		zombieWave.mSleepTime = GetZombeiSleepTimeCurve(i, 10) * 1000;

		for (size_t j = 0; j < num; j++)
		{
			ZombieType zomType = ZOMBIE_NORMAL;

			for (auto& x : mLevel.mZombieTypes)
			{
				ZombieDefinition& def = gZombiesDefinitions[x];
				int spot = sgf::Rand(0, 100);
				if (def.mMinimumWave < i && def.mPickWeight > spot)
				{
					zomType = def.mZombieType;
				}
			}

			zombieWave.AppendZombie(zomType,sgf::Rand(0,5), 11);
		}
		mLevel.mZombieWaves.push_back(zombieWave);
	}
}

void Lawn::Board::UpdateZombieWaves()
{
	if (!mStartSpawningZombie)
		return;

	if (mNextWaveCounts != -1) {
		if (mNextWaveCounts < mTickDelta)
		{
			if (mLevel.mZombieWaves.size() > mCurrentWaveIndex)
			{
				ZombieWave& currentZombieWave = mLevel.mZombieWaves[mCurrentWaveIndex];
				mNextWaveCounts = currentZombieWave.mSleepTime;
				mCurrentWaveIndex += 1;

				if (mCurrentWaveIndex > 0 && !((mCurrentWaveIndex) % BOARD_WAVE_EACH_HUGE_EAVE)) {
					gLawnApp->mMusicManager.PlayChunk("CHUNK_HUGEWAVE");
					mHugeCounter = 3000;
				}
				for (size_t i = 0; i < currentZombieWave.mZombieNum; i++)
				{
					SpawnZombieAt(currentZombieWave.mZombies[i].mTargetColumn,
						currentZombieWave.mZombies[i].mTargetRow, currentZombieWave.mZombies[i].mZombieType);
				}
			}
			else
				mNextWaveCounts = -1;
		}
		else {
			if (GetCurrentZombieNum() == 0 && mNextWaveCounts > 300) {
				mNextWaveCounts = 300;
			}
			mNextWaveCounts -= mTickDelta;
		}
	}
}

std::unordered_map<sgf::String, Lawn::ZombieType> gZombieStringMap = {
	{"ZOMBIE_NORMAL",Lawn::ZOMBIE_NORMAL},
	{"ZOMBIE_PAIL",Lawn::ZOMBIE_PAIL},
	{"ZOMBIE_TRAFFIC_CONE",Lawn::ZOMBIE_TRAFFIC_CONE},
	{"ZOMBIE_DOOR",Lawn::ZOMBIE_DOOR},
};

void Lawn::Board::LoadZombieFromJson(const nlohmann::json& json)
{
	mLevel.mLevelName = json["LevelName"];
	mLevel.mWavesNum = json["WavesNumber"];
	nlohmann::json autoLoadArray = json["AutoLoadZombie"];
	nlohmann::json zombieArray = json["ZombieList"];
	for (auto& x : autoLoadArray)
	{
		ZombieType type = gZombieStringMap[x];
	}

	AutoSpawnZombieWaves();

	for (auto& x : zombieArray)
	{
		ZombieType type = gZombieStringMap[x["Type"]];
		int row = x["Row"];
		int column = x["Column"];
		int currentWave = x["CurrentWave"];
		mLevel.mZombieWaves[currentWave - 1].AppendZombie(type, row, column);
		//std::cout << x["Type"] << " " << row << " " << column << std::endl;
	}

	
}

#include <fstream>


void Lawn::Board::LoadZombieFromJsonFile(const char* path)
{
	std::ifstream file;
	file.open(path);
	file.seekg(0,std::ios_base::end);
	int size = file.tellg();
	file.seekg(0, std::ios_base::beg);
	char* jsonStr = new char[size+1];
	memset(jsonStr,0, size + 1);
	file.read(jsonStr, size);
	file.close();

	nlohmann::json result = nlohmann::json::parse(jsonStr);

	LoadZombieFromJson(result);
}

void Lawn::Board::DrawLevelInfo(sgf::Graphics* g)
{
	if (!mLevelNameImage) {
		sgf::Font* levelFont = gLawnApp->mResourceManager.GetResourceFast<sgf::Font>("FONT_FONT3");
		levelFont->SetFontSize(30);
		mLevelNameImage = levelFont->GenTextImage(mLevel.mLevelName);
	}
	g->ModelMoveTo(0, 0);
	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - mLevelNameImage->GetWidth()) / 2 + 2, 50);
	g->SetCubeColor({ 0,0,0,1 });
	g->DrawImage(mLevelNameImage);

	g->Translate(-2, -2);
	g->SetCubeColor({ 0.96,0.97,0.73,1 });
	g->DrawImage(mLevelNameImage);

	sgf::SimpleImage* processBarImage = gLawnApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_FLAGMETER");
	sgf::SimpleImage* processBarPartsImage = gLawnApp->mResourceManager.GetResourceFast<sgf::SimpleImage>("IMAGE_FLAGMETERPARTS");

	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2, 20);
	g->SetCubeColor({ 1,1,1,1 });
	g->DrawImageGridAtlas(processBarImage,2,1,0,0);
	
	g->Present();
	g->SetClipRect({ (LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2,20,processBarImage->GetWidth() * mFlagMeterIndex / float(mLevel.mZombieWaves.size()),processBarImage->GetHeight()});
	g->MoveTo(0,0);
	g->DrawImageGridAtlas(processBarImage,2,1,1,0);
	g->Present();
	g->ClearClipRect();

	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2 + processBarImage->GetWidth() * mFlagMeterIndex / float(mLevel.mZombieWaves.size()) - 5, 20);
	g->DrawImageGridAtlas(processBarPartsImage, 1, 2, 0, 0);

	int hugeWaveNum = mLevel.mZombieWaves.size() / BOARD_WAVE_EACH_HUGE_EAVE;
	float distanceEachFlag = processBarImage->GetWidth() * BOARD_WAVE_EACH_HUGE_EAVE / float(mLevel.mZombieWaves.size());
	for (size_t i = 1; i <= hugeWaveNum; i++)
	{
		g->MoveTo((LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2 + i * distanceEachFlag - 25, 20);
		if (mCurrentWaveIndex >= i * BOARD_WAVE_EACH_HUGE_EAVE)
			g->Translate(0,-5);
		g->DrawImageGridAtlas(processBarPartsImage, 1, 2, 0, 1);
	}
}

void Lawn::Board::InitLawnMover()
{
	SpawnLawnMoverAt(0,0,LAWN_MOVER_NORMAL);
	SpawnLawnMoverAt(0,1,LAWN_MOVER_NORMAL);
	SpawnLawnMoverAt(0,2,LAWN_MOVER_NORMAL);
	SpawnLawnMoverAt(0,3,LAWN_MOVER_NORMAL);
	SpawnLawnMoverAt(0,4,LAWN_MOVER_NORMAL);
}

void Lawn::Board::Update()
{
	if (mBlackScreenCounter > mTickDelta)
		mBlackScreenCounter -= mTickDelta;
	else
		mBlackScreenCounter = 0;

	if (mHugeCounter > mTickDelta)
		mHugeCounter -= mTickDelta;
	else
		mHugeCounter = 0;

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

	length = mLawnMoverVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mLawnMoverVector[i]->mAvailable) {
			delete mLawnMoverVector[i];
			mLawnMoverVector.erase(mLawnMoverVector.begin() + i);
			i--;
			length--;
			continue;
		}

		mLawnMoverVector[i]->mTickDelta = mTickDelta;
		mLawnMoverVector[i]->Update();
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

	if (mFlagMeterIndex < float(mCurrentWaveIndex))
		mFlagMeterIndex += float(mTickDelta) / 200.0f;


	mParticleManager.Update(mTickDelta);
	mSeedBank->Update();
	UpdateZombieWaves();
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

	for (auto& x : mLawnMoverVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
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


	DrawLevelInfo(g);
	
	if (mHugeCounter > 0) {
		if (!mHugeTitleImage) {
			sgf::Font* levelFont = gLawnApp->mResourceManager.GetResourceFast<sgf::Font>("FONT_BASEFONT");
			levelFont->SetFontSize(50);
			mHugeTitleImage = levelFont->GenTextImage(_LS("HugeWave"));
		}

		float scaleF = 0;
		if (mHugeCounter >= 2800)
			scaleF = (mHugeCounter - 2800) / 200.0f + 1.0f;
		else
			scaleF = 1.0f;

		glm::mat4x4 bigTitleMatrix = glm::scale(glm::mat4x4(1.0f), glm::vec3(scaleF, scaleF, 1.0f));
		
		g->ModelMoveTo(
			(LAWN_GAME_WINDOW_WIDTH - mHugeTitleImage->GetWidth()) / 2 + 2,
			(LAWN_GAME_WINDOW_HEIGHT - mHugeTitleImage->GetHeight()) / 2 + 2);
		g->MoveTo(0,0);
		g->SetCubeColor({ 0,0,0,1 });
		g->DrawImageMatrix(mHugeTitleImage, bigTitleMatrix, mHugeTitleImage->GetWidth() / 2, mHugeTitleImage->GetHeight() / 2);
		g->Translate(-2,-2);
		g->SetCubeColor({ 1,0,0,1 });
		g->DrawImageMatrix(mHugeTitleImage, bigTitleMatrix, mHugeTitleImage->GetWidth() / 2, mHugeTitleImage->GetHeight() / 2);
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
