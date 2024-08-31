#pragma once
#ifndef __SIMPLE_APP__
#define __SIMPLE_APP__
#include "Common.h"
#include "../../glm/ext/matrix_float4x4.hpp"
#include "GameMessage.h"

namespace sgf {
	class SimpleApp {
	public:
		void (*mDisplay)(SimpleApp*,int) = nullptr;
		void (*mCallback)(SimpleApp*, SDL_Event&) = nullptr;

	public:
		int mWidth = 0;
		int mHeight = 0;
		bool mIsOpen = false;
		bool mShowDebugWindow = false;
		int mMouseX = 0;
		int mMouseY = 0;
		float mMouseXScale = 1.0f;
		float mMouseYScale = 1.0f;
		unsigned int mDeltaTime = 0;

		unsigned int mLastSecondBuffer = 0;
		unsigned int mFramePerSecondBuffer = 0;
		unsigned int mFPS = 0;
		
		bool mIsMouseLeftDown = false;
		bool mIsMouseMiddleDown = false;
		bool mIsMouseRightDown = false;

		int mTextureNumberMax = 0;
		SDL_Window* mGameWindow;
		GameMessageManager mMessageManager;
		SDL_GLContext mGLContext;
		glm::mat4x4 mViewMatrix;
		bool mEnabledASync = false;

	public:
		SimpleApp(int width, int height, const sgf::String& windowCaptain,bool enabledASync=true, bool resiziable = false);
		~SimpleApp();

		void EnterMainLoop();
		void SetDisplayFunction(void (*display)(SimpleApp*, int));
		void SetCallBackFunction(void (*callback)(SimpleApp*, SDL_Event&));
		virtual void CopeEvent(SDL_Event& theEvent);

		virtual void Update();
		virtual void Draw();
	};
}

#endif // !__SIMPLE_APP__
