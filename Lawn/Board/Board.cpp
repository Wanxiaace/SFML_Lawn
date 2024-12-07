#include "Board.h"
#include "../LawnCursor.h"
#include "../Constant.h"
#include "../LawnApp.h"
#include "Plant.h"

Lawn::Board::Board():Widget(LAWN_WIDGET_BOARD)
{
	gLawnApp->mCursor->mIsOnBoard = true;
	gLawnApp->mCursor->mBoard = this;
	mBackGroundType = BACKGROUND_FRONT_YARD_DAY;
	UpdateBoardBackground();
	Resize(0, 0, LAWN_GAME_WINDOW_WIDTH, LAWN_GAME_WINDOW_HEIGHT);

	mDefeatReturnToMenuButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_RETURN_TO_MENU);
	mDefeatReturnToMenuButton->LoadLabel(_LS("ReturnToMenu"));
	mDefeatReturnToMenuButton->Resize((LAWN_GAME_WINDOW_WIDTH - 210)/2, 500, 210, 50);
	mDefeatReturnToMenuButton->AttachToListener(this);
	mDefeatReturnToMenuButton->mVisible = false;

	mSeedBank = new SeedBank(this);
	mSeedBank->AttachToListener(this);

	AppendChild(mSeedBank);
	AppendChild(mDefeatReturnToMenuButton);
	AttachToListener(this);

	mCurrentWaveIndex = 0;
	mNextWaveCounts = 1000;
	//mStartSpawningZombie = false;

	LoadLevelFromJsonFile("level0.json");
	SpawnZombiesInView();
	InitLawnMover();
}

Lawn::Board::~Board()
{
	gLawnApp->mCursor->mIsOnBoard = false;
	gLawnApp->mCursor->Reset();
	if (mSeedChooseScreen)
		gLawnApp->SafeDeleteWidget(mSeedChooseScreen);

	for (auto& x : mPlantVector)
	{
		delete x;
	}
	for (auto& x : mZombieVector)
	{
		delete x;
	}

	if (mStartReadySetPlantAnim)
		delete mStartReadySetPlantAnim;
}

void Lawn::Board::UpdateBoardBackground()
{
	switch (mBackGroundType)
	{
	case Lawn::BACKGROUND_FRONT_YARD_DAY:
		mBackGroundImageCache = RES_IMAGE::IMAGE_BACKGROUND1;
		mGridOriPosX = 255;
		mGridOriPosY = 85;
		mGridWidth = 80;
		mGridHeight = 95;
		break;
	case Lawn::BACKGROUND_FRONT_YARD_NIGHT:
		mBackGroundImageCache = RES_IMAGE::IMAGE_BACKGROUND2;
		mGridOriPosX = 255;
		mGridOriPosY = 85;
		mGridWidth = 80;
		mGridHeight = 95;
		break;
	default:
		break;
	}
	mBackgroundScaleF = float(LAWN_GAME_WINDOW_HEIGHT) / mBackGroundImageCache->mSurface->h;
	
}

