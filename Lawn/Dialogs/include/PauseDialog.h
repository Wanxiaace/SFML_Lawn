#pragma once
#ifndef __LAWN_PARSE_DIALOG__
#define __LAWN_PARSE_DIALOG__

#include "../../include/BasicWidgets.h"
#include "Graphics.h"

namespace Lawn {

	class PauseDialog : public LawnDialog,sgf::WidgetListener {
	public:
		enum {
			PAUSE_DIALOG_CONTINUE = 100,
			PAUSE_DIALOG_MENU = 101,
		};

	public:
		LawnImageButton* mContinueButton = nullptr;
		LawnStoneButton* mMenuButton = nullptr;

	public:
		PauseDialog();
		~PauseDialog();

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;

		virtual void OnClick(int buttonId) override;
	};
}

#endif // !__LAWN_PARSE_DIALOG__
