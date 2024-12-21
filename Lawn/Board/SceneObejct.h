
#ifndef __LAWN_BOARD_SCENE_OBJECT__
#define __LAWN_BOARD_SCENE_OBJECT__

#include "GameObject.h"
#include "BoardEnums.h"
#include "Board.h"

namespace Lawn {
	class Board;

	enum MotionType {
		MOTION_NORMAL,
		MOTION_FALL,
		MOTION_MOVE_TO,
	};

	class SceneObejct: public GameObject
	{
	public:
		SceneObjectType mType = SCENE_OBJECT_SUN;
		bool mAvailable = true;
		Board* mBoard = nullptr;
		MotionType mMotionType = MOTION_NORMAL;
		Animator* mReanim = nullptr;
		bool mIsMouseHover = false;
		bool mIsCollect = false;
		float mVelZ = 0;
		float mOriY = 0;
		float mTargetX = 0;
		float mTargetY = 0;


	public:
		SceneObejct();
		~SceneObejct();

		void Init();
		void Collect();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;
	};

}

#endif // !__LAWN_BOARD_SCENE_OBJECT__
