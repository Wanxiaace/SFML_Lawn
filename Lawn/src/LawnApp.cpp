#include "../include/LawnApp.h"
#include "../include/Constant.h"
#include "../Dialogs/include/SettingDialog.h"
#include "../Board/include/Plant.h"
#include "../Board/include/Zombie.h"
#include "../Board/include/Projectile.h"
#include <mutex>

Lawn::LawnApp* gLawnApp = nullptr;
std::thread* gUpdateThread = nullptr;
std::mutex gMutex;

static unsigned int m_deltaTimeTick;

Lawn::LawnApp::LawnApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync, bool resiziable) : GameApp(width, height, windowCaptain, enabledASync, resiziable)
{
    Plant::InitPlantsDefinitions();
    Zombie::InitZombiesDefinitions();
    Projectile::InitProjectilesDefinitions();
#ifdef _WIN32
    sgf::SRand(GetTickCount());
#endif
}

Lawn::LawnApp::~LawnApp()
{
	GameApp::~GameApp();
	if (mLoadingPage)
		KillLoadingPage();

}

void Lawn::LawnApp::LawnStart()
{
    m_deltaTimeTick = sgf::TryGetTicks();
    mIsOpen = true;
    mLastSecondBuffer = sgf::TryGetTicks();
    mFramePerSecondBuffer = 0;

    gUpdateThread = new std::thread(GameUpdateThread, this);

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
        mDeltaTime = (tick - m_deltaTimeTick);

        if (tick - mLastSecondBuffer >= 1000) {
            mLastSecondBuffer = tick;
            mFPS = mFramePerSecondBuffer;
            mFramePerSecondBuffer = 0;
        }
        else {
            mFramePerSecondBuffer++;
        }

        if (mDisplay)
            mDisplay(this, mDeltaTime);

        gMutex.lock();
        Draw();
        gMutex.unlock();

        m_deltaTimeTick = tick;

        static const Uint32 FPS = 1000 / 60;//可替换为限制的帧速
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
		mLoadingPage = new LoadingPage(LAWN_WIDGET_LOADING_PAGE_ID,this);
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
        mSettingDialog = new SettingDialog(this);
        SafeAppendWidget(mSettingDialog);
        mMusicManager.PlayChunk("CHUNK_TAP2");
    }
}

void Lawn::LawnApp::EnterGameSelector()
{
    if (!mGameSelector) {
        mGameSelector = new GameSelector(LAWN_WIDGET_GAME_SELECTOR_ID,this, true);
        SafeAppendWidget(mGameSelector);
        mMusicManager.PlayMusic("MUSIC_TH06_B2");
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
    mBoard = new Board(this);
    mBoard->SpawnPlantAt(0,0,SEED_PEASHOOTER);
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 4; j < 8; j++)
        {
            mBoard->SpawnZombieAt(j, i, ZOMBIE_NORMAL);
        }
    }
}

void Lawn::LawnApp::KillBoard()
{
    if (mBoard) {
        SafeDeleteWidget(mBoard);
        mBoard = nullptr;
    }
}


void Lawn::LawnApp::CopeEvent(SDL_Event& _event)
{
	sgf::GameApp::CopeEvent(_event);
}


void Lawn::GameUpdateThread(LawnApp* app)
{
    app->Update();
    app->LoadResources("assets/ResourceList.xml");
    app->KillLoadingPage();
    app->EnterGameSelector();

    while (true)
    {
        gMutex.lock();
        app->Update();
        gMutex.unlock();
        
        Sleep(10);
    }
}
