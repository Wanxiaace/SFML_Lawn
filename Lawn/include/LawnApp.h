#pragma once
#ifndef __LAWN_APP__
#define __LAWN_APP__
#include "Common.h"
#include "GameApp.h"
#include "LoadingPage.h"
#include <thread>
#include "GameSelector.h"
#include "../Dialogs/include/SettingDialog.h"
#include "../Board/Board.h"
#include "PlayerInfo.h"
#include "LawnPlayerInfo.h"
#include "LawnCursor.h"



namespace Lawn {
	class Board;
	class LawnCursor;

	class LawnApp : public sgf::GameApp {
	public:
		LoadingPage* mLoadingPage = nullptr;
		GameSelector* mGameSelector = nullptr;
		SettingDialog* mSettingDialog = nullptr;
		Board* mBoard = nullptr;
		LawnCursor* mCursor;

		LawnPlayerInfo mPlayerInfo;

	public:
		LawnApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync = true, bool resiziable = false);
		~LawnApp();

		void LawnStart();
		void EnterLoadingPage();
		void KillLoadingPage();

		void ShowSettingDialog();

		void EnterGameSelector();
		void KillGameSelector();

		void EnterGameBoard();
		void MakeNewBoard();
		void KillBoard();

		void LoadPlayerInfo(const sgf::String& path);
		

	public:
		virtual void CopeEvent(SDL_Event& _event) override;
		virtual void Draw() override;
	};

	static void GameUpdateThread(LawnApp* app);
}

extern Lawn::LawnApp* gLawnApp;//全局App
extern std::thread* gUpdateThread;
extern std::mutex gLoopMutex;//全局Mutex

#define DO_IN_GRAPHICS_BEGIN gLawnApp->DoInGraphicsThread(new std::function<void()>([]()->void{
#define DO_IN_GRAPHICS_END }));


#endif