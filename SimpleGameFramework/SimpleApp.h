#pragma once
#ifndef __SIMPLE_APP__
#define __SIMPLE_APP__
#include "Common.h"
#include "TickCounter.h"
#include "../../glm/ext/matrix_float4x4.hpp"
#include "GameMessage.h"

namespace sgf {
	class GameAppBase {
	public:
		void (*mDisplay)(GameAppBase*,int) = nullptr;
		void (*mCallback)(GameAppBase*, SDL_Event&) = nullptr;

	public:
		int mWidth = 0;
		int mHeight = 0;
		bool mIsOpen = false;
		bool mShowDebugWindow = false;
		bool mShowAnalyzeWindow = false;
		int mMouseX = 0;
		int mMouseY = 0;
		float mMouseXScale = 1.0f;
		float mMouseYScale = 1.0f;
		TickCounter mTick;

		unsigned int mDeltaFrameTime = 0;

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
		glm::mat4x4 mViewMatrixBuf;
		bool mEnabledASync = false;
		std::ostream* mLog = nullptr;

	public:
		GameAppBase(int width, int height, const sgf::String& windowCaptain,bool enabledASync=true, bool resiziable = false);
		~GameAppBase();

		void EnterMainLoop();
		void SetDisplayFunction(void (*display)(GameAppBase*, int));
		void SetCallBackFunction(void (*callback)(GameAppBase*, SDL_Event&));
		void SetOutStream(std::ostream& src) { mLog = &src; };
		std::ostream& Log() { return *mLog; };

		virtual void CopeEvent(SDL_Event& theEvent);

		virtual void Update();
		virtual void Draw();
	};
}

#endif // !__SIMPLE_APP__
