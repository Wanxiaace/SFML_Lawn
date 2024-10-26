

#ifndef __LAWN_BOARD_GAMEMOVER__
#define __LAWN_BOARD_GAMEMOVER__

#include "GameObject.h"
#include "Animator.h"
#include "Zombie.h"

namespace Lawn {
	class LawnMover :public GameObject 
	{
	public:
		sgf::Animator* mLawnMoverAnim = nullptr;
		sgf::SimpleImage* mLawnMoverShadow = nullptr;
		LawnMoverType mType = LAWN_MOVER_NORMAL;
		bool mIsRunning = false;
		bool mAvailable = true;

	public:
		LawnMover();
		~LawnMover();

		void Init(LawnMoverType type);
		Zombie* TryToFindZombie();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;
	};
}

#endif