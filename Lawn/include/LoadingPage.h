#pragma once
#ifndef __LAWN_LOADING_PAGE__
#define __LAWN_LOADING_PAGE__
#include "Common.h"
#include "WidgetSystem.h"
#include "Graphics.h"

namespace Lawn {
	class LoadingPage :public sgf::Widget, sgf::WidgetListener {
	public:
		LoadingPage(int id);
		~LoadingPage();

		virtual void OnClick(int theId) override;
		virtual void Draw(sgf::Graphics* g) override;
	};
}

#endif