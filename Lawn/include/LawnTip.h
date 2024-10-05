#pragma once
#ifndef __LAWN_TIP__
#define __LAWN_TIP__

#include "WidgetSystem.h"
#include "Graphics.h"

namespace Lawn {
	enum TipLocation {
		TIPLOC_NORMAL,
		TIPLOC_TRACK_MOUSE,
		TIPLOC_TOP_CENTER,
		TIPLOC_RIGHT_CENTER,
	};

	enum TipStyle {
		TIPSTYLE_FADE = 0b00000001,
	};


	class ToolTip : public sgf::Widget {
	public:
		sgf::String mLabel = "";
		sgf::SimpleImage* mLabelImage = nullptr;
		TipLocation mLocation = TIPLOC_NORMAL;
		sgf::Color mColor = { 1,1,1,1 };
		unsigned int mStyle = 0;
		float mLifeTime = 0;
		bool mCostLifeTime = true;

	public:
		ToolTip(int id);
		~ToolTip();

		void Init(const sgf::String& label, unsigned int style, TipLocation location,float lifeTime = 3000);
		void LoadLabel(const sgf::String& label);
		void InitCurrentLoc();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;

	};

}

#endif // 
