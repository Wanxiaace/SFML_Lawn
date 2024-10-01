#pragma once
#ifndef __LAWN_BOARD__
#define __LAWN_BOARD__

#include "Common.h"
#include "Graphics.h"
#include "SimpleImage.h"
#include "WidgetSystem.h"
#include "GameApp.h"
#include "../include/BasicWidgets.h"
#include "include/ObjectArray.h"
#include "Particle.h"
#include "include/SeedPack.h"

namespace Lawn {
	class SeedBank;
	class LawnApp;

	enum BackGroundType {
		BACKGROUND_FRONT_YARD_DAY,
		BACKGROUND_FRONT_YARD_NIGHT,
	};

	class Board : public sgf::Widget,sgf::WidgetListener {
	public:
		sgf::SimpleImage* mBackGroundImageCache;
		BackGroundType mBackGroundType;
		LawnStoneButton* mMenuButton = nullptr;
		LawnStoneButton* mDefeatReturnToMenuButton = nullptr;
		SeedBank* mSeedBank = nullptr;

		sgf::ParticleManager mParticleManager;

		PlantVector mPlantVector;
		ZombieVector mZombieVector;
		ProjectileVector mProjectileVector;

		Lawn::LawnApp* mApp;
		float mBackgroundScaleF;

		int mGridOriPosX = 0;
		int mGridOriPosY = 0;

		int mGridWidth = 0;
		int mGridHeight = 0;
		bool mIsBoardRunning = true;
		Zombie* mWinZombie = nullptr;
		sgf::Animator* mZombieAnimator = nullptr;
		bool mIsZombieWin = false;
		float mBlackScreenCounter = 0;

	public:
		Board(LawnApp* app);
		~Board();

		void UpdateBoardBackground();
		void DrawBackDrop(sgf::Graphics* g) const;
		int PointXtoGridX(int pointX) const;
		int PointYtoGridY(int pointY) const;

		int GridXtoPointX(int gridX) const;
		int GridYtoPointY(int gridY) const;

		Plant* SpawnPlantAt(int gridX,int gridY, SeedType seedType);
		Zombie* SpawnZombieAt(int gridX,int gridY, ZombieType zomType);
		Projectile* SpawnProjectileAt(int x,int y, ProjectileType projectileType);
		sgf::Particle* SpawnParticleAt(sgf::Emitter* emitter,int x,int y,int z);
		void SpawnParticlesAt(sgf::Emitter* emitter,int number,int x,int y,int z);
		void ZombieWin(Zombie* target);

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;

		virtual void OnClick(int theId) override;
	};
}
#endif