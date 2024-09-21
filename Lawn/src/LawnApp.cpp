#include "../include/LawnApp.h"
#include "../include/Constant.h"
#include "../Dialogs/include/SettingDialog.h"
#include "../Board/include/Plant.h"
#include "../Board/include/Zombie.h"
#include "../Board/include/Projectile.h"
#include <mutex>

Lawn::LawnApp* gLawnApp = nullptr;
std::thread* gUpdateThread = nullptr;
std::mutex gLoopMutex;

static unsigned int m_deltaTimeTick;

Lawn::LawnApp::LawnApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync, bool resiziable) : GameApp(width, height, windowCaptain, enabledASync, resiziable)
{
    Plant::InitPlantsDefinitions();
    Zombie::InitZombiesDefinitions();
    Projectile::InitProjectilesDefinitions();
#ifdef _WIN32
    sgf::SRand(sgf::TryGetTicks());
#endif
    mCursor = new LawnCursor();
    mCursor->AttachApp(this);
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

        gLoopMutex.lock();
        //SDL_ShowCursor(SDL_DISABLE);
        Draw();
        
        mMessageManager.CopeAllMessage();


        gLoopMutex.unlock();

        m_deltaTimeTick = tick;

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
	sgf::GameApp::CopeEvent(_event);
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
    app->Update();
    app->LoadResources((app->mResourceManager.mBasePath + "ResourceList.xml").c_str());
    app->KillLoadingPage();
    app->EnterGameSelector();

    while (true)
    {
        gLoopMutex.lock();
        app->Update();
        app->mCursor->Update();
        if (app->mIsMouseRightDown)
            app->mCursor->MouseRightOn();
        gLoopMutex.unlock();
        
        SDL_Delay(10);
    }
}
