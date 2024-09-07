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


namespace Lawn {
	class LawnApp : public sgf::GameApp {
	public:
		LoadingPage* mLoadingPage = nullptr;
		GameSelector* mGameSelector = nullptr;
		SettingDialog* mSettingDialog = nullptr;
		Board* mBoard = nullptr;

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
		

	public:
		virtual void CopeEvent(SDL_Event& _event) override;
	};

	static void GameUpdateThread(LawnApp* app);
}

extern Lawn::LawnApp* gLawnApp;//全局App
extern std::thread* gUpdateThread;//全局App


#endif