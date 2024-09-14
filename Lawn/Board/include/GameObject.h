#pragma once

#ifndef __LAWN_BOARD_GAMEOBEJCT__
#define __LAWN_BOARD_GAMEOBEJCT__

#include "Common.h"
#include "Graphics.h"

namespace Lawn {
	class GameObject {
	public:
		sgf::FloatRect mBox;
		unsigned int mTickDelta = 0;

	public:
		const sgf::FloatRect& GetHitBox() const { return mBox; };

	public:
		virtual void Update() {};
		virtual void Draw(sgf::Graphics* g) {};

		virtual void MoveTo(float x, float y) { mBox.mX = x; mBox.mY = y; };
	};
}

#endif