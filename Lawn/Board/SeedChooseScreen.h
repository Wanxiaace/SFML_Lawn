#ifndef __LAWN_SEED_CHOOSE_SCREEN__
#define __LAWN_SEED_CHOOSE_SCREEN__

#include "WidgetSystem.h"
#include "EffectHolder.h"
#include "Graphics.h"
#include "BoardEnums.h"
#include "Common.h"
#include <vector>

namespace Lawn {
	class SeedChooseScreen;

	class SeedView : public sgf::Widget {
	public:
		SeedType mSeedType = SEED_NONE;
		SeedChooseScreen* mScreen = nullptr;
		sgf::EffectHolderFloat mBounceHolder{};
		float mOriY = 0;
		float mBounceScale = 1.0f;
		bool mIsChosen = false;

	public:
		SeedView(SeedType type);
		~SeedView();

		void SetOriginY(float y);
		void BindScreen(SeedChooseScreen* ptr);
		void ChooseSeed();
		void Bounce();

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};

	struct SeedPair {
		SeedType mType = SeedType::SEED_NONE;
		SeedView* mViewPointer = nullptr;
	};

	class SeedChooseScreen :public sgf::Widget,sgf::WidgetListener {
	public:
		sgf::EffectHolderFloat mHolder;
		std::vector<SeedView*> mSeedViewArray;
		std::vector<SeedPair> mSeeds;

		int mSeedViewIndexCounter = 0;
		int mSeedsNumber = 0;
		float mSeedViewOffsetY = 0;
		float mSeedViewOffsetYChangeSpeed = 0;
		bool mIsOnChoosing = false;

	public:
		SeedChooseScreen();
		~SeedChooseScreen();

		void ShowScreen();
		void AddSeedView(SeedType type,bool autoAddToManager);

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
		virtual void OnClick(int widgetId);

	};
}


#endif // !__LAWN_SEED_CHOOSE_SCREEN__

