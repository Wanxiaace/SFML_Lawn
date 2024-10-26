
#ifndef __LAWN_GAME_SELECTOR__
#define __LAWN_GAME_SELECTOR__
#include "Common.h"
#include "WidgetSystem.h"
#include "Graphics.h"
#include "GameApp.h"
#include "BasicWidgets.h"

namespace Lawn {
	class GameSelector :public sgf::Widget, sgf::WidgetListener {
	public:
		bool mPlayingEntryAnimation = false;
		unsigned int mAnimeTicker = 0;
		unsigned int mTickerCache = 0;
		TextButton* mStartButton = nullptr;
		TextButton* mConfigButton = nullptr;
		TextButton* mExitButton = nullptr;

	public:
		GameSelector(int theId,bool useEntryAnimation = false);
		~GameSelector();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;

		virtual void OnClick(int buttonId) override;
	};
}

#endif