#include "LawnApp.h"
#include "Constant.h"
#include "Dialogs/SettingDialog.h"
#include "Board/Plant.h"
#include "Board/Zombie.h"
#include "Board/Projectile.h"
#include <mutex>
#include <ctime>
#include <chrono>

Lawn::LawnApp* gLawnApp = nullptr;

Lawn::LawnApp::LawnApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync, bool resiziable) : GameApp(width, height, windowCaptain, enabledASync, resiziable)
{
    Plant::InitPlantsDefinitions();
    Zombie::InitZombiesDefinitions();
    Projectile::InitProjectilesDefinitions();

    auto src = 
        std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch());
    sgf::SRand(src.count());

    mCursor = new LawnCursor();
}

Lawn::LawnApp::~LawnApp()
{
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

        if (!mBoardMenuButton)
        {
            mBoardMenuButton = new LawnStoneButton(LAWN_WIDGET_BUTTON_PAUSE);
            mBoardMenuButton->LoadLabel(_LS("Menu"));
            mBoardMenuButton->Resize(LAWN_GAME_WINDOW_WIDTH - 130, 0, 120, 50);
            SafeAppendWidget(mBoardMenuButton);
            mBoardMenuButton->mVisible = true;
            mBoardMenuButton->AttachToListener(mBoard);
        }
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
        SafeDeleteWidget(mBoardMenuButton);
        mBoard = nullptr;
        mBoardMenuButton = nullptr;
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

#include "GamePacker/GamePacker.h"

void Lawn::LawnApp::InitEnv()
{
    gLawnApp->WindowsEnhance();
    gLawnApp->LoadGraphics();
}

std::ofstream gDirayOut;

void Lawn::LawnApp::InitByManifest(const sgf::String& path)
{
    pugi::xml_parse_result result;
    auto xml = FileManager::TryToLoadXMLFile(path,&result);
    if (!result) {
        gGameApp->Log() << "Failed to load manifest at: " << path
            << " with: " << result.description() << std::endl;
        SHOW_ERROR_ABORT_EXIT(("Failed to load manifest at: " + path
            + " with: " + result.description()).c_str());
    }

    for (auto& x : xml.first_child().children())
    {
        //std::cout << x.name() << std::endl;
        const sgf::String str = x.name();
        if (str == "ResourceBasePath") {
            mResourceManager.AttachBasePath(x.text().as_string());
        }
        if (str == "AppDiary") {
#ifndef _DEBUG
            gDirayOut = std::ofstream(x.text().as_string());
            sgf::SetStdOutStream(gDirayOut);
#endif
        }
        if (str == "Dictionay") {
            LoadDict(x.text().as_string());
        }

        if (str == "GameTitle") {
            SetWindowCaptain(x.text().as_string());
        }

        if (str == "PackPath") {
            sgf::FileManager::TryToLoadPak(mResourceManager.mBasePath + x.text().as_string());
        }

        if(str == "EnableVSync")
        {
            mEnabledASync = x.text().as_bool();
        }

        if (str == "CustomFPS")
        {
            mFPSMax = x.text().as_int();
        }
    }

    InitEnv();
}

void Lawn::LawnApp::LoadPlayerInfo(const sgf::String& path)
{
    mPlayerInfo.TryLoadFromFile(path);
}

bool Lawn::LawnApp::IsPlayerSeedUsable(SeedType type)
{
    return true;
}

int Lawn::LawnApp::GetPlayerUsableSeedsTotalNum()
{
    return 10;//NUM_SEEDS_IN_CHOOSER
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
    //gLawnApp->mTick.Update();
    //gLawnApp->Update();

    while (true)
    {
        gLoopMutex.lock();
        gLawnApp->mMouseStyleEach = MOUSE_STYLE_ARROW;
        
        gLawnApp->mTick.Update();
        gLawnApp->Update();
        gLawnApp->mCursor->Update();

        if (gLawnApp->mIsMouseRightDown)
            gLawnApp->mCursor->MouseRightOn();

        sgf::SetMouseStyle(gLawnApp->mMouseStyleEach);

        gLoopMutex.unlock();
        
        SDL_Delay(10);
    }
}
