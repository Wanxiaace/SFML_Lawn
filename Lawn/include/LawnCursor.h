#pragma once
#ifndef __LAWN_CURSOR__
#define __LAWN_CURSOR__

#include "../Board/Board.h"
#include "../Board/include/SeedPack.h"

#include "Common.h"
#include "CursorObject.h"

namespace Lawn{
	class SeedPack;
	class Board;


	enum LawnCursorState {
		LAWN_CURSOR_NONE,
		LAWN_CURSOR_SEED,
	};

	class LawnCursor :public sgf::CursorObject {
	public:
		LawnCursorState mState;
		SeedType mSeed;
		SeedPack* mSeedPack = nullptr;
		Board* mBoard = nullptr;
		bool mIsOnBoard = false;

	public:
		LawnCursor();
		~LawnCursor();

		void PickSeedPack(SeedPack* pack);
		void Reset();

		void MouseRightOn();
		void SeedPlant();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;
	};
}

#endif