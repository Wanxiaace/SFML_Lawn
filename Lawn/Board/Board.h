#ifndef __LAWN_BOARD__
#define __LAWN_BOARD__

#include "Common.h"
#include "Graphics.h"
#include "SimpleImage.h"
#include "WidgetSystem.h"
#include "../BasicWidgets.h"
#include "ObjectArray.h"
#include "Particle.h"
#include "SeedPack.h"
#include "../Constant.h"
#include "EffectHolder.h"
#include "SeedChooseScreen.h"


namespace Lawn {
	class SeedBank;


	enum BackGroundType {
		BACKGROUND_FRONT_YARD_DAY,
		BACKGROUND_FRONT_YARD_NIGHT,
	};

	struct BackstageZombie {
		ZombieType mZombieType = ZOMBIE_INVALID;
		int mTargetRow = -1;
		int mTargetColumn = -1;
	};

	class ZombieWave {
	public:
		int mZombieNum = 0;
		std::vector<BackstageZombie> mZombies;
		int mSleepTime = 0;

	public:
		ZombieWave() {};
		~ZombieWave() {};

		void AppendZombie(ZombieType type, int row, int column) { 
			mZombies.push_back({type,row,column}); 
			mZombieNum += 1;
		};

		void SetSleepTime(int sleepTime) { mSleepTime = sleepTime; };
	};

	struct LevelInfo {
		sgf::String mLevelName = u8"测试关卡";
		std::vector<ZombieWave> mZombieWaves;
		std::vector<ZombieType> mZombieTypes;
		int mWavesNum = 0;
		float mHugeWaveScale = 1.0f;
	};

	class Board : public sgf::Widget,public sgf::WidgetListener {
	public:
		sgf::SimpleImage* mBackGroundImageCache;
		BackGroundType mBackGroundType;
		LawnStoneButton* mDefeatReturnToMenuButton = nullptr;
		SeedBank* mSeedBank = nullptr;
		sgf::Animator* mStartReadySetPlantAnim = nullptr;
		SeedChooseScreen* mSeedChooseScreen = nullptr;

		sgf::ParticleManager mParticleManager;

		PlantVector mPlantVector;
		ZombieVector mZombieVector;
		ZombieVector mViewZombieVector;
		ProjectileVector mProjectileVector;
		LawnMoverVector mLawnMoverVector;
		SceneObejctVector mSceneObjectVector;
		LevelInfo mLevel;

		float mBackgroundScaleF;
		float mHugeWaveScaleF = BOARD_SCALE_EACH_HUGE_WAVW;//大波僵尸时出怪数扩大的因数

		int mGridOriPosX = 0;
		int mGridOriPosY = 0;

		int mGridWidth = 0;
		int mGridHeight = 0;
		bool mIsBoardRunning = true;
		Zombie* mWinZombie = nullptr;
		sgf::Animator* mZombieAnimator = nullptr;
		sgf::SimpleImage* mLevelNameImage = nullptr;
		sgf::SimpleImage* mHugeTitleImage = nullptr;
		bool mIsZombieWin = false;
		int mSun = 0;

		//开始出怪
		bool mStartSpawningZombie = false;
		//下一波的倒计时，场上没有僵尸会瞬间变小
		int mNextWaveCounts = -1;
		//当前波数索引
		int mCurrentWaveIndex = 0;

		float mFlagMeterIndex = 0;

		float mBlackScreenCounter = 0;
		float mHugeCounter = 0;

		unsigned int mPlantRandomSeed = 0;//随机数种子
		unsigned int mZombieRandomSeed = 0;//随机数种子

		sgf::EffectHolderFloat mCutSenceHolder{};
		bool mShowCutscene = false;
		bool mChooseCard = false;

	public:
		Board();
		~Board();

		void UpdateBoardBackground();
		void TryShowCutSceneBegin();
		void DrawBackDrop(sgf::Graphics* g) const;
		int PointXtoGridX(int pointX) const;
		int PointYtoGridY(int pointY) const;

		int GridXtoPointX(int gridX) const;
		int GridYtoPointY(int gridY) const;

		Plant* SpawnPlantAt(int gridX,int gridY, SeedType seedType);
		Zombie* SpawnZombieAt(int gridX,int gridY, ZombieType zomType);
		SceneObejct* SpawnSceneObjectAt(int x,int y, SceneObjectType objType);
		Projectile* SpawnProjectileAt(int x,int y, ProjectileType projectileType);
		sgf::Particle* SpawnParticleAt(sgf::Emitter* emitter,int x,int y,int z);
		void SpawnParticlesAt(sgf::Emitter* emitter,int number,int x,int y,int z);
		sgf::AnimatorParticle* SpawnParticleReanimAt(sgf::Reanimation* anim, float x, float y,float scale);
		
		LawnMover* SpawnLawnMoverAt(int gridX, int gridY, LawnMoverType carType);

		void ZombieWin(Zombie* target);
		int GetCurrentZombieNum() const;
		void AutoSpawnZombieWaves(int waveMax=10);
		void UpdateZombieWaves();
		
		void LoadLevelFromJson(const nlohmann::json& json);
		void LoadLevelFromJsonFile(const char* path);

		void DrawLevelInfo(sgf::Graphics* g);
		void InitLawnMover();
		void ShowSeedChooseScreen();
		void EndCutsence();
		void EndReady();
		void FadeInBackgroundMuisc();
		bool GetIsShowingCutscene() const;
		bool GetWillChooseCard() const;
		void SpawnZombiesInView();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;

		virtual void OnClick(int theId) override;
	};
}
#endif