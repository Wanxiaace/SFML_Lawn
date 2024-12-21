
#ifndef __LAWN_BOARD_SEED_PACK__
#define __LAWN_BOARD_SEED_PACK__


#include "Board.h"
#include "BoardEnums.h"
#include "WidgetSystem.h"
#include <vector>

namespace Lawn {
	class Board;
	void DrawSeedPack(SeedType type, sgf::Graphics* g, float scaleF);
	void DrawSeedPackWithCost(SeedType type, sgf::Graphics* g, float scaleF);
	void DrawSeedBackgound(SeedPackBackground type, sgf::Graphics* g, float scaleF);
	static SeedPackBackground GetSeedPackBackground(SeedType type);

	class SeedPack {
	public:
		int mCost = 0;
		sgf::FloatRect mRect = {};

		SeedType mSeedType = SEED_NONE;
		sgf::SimpleImage* mTextImage = nullptr;
		float mScaleF = 1.0f;
		bool mIsMouseHover = false;
		bool mIsChose = false;
		bool mStartCD = false;
		int mCDMax = 0;
		int mCD = 0;

		sgf::Widget* mParent = nullptr;
		Lawn::Board* mBoard = nullptr;

	public:
		SeedPack();
		~SeedPack();

		void Init(SeedType type, Board* board);

	public:
		void Draw(sgf::Graphics* g);
		void Update();
		void StartCD();

		void OnClick();
	};

	class SeedBank : public sgf::Widget {
	public:
		Board* mBoard = nullptr;
		std::vector<SeedPack> mSeedPacks;

	public:
		SeedBank(Board* board);
		~SeedBank();

		void AppendSeedPack(SeedType type);
		void ClearSeedPack();
		void ClickOn();

		SeedPack& operator [](int index);

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};
}

#endif