void Lawn::Board::TryShowCutSceneBegin()
{
	mSeedBank->mVisible = false;
	mCutSenceHolder.mTick.BindToCounter(&mTick);
	mCutSenceHolder.SetSpeed(-300);
	mCutSenceHolder.BindSpot(&mRect.mX, 0,-250,CURVE_EASE_IN_OUT);

	mCutSenceHolder.SetNextFunction([this]() {
		if(GetWillChooseCard())
			ShowSeedChooseScreen();
		else {
			gLawnApp->mBoard->mSeedBank->mVisible = true;
			mCutSenceHolder.SetSpeed(300);
			mCutSenceHolder.BindSpot(&mRect.mX, 0, -250, CURVE_EASE_IN_OUT);
			mCutSenceHolder.SetNextFunction([this]() {
				EndCutsence();
				});
		}
		});
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
	plant->Init();
	
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

Lawn::SceneObejct * Lawn::Board::SpawnSceneObjectAt(int x, int y, SceneObjectType objType)
{
	Lawn::SceneObejct* obj = new Lawn::SceneObejct();
	obj->mType = objType;
	obj->mBoard = this;
	obj->MoveTo(x,y);
	obj->Init();
	mSceneObjectVector.push_back(obj);
	return nullptr;
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
	mZombieAnimator = new sgf::Animator(RES_RAXML::RAXML_ZOMBIESWON);
	mZombieAnimator->SetFrameRangeByTrackName("anim_screen");
	mZombieAnimator->Play(sgf::Animator::PlayState::PLAY_ONCE);
	mIsZombieWin = true;
	mBlackScreenCounter = 2000;
	mIsBoardRunning = false;
	mSeedBank->mVisible = false;
	mDefeatReturnToMenuButton->mVisible = true;
	gLawnApp->mMusicManager.FadeOutMusic(2000);
	gLawnApp->mMusicManager.PlayChunk("CHUNK_LOSEMUSIC");
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
void Lawn::Board::AutoSpawnZombieWaves(int waveMax)
{
	for (int i = 0; i < waveMax; i++)
	{
		
		int num = GetZombieWaveCurve(i, waveMax);
		if (!((i + 1) % BOARD_WAVE_EACH_HUGE_EAVE))
			num *= mLevel.mHugeWaveScale;
		ZombieWave zombieWave;
		zombieWave.mSleepTime = GetZombeiSleepTimeCurve(i, waveMax) * 1000;

		for (int j = 0; j < num; j++)
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
		if (mNextWaveCounts < mTick.GetDeltaTick())
		{
			if (int(mLevel.mZombieWaves.size()) > mCurrentWaveIndex)
			{
				ZombieWave& currentZombieWave = mLevel.mZombieWaves[mCurrentWaveIndex];
				mNextWaveCounts = currentZombieWave.mSleepTime;
				mCurrentWaveIndex += 1;

				if (mCurrentWaveIndex > 0 && !((mCurrentWaveIndex) % BOARD_WAVE_EACH_HUGE_EAVE)) {
					gLawnApp->mMusicManager.PlayChunk("CHUNK_HUGEWAVE");
					mHugeCounter = 3000;
				}
				for (int i = 0; i < currentZombieWave.mZombieNum; i++)
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
			mNextWaveCounts -= mTick.GetDeltaTick();
		}
	}
}

std::unordered_map<sgf::String, Lawn::ZombieType> gZombieStringMap = {
	{"ZOMBIE_NORMAL",Lawn::ZOMBIE_NORMAL},
	{"ZOMBIE_PAIL",Lawn::ZOMBIE_PAIL},
	{"ZOMBIE_TRAFFIC_CONE",Lawn::ZOMBIE_TRAFFIC_CONE},
	{"ZOMBIE_DOOR",Lawn::ZOMBIE_DOOR},
};

std::unordered_map<sgf::String, Lawn::SeedType> gPlantStringMap = {
	{"SEED_PEASHOOTER",Lawn::SEED_PEASHOOTER},
	{"SEED_SUNFLOWER",Lawn::SEED_SUNFLOWER},
};

void Lawn::Board::LoadLevelFromJson(const nlohmann::json& json)
{
	mShowCutscene = (bool(json["AllowSeedChoose"]) || bool(json["AllowCutScene"]));
	mChooseCard = bool(json["AllowSeedChoose"]);
	mLevel.mLevelName = json["LevelName"];
	mLevel.mWavesNum = json["WavesNumber"];
	mLevel.mHugeWaveScale = json["HugeWaveScale"];
	mStartSpawningZombie = !GetIsShowingCutscene();

	nlohmann::json autoLoadZombieArray = json["AutoLoadZombie"];
	nlohmann::json autoLoadCardArray = json["AutoLoadCard"];

	nlohmann::json zombieArray = json["ZombieList"];
	for (auto& x : autoLoadZombieArray)
	{
		ZombieType type = gZombieStringMap[x];
		mLevel.mZombieTypes.push_back(type);
	}
	for (auto& x : autoLoadCardArray)
	{
		SeedType type = gPlantStringMap[x];
		mSeedBank->AppendSeedPack(type);
	}

	AutoSpawnZombieWaves(mLevel.mWavesNum);

	for (auto& x : zombieArray)
	{
		ZombieType type = gZombieStringMap[x["Type"]];
		int row = x["Row"];
		int column = x["Column"];
		int currentWave = x["CurrentWave"];
		mLevel.mZombieWaves[currentWave - 1].AppendZombie(type, row, column);
		//std::cout << x["Type"] << " " << row << " " << column << std::endl;
	}

	if (!mStartSpawningZombie)
	{
		gLawnApp->mMusicManager.FadeInMusic("MUSIC_LAWNBGM(6)", 5000);
		TryShowCutSceneBegin();
	}
	else {
		gLawnApp->mMusicManager.FadeInMusic("MUSIC_LAWNBGM(13)", 5000);
	}
}

#include <fstream>
#include <EffectHolder.h>
#include "../Resources.h"
#include "GamePacker/GamePacker.h"

void Lawn::Board::LoadLevelFromJsonFile(const char* path)
{
	nlohmann::json result = FileManager::TryToLoadJsonFile(path);
	LoadLevelFromJson(result);
	
}

void Lawn::Board::DrawLevelInfo(sgf::Graphics* g)
{
	if (!mLevelNameImage) {
		sgf::Font* levelFont = RES_FONT::FONT_FONT3;
		levelFont->SetFontSize(30);
		mLevelNameImage = levelFont->GenTextImage(mLevel.mLevelName);
	}
	g->ModelMoveTo(0, 0);
	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - mLevelNameImage->GetWidth()) / 2 + 2, 50);
	g->SetCubeColor({ 0,0,0,1 });
	g->DrawImage(mLevelNameImage);

	g->Translate(-2, -2);
	g->SetCubeColor({ 0.96f,0.97f,0.73f,1.0f });
	g->DrawImage(mLevelNameImage);

	sgf::SimpleImage* processBarImage = RES_IMAGE::IMAGE_FLAGMETER;
	sgf::SimpleImage* processBarPartsImage = RES_IMAGE::IMAGE_FLAGMETERPARTS;

	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2, 20);
	g->SetCubeColor({ 1,1,1,1 });
	g->DrawImageGridAtlas(processBarImage,2,1,0,0);

	g->SetClipRect({ (LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2,20,processBarImage->GetWidth() * mFlagMeterIndex / float(mLevel.mZombieWaves.size()),processBarImage->GetHeight()});
	
	g->MoveTo(0,0);
	g->DrawImageGridAtlas(processBarImage,2,1,1,0);

	g->ClearClipRect();

	g->MoveTo((LAWN_GAME_WINDOW_WIDTH - processBarImage->GetWidth()) / 2 + processBarImage->GetWidth() * mFlagMeterIndex / float(mLevel.mZombieWaves.size()) - 5, 20);
	g->DrawImageGridAtlas(processBarPartsImage, 1, 2, 0, 0);

	int hugeWaveNum = mLevel.mZombieWaves.size() / BOARD_WAVE_EACH_HUGE_EAVE;
	float distanceEachFlag = processBarImage->GetWidth() * BOARD_WAVE_EACH_HUGE_EAVE / float(mLevel.mZombieWaves.size());
	for (int i = 1; i <= hugeWaveNum; i++)
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

void Lawn::Board::ShowSeedChooseScreen()
{
	if (!mSeedChooseScreen) {
		mSeedChooseScreen = new SeedChooseScreen();
		gLawnApp->SafeAppendWidget(mSeedChooseScreen);
		mSeedChooseScreen->ShowScreen();
	}
}

void Lawn::Board::EndCutsence()
{
	//mStartSpawningZombie = true;
	mSeedBank->mVisible = true;
	mStartReadySetPlantAnim = new sgf::Animator(RES_RAXML::RAXML_STARTREADYSETPLANT);
	mStartReadySetPlantAnim->mSpeed = 1.0f;
	mStartReadySetPlantAnim->Play(sgf::Animator::PlayState::PLAY_ONCE);
	gLawnApp->mMusicManager.PlayChunk("CHUNK_READYSETPLANT");
}

void Lawn::Board::EndReady()
{
	mStartSpawningZombie = true;
	gLawnApp->mMusicManager.FadeOutMusic(2000);
	FadeInBackgroundMuisc();

	for (auto& x :mViewZombieVector)
	{
		delete x;
	}
	mViewZombieVector.clear();

}

void Lawn::Board::FadeInBackgroundMuisc()
{
	switch (mBackGroundType)
	{
	case Lawn::BACKGROUND_FRONT_YARD_DAY:
		gLawnApp->mMusicManager.FadeInMusic("MUSIC_LAWNBGM(1)", 5000);
		break;
	}
}

bool Lawn::Board::GetIsShowingCutscene() const
{
	return mShowCutscene;
}

bool Lawn::Board::GetWillChooseCard() const
{
	return mChooseCard;
}

void Lawn::Board::SpawnZombiesInView()
{
	for (auto& x : mLevel.mZombieTypes)
	{
		int length = Rand(1,4);
		for (size_t i = 0; i < length; i++)
		{
			Zombie* viewZom = new Zombie();
			viewZom->MoveTo(RandF(1100, 1300), RandF(0, 550));
			viewZom->mZombieType = x;
			viewZom->mBoard = this;
			viewZom->Init();
			viewZom->PlayIdleTrack();
			mViewZombieVector.push_back(viewZom);
		}
	}

}

void Lawn::Board::Update()
{
	mCutSenceHolder.Update();
	if (mBlackScreenCounter > mTick.GetDeltaTick())
		mBlackScreenCounter -= mTick.GetDeltaTick();
	else
		mBlackScreenCounter = 0;

	if (mHugeCounter > mTick.GetDeltaTick())
		mHugeCounter -= mTick.GetDeltaTick();
	else
		mHugeCounter = 0;

	if (mStartReadySetPlantAnim) {
		mStartReadySetPlantAnim->Update();
		if (!mStartReadySetPlantAnim->mIsPlaying) {
			delete mStartReadySetPlantAnim;
			mStartReadySetPlantAnim = nullptr;
			EndReady();
		}
	}

	if (!mIsBoardRunning) {
		mWinZombie->Update();
		if(mBlackScreenCounter <= 0)
			mZombieAnimator->Update();
		return;
	}

	size_t length = mPlantVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mPlantVector[i]->mAvailable) {
			delete mPlantVector[i];
			mPlantVector.erase(mPlantVector.begin() + i);
			i--;
			length--;
			continue;
		}
		mPlantVector[i]->Update();
	}

	length = mSceneObjectVector.size();
	for (size_t i = 0; i < length; i++)
	{
		if (!mSceneObjectVector[i]->mAvailable) {
			delete mSceneObjectVector[i];
			mSceneObjectVector.erase(mSceneObjectVector.begin() + i);
			i--;
			length--;
			continue;
		}
		mSceneObjectVector[i]->Update();
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

		mZombieVector[i]->Update();
	}

	length = mViewZombieVector.size();
	for (size_t i = 0; i < length; i++)
	{
		mViewZombieVector[i]->Update();
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
		mProjectileVector[i]->Update();
	}

	if (mFlagMeterIndex < float(mCurrentWaveIndex))
		mFlagMeterIndex += float(mTick.GetDeltaTick()) / 200.0f;


	mParticleManager.Update(mTick.GetDeltaTick());
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

	for (auto& x : mSceneObjectVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
		g->MoveTo(0, 0);
		x->Draw(g);
	}

	for (auto& x : mZombieVector)
	{
		g->ModelMoveTo(boardPos.first + x->mBox.mX, boardPos.second + x->mBox.mY);
		g->MoveTo(0, 0);
		x->Draw(g);
	}


	for (auto& x : mViewZombieVector)
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
			sgf::Font* levelFont = RES_FONT::FONT_BASEFONT;
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

	if (mStartReadySetPlantAnim)
	{
		g->ModelMoveTo(
			(LAWN_GAME_WINDOW_WIDTH - 200.0f) / 2.0f,
			(LAWN_GAME_WINDOW_HEIGHT - 100.0f) / 2.0f);
		g->MoveTo(0, 0);
		mStartReadySetPlantAnim->Present(g);
	}

}

