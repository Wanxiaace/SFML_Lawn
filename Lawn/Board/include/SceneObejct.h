#pragma once
#ifndef __LAWN_BOARD_SCENE_OBJECT__
#define __LAWN_BOARD_SCENE_OBJECT__

#include "GameObject.h"
#include "BoardEnums.h"
#include "../Board.h"

namespace Lawn {
	class Board;

	enum MotionType {
		MOTION_NORMAL,
		MOTION_FALL,
	};

	class SceneObejct: public GameObject
	{
	public:
		SceneObjectType mType;
		bool mAvailable = true;
		Board* mBoard = nullptr;
		MotionType mMotionType = MOTION_NORMAL;
		Animator* mReanim = nullptr;

	public:
		SceneObejct();
		~SceneObejct();

		void Init();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;
	};

}

#endif // !__LAWN_BOARD_SCENE_OBJECT__
