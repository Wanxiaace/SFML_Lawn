#pragma once

#include "Common.h"
#include "Graphics.h"

namespace sgf {
	class Graphics;

	class CursorObject {
	public:
		bool mVisible = true;
		SDL_Cursor* mCursor = nullptr;
	public:
		CursorObject();
		~CursorObject();

		void SetVisible(bool visible);

	public:
		virtual void Update();
		virtual void Draw(Graphics* g);
	};
}