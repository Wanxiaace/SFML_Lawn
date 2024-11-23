#ifndef __LAWN_SEED_CHOOSE_SCREEN__
#define __LAWN_SEED_CHOOSE_SCREEN__

#include "WidgetSystem.h"
#include "EffectHolder.h"
#include "Graphics.h"
#include "BoardEnums.h"
#include "Common.h"

namespace Lawn {
	class SeedChooseScreen;

	class SeedView : public sgf::Widget {
	public:
		SeedType mSeedType = SEED_NONE;
		SeedChooseScreen* mScreen = nullptr;
		float mOriY = 0;
		bool mIsChosen = false;

	public:
		SeedView(SeedType type);
		~SeedView();

		void SetOriginY(float y);
		void BindScreen(SeedChooseScreen* ptr);
		void ChooseSeed();

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};

	class SeedChooseScreen :public sgf::Widget,sgf::WidgetListener {
	public:
		sgf::EffectHolderFloat mHolder;
		std::vector<SeedView*> mSeedViewArray;
		std::vector<SeedType> mSeeds;

		int mSeedViewIndexCounter = 0;
		int mSeedsNumber = 0;
		float mSeedViewOffsetY = 0;
		float mSeedViewOffsetYChangeSpeed = 0;


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

