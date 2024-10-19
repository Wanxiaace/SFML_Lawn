#include "../include/LawnApp.h"
#include "../include/Constant.h"
#include "../Dialogs/include/SettingDialog.h"
#include "../Board/include/Plant.h"
#include "../Board/include/Zombie.h"
#include "../Board/include/Projectile.h"
#include <mutex>
#include <ctime>

Lawn::LawnApp* gLawnApp = nullptr;
std::thread* gUpdateThread = nullptr;
std::mutex gLoopMutex;

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

void Lawn::LawnApp::LawnStart()
{
    Log() << "Game Running" << std::endl;
    Log() << "--------------------------" << std::endl;

    mIsOpen = true;
    mLastSecondBuffer = sgf::TryGetTicks();
    mFramePerSecondBuffer = 0;

    while (mIsOpen)
    {

        SDL_Event _event;

        if (SDL_PollEvent(&_event)) {
            ImGui_ImplSDL2_ProcessEvent(&_event);
            CopeEvent(_event);
            if (mCallback)
                mCallback(this, _event);
        }

        Uint32 tick = sgf::TryGetTicks();

        if (tick - mLastSecondBuffer >= 1000) {
            mLastSecondBuffer = tick;
            mFPS = mFramePerSecondBuffer;
            mFramePerSecondBuffer = 0;
        }
        else {
            mFramePerSecondBuffer++;
        }

        if (mDisplay)
            mDisplay(this, mDeltaFrameTime);

        gLoopMutex.lock();
        //SDL_ShowCursor(SDL_DISABLE);
        Draw();
        
        mMessageManager.CopeAllMessage();


        gLoopMutex.unlock();

        static const Uint32 FPS = 1000 / 100;//可替换为限制的帧速
        static Uint32 _FPS_Timer;
        if (mEnabledASync) {
            if (sgf::TryGetTicks() - _FPS_Timer < FPS) {
                SDL_Delay(FPS - sgf::TryGetTicks() + _FPS_Timer);
            }
            _FPS_Timer = sgf::TryGetTicks();
        }
    }
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
    /*
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 4; j < 8; j++)
        {
            for (size_t k = 0; k < 20; k++)
            {
                switch (sgf::Rand(0, 4)) {
                case 0:
                    mBoard->SpawnZombieAt(j, i, ZOMBIE_NORMAL); break;
                case 1:
                    mBoard->SpawnZombieAt(j, i, ZOMBIE_TRAFFIC_CONE); break;
                case 2:
                    mBoard->SpawnZombieAt(j, i, ZOMBIE_PAIL); break;
                case 3:
                    mBoard->SpawnZombieAt(j, i, ZOMBIE_DOOR); break;
                }
            }
        }
    }*/


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

#include "../resource.h"

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

void Lawn::GameUpdateThread(LawnApp* app)
{
    app->mTick.Update();
    app->Update();

    while (true)
    {
        app->mTick.Update();

        gLoopMutex.lock();
        app->Update();
        app->mCursor->Update();
        if (app->mIsMouseRightDown)
            app->mCursor->MouseRightOn();
        gLoopMutex.unlock();
        
        SDL_Delay(10);
    }
}
