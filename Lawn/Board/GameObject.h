

#ifndef __LAWN_BOARD_GAMEOBEJCT__
#define __LAWN_BOARD_GAMEOBEJCT__

#include "Common.h"
#include "TickCounter.h"
#include "Graphics.h"
#include "json.hpp"

namespace Lawn {

	class GameObject {
	public:
		sgf::FloatRect mBox;
		sgf::TickCounter mTick;
		unsigned int mCreatedTick = 0;//记录出现时的tick

	public:
		GameObject();
		const sgf::FloatRect& GetHitBox() const { return mBox; };

	public:
		virtual void Update() { mTick.Update(); };
		virtual void Draw(sgf::Graphics* g) {};

		virtual void MoveTo(float x, float y) { mBox.mX = x; mBox.mY = y; };
		virtual void ParseToJson(nlohmann::json& dstJson) {};
		virtual void UnparseFromJson(nlohmann::json& srcJson) {};

		virtual ~GameObject() {};
	};
}

#endif