void Lawn::Board::OnClick(int theId)
{
	if (theId == mId && gLawnApp->mCursor->mState == LAWN_CURSOR_SEED) {
		gLawnApp->mCursor->SeedPlant();
	}

	switch (theId)
	{
	case LAWN_WIDGET_BUTTON_RETURN_TO_MENU:
		gLawnApp->KillBoard();
		gLawnApp->EnterGameSelector();
		break;
	case LAWN_WIDGET_BUTTON_PAUSE:
		gLawnApp->GamePause();
		break;
	case LAWN_SEED_BANK:
		mSeedBank->ClickOn();
		break;
	default:
		break;
	}


	if (mSeedChooseScreen) {
		int ctr = 0;
		auto widgetRect = mSeedChooseScreen->GetExactPosition();

		SeedView* seedPair = nullptr;

		for (auto& x : mSeedChooseScreen->mSeeds)
		{
			FloatRect seedRect = { widgetRect.first - 100,widgetRect.second + ctr * 55 - 5,100,55 };
			bool isCover = seedRect.IsPointIn(gLawnApp->mMouseX, gLawnApp->mMouseY);
			if (isCover)
			{
				seedPair = x.mViewPointer;
				break;
			}
			ctr++;
		}

		if (seedPair)
		{
			auto pos = std::find_if(mSeedChooseScreen->mSeeds.begin(), mSeedChooseScreen->mSeeds.end(),
				[seedPair](const SeedPair& pair)->bool {
					return pair.mViewPointer == seedPair; });
			pos->mViewPointer->mIsChosen = false;
			mSeedChooseScreen->mSeedsNumber -= 1;
			mSeedChooseScreen->mSeeds.erase(pos);
		}
		
	}
	
}
