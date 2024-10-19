#pragma once
#ifndef __LAWN_GAME_BASIC_WIDGETS__
#define __LAWN_GAME_BASIC_WIDGETS__
#include "Common.h"
#include "WidgetSystem.h"
#include "Graphics.h"
#include "Font.h"
#include "Resources.h"

namespace Lawn {
	class TextButton :public sgf::TemplateButton {
	public:
		sgf::SimpleImage* mTextImage = nullptr;
		int mFontSize = 0;

	public:
		TextButton(int buttonId, int fontSize);
		~TextButton();

		void LoadLabel(const sgf::String& text, sgf::Font* font = RES_FONT::FONT_FONT2);
		void MoveTo(float x,float y);
		
	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};

	class LawnStoneButton :public sgf::TemplateButton {
	protected:
		sgf::SimpleImage* mImageLeft = nullptr;
		sgf::SimpleImage* mImageMiddle = nullptr;
		sgf::SimpleImage* mImageRight = nullptr;
		sgf::SimpleImage* mImageLabel = nullptr;
	public:

	public:
		LawnStoneButton(int buttonId);
		~LawnStoneButton();

		void LoadLabel(const sgf::String& label);

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};

	class LawnImageButton : public LawnStoneButton {
	public:
		sgf::SimpleImage* mImageNormal = nullptr;
		sgf::SimpleImage* mImageHover = nullptr;
		sgf::SimpleImage* mImageDown = nullptr;
	public:
		LawnImageButton(int buttonId);
		~LawnImageButton();

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};

	class LawnDialog:public sgf::Widget {
	protected:
		sgf::SimpleImage* mDialogTopLeft = nullptr;
		sgf::SimpleImage* mDialogTopMiddle = nullptr;
		sgf::SimpleImage* mDialogTopRight = nullptr;

		sgf::SimpleImage* mDialogCenterLeft = nullptr;
		sgf::SimpleImage* mDialogCenterMiddle = nullptr;
		sgf::SimpleImage* mDialogCenterRight = nullptr;

		sgf::SimpleImage* mDialogBottomLeft = nullptr;
		sgf::SimpleImage* mDialogBottomMiddle = nullptr;
		sgf::SimpleImage* mDialogBottomRight = nullptr;

		sgf::SimpleImage* mDialogHeader = nullptr;
	public:
		sgf::Point mMouseOriPoint = {};

	public:

		LawnDialog(int theId);
		~LawnDialog();

		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};


	class LawnLabel :public sgf::Widget {
	public:
		sgf::SimpleImage* mLabelImage = nullptr;
		sgf::Font* mFont = nullptr;
		sgf::Color mTextColor = { 1,1,1,1 };

	public:
		LawnLabel();
		~LawnLabel();

		void LoadLabel(const sgf::String& label, int fontSize = 30);
		void SetColor(const sgf::Color& srcColor);

	public:
		virtual void Draw(sgf::Graphics* g) override;

	};


	class LawnSlider :public sgf::Widget {
	public:
		sgf::SimpleImage* mImageSliderBar = nullptr;
		sgf::SimpleImage* mImageSliderButton = nullptr;

		float mRangeMax;
		float mRangeMin;
		float mRangeDistence;

		float mValue;
		float* mTargetFloat = nullptr;


	public:

		LawnSlider(int theId);
		~LawnSlider();

		void SetRange(float max,float min);
		void BindToFloat(float* target);

		void SetValue(float value) { mValue = value; };

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;

	};

	class LawnCheckBox : public sgf::Widget {
	public:
		bool* mTargetBool = nullptr;
		sgf::SimpleImage* mCheckImage = nullptr;
		sgf::SimpleImage* mUnCheckImage = nullptr;
		LawnLabel* mLabel = nullptr;
		bool mIsCheck = false;

	public:
		LawnCheckBox(int theId);
		~LawnCheckBox();

		void SetLabel(const sgf::String& label);
		void BindToBool(bool* boolean) { mTargetBool = boolean; };
		void UpdateState();

	public:
		virtual void Draw(sgf::Graphics* g) override;
		virtual void Update() override;
	};
}

#endif