#include "LawnApp.h"
#include "Constant.h"
#include "Dialogs/SettingDialog.h"
#include "Board/Plant.h"
#include "Board/Zombie.h"
#include "Board/Projectile.h"
#include <mutex>
#include <ctime>

Lawn::LawnApp* gLawnApp = nullptr;

Lawn::LawnApp::LawnApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync, bool resiziable) : GameApp(width, height, windowCaptain, enabledASync, resiziable)
{
    Plant::InitPlantsDefinitions();
    Zombie::InitZombiesDefinitions();
    Projectile::InitProjectilesDefinitions();

    sgf::SRand(time(nullptr));

    mCursor = new LawnCursor();
}

Lawn::LawnApp::~LawnApp()
{
	GameApp::~GameApp();
	if (mLoadingPage)
		KillLoadingPage();

}


void Lawn::LawnApp::EnterLoadingPage()
{
	if (!mLoadingPage) {
		mLoadingPage = new LoadingPage(LAWN_WIDGET_LOADING_PAGE_ID);
		SafeAppendWidget(mLoadingPage);
	}
}

void Lawn::LawnApp::KillLoadingPage()
{
	if (mLoadingPage) {
		SafeDeleteWidget(mLoadingPage);
		mLoadingPage = nullptr;
	}
}


void Lawn::LawnApp::ShowSettingDialog()
{
    if (!mSettingDialog) {
        mSettingDialog = new SettingDialog();
        SafeAppendWidget(mSettingDialog);
        mMusicManager.PlayChunk("CHUNK_TAP2");
    }
}

void Lawn::LawnApp::EnterGameSelector()
{
    if (!mGameSelector) {
        mGameSelector = new GameSelector(LAWN_WIDGET_GAME_SELECTOR_ID, true);
        SafeAppendWidget(mGameSelector);
        mMusicManager.PlayMusic("MUSIC_LAWNBGM(8)");
    }
}

void Lawn::LawnApp::KillGameSelector()
{
    if (mGameSelector) {
        SafeDeleteWidget(mGameSelector);
        mGameSelector = nullptr;
    }
}

void Lawn::LawnApp::EnterGameBoard()
{
    if (!mBoard) {
        MakeNewBoard();
        SafeAppendWidget(mBoard);
    }
}

void Lawn::LawnApp::MakeNewBoard()
{
    mBoard = new Board();
}

void Lawn::LawnApp::KillBoard()
{
    if (mBoard) {
        SafeDeleteWidget(mBoard);
        mBoard = nullptr;
    }
}

void Lawn::LawnApp::GamePause()
{
    if (!mPauseDialog) {
        mPauseDialog = new PauseDialog();
        SafeAppendWidget(mPauseDialog);
    }
}

void Lawn::LawnApp::KillGamePause()
{
    if (mPauseDialog) {
        SafeDeleteWidget(mPauseDialog);
        mPauseDialog = nullptr;
    }
}

#include "res/resource.h"

void Lawn::LawnApp::WindowsEnhance()
{
#ifdef _WIN32

#endif
}

void Lawn::LawnApp::LoadPlayerInfo(const sgf::String& path)
{
    mPlayerInfo.TryLoadFromFile(path);
}


void Lawn::LawnApp::CopeEvent(SDL_Event& _event)
{
    GameApp::CopeEvent(_event);
}

void Lawn::LawnApp::Draw()
{
    mGraphics->Clear();

    mGraphics->ActiveTextureShader();
    mWidgetManager->Draw(mGraphics);
    mCursor->Draw(mGraphics);
    mGraphics->Present();

    DrawImgui();
    SDL_GL_SwapWindow(mGameWindow);
}

void Lawn::GameUpdateThread()
{
    gLawnApp->mTick.Update();
    gLawnApp->Update();

    while (true)
    {
        gLawnApp->mTick.Update();

        gLoopMutex.lock();
        gLawnApp->Update();
        gLawnApp->mCursor->Update();
        if (gLawnApp->mIsMouseRightDown)
            gLawnApp->mCursor->MouseRightOn();
        gLoopMutex.unlock();
        
        SDL_Delay(10);
    }
}
