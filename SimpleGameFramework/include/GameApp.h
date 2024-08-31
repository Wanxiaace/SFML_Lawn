#pragma once
#ifndef __SIMPLE_GAME_APP__
#define __SIMPLE_GAME_APP__

#include "SimpleApp.h"
#include "Graphics.h"
#include "WidgetSystem.h"
#include "MusicSystem.h"
#include "ResourceManager.h"
#include "Dictionary.h"

namespace sgf {
	class GameApp : public SimpleApp {
	public:
		Graphics* mGraphics = nullptr;
		WidgetManager* mWidgetManager = nullptr;
		bool mDragAllowed = false;
		bool mRotateAllowed = false;
		bool mShowWidgetHitBoxAllowed = false;
		float mMovingSpeed = 1.0f;

		ResourceList mResourceList;
		ResourceManager mResourceManager;
		MusicManager mMusicManager;
		Dictionary mDictionary;

		float mMusicVolume = 100.0f;
		

	public:
		GameApp(int width, int height, const sgf::String& windowCaptain, bool enabledASync = true,bool resiziable = false);
		~GameApp();

		void SafeDeleteWidget(Widget* target);
		void SafeAppendWidget(Widget* target);
		void LoadResources(const char* resourcesListPath);
		void LoadDict(const char* dictPath);
		void UpdateMusicVolume() { mMusicManager.SetMusicVolume(mMusicVolume / 100.0f); };

	public:
		virtual void DrawImgui();
		virtual void Update() override;
		virtual void Draw() override;
		virtual void CopeEvent(SDL_Event& theEvent) override;
	};
}

#endif