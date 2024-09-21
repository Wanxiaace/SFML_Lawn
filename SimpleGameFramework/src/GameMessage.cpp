#include "../include/GameMessage.h"
#include "../include/WidgetSystem.h"

void sgf::GameMessageManager::SendGameMessage(GameMessage src)
{
	mMessages.push_back(src);
}

void sgf::GameMessageManager::CopeAllMessage()
{
	size_t length = mMessages.size();
	for (size_t i = 0; i < length; i++)
	{
		switch (mMessages[i].mMessageType)
		{
		case sgf::GameMessage::MessageType::MSG_TYPE_DELETE_WIDGET:
		{
			Widget* widget = (Widget*)mMessages[i].mParamPointer2;
			((WidgetManager*)mMessages[i].mParamPointer1)->RemoveWidget(widget);
			delete widget;
			break;
		}
		case sgf::GameMessage::MessageType::MSG_TYPE_APPEND_WIDGET:
		{
			Widget* widget = (Widget*)mMessages[i].mParamPointer2;
			((WidgetManager*)mMessages[i].mParamPointer1)->AddWidget(widget);
			break;
		}
		case sgf::GameMessage::MessageType::MSG_TYPE_SCREEN_SHOT: {
			GameApp* app = static_cast<GameApp*>(mMessages[i].mParamPointer1);
			auto screenShot = app->mGraphics->CaptureScreen(0, 0, app->mWidth, app->mHeight);
			screenShot->DumpToFiles("ScreenShot.png");
			delete screenShot;
			break;
		}
		case sgf::GameMessage::MessageType::MSG_TYPE_FULL_SCREEN: {
			GameApp* app = static_cast<GameApp*>(mMessages[i].mParamPointer1);
			SDL_SetWindowFullscreen(app->mGameWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;
		}
		case sgf::GameMessage::MessageType::MSG_TYPE_EXIT_FULL_SCREEN: {
			GameApp* app = static_cast<GameApp*>(mMessages[i].mParamPointer1);
			SDL_SetWindowFullscreen(app->mGameWindow, 0);
			break;
		}
		case sgf::GameMessage::MessageType::MSG_TYPE_DO_LATER:
		{
			GameApp* app = static_cast<GameApp*>(mMessages[i].mParamPointer1);
			std::function<void()>* func = (std::function<void()>*)(mMessages[i].mParamPointer2);
			(*func)();
			delete func;
			break;
		}
		default:
			break;
		}
	}
	mMessages.clear();
}

void sgf::GameMessageManager::CopeMessage(int index)
{
}

sgf::GameMessageManager::GameMessageManager()
{
}

sgf::GameMessageManager::~GameMessageManager()
{
}
