#pragma once

#include "../Board.h"
#include "BoardEnums.h"
#include "WidgetSystem.h"


namespace Lawn {
	void DrawSeedPack(SeedType type, sgf::Graphics* g, float scaleF);
	void DrawSeedBackgound(SeedPackBackground type, sgf::Graphics* g, float scaleF);
	static SeedPackBackground GetSeedPackBackground(SeedType type);

	class SeedPack {
	public:
		int mCost = 0;
		int mX;
		int mY;
		SeedType mSeedType;
		sgf::SimpleImage* mTextImage = nullptr;
		float mScaleF = 1.0f;

	public:
		SeedPack();
		~SeedPack();

		void Init(SeedType type);

	public:
		void Draw(sgf::Graphics* g);

	};

	class SeedBank : sgf::Widget {
	public:

	public:
		SeedBank();
		~SeedBank();
	};
}