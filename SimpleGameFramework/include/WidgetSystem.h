#pragma once
#ifndef __SIMPLE_WIDGET_SYSTEM__
#define __SIMPLE_WIDGET_SYSTEM__

#include "Common.h"
#include "SimpleApp.h"
#include "Graphics.h"
#include <vector>
#include <unordered_map>

extern bool gShowWidgetHitBoxAllowed;

namespace sgf {
	class Graphics;
	class WidgetListener;
	class Widget;
	class WidgetManager;

	class WidgetListener {
	public:
		virtual void OnClick(int widgetId) {};
		virtual void MouseHover(int widgetId) {};
		virtual void OnChange(int widgetId) {};//����Slider��TextEdit��Ч
	};

	class WidgetManager {
	public:
		std::vector<Widget*> mWidgets;
		std::unordered_map<int , Widget*> mWidgetMap;

	public:
		void AddWidget(Widget* widget);
		void RemoveWidget(Widget* widget);
		
		Widget* GetWidgetByID(int srcId);
		void ClearWidget();

		void Update(GameAppBase* app);
		void Draw(Graphics* graphics);
	};

	class Widget {
	public:
		FloatRect mRect = {0};
		WidgetListener* mListener = nullptr;
		WidgetManager* mWidgetManager = nullptr;
		Widget* mParent = nullptr;
		std::vector<Widget*> mChilds;
		bool mVisible = true;

		bool mIsMouseHover = false;
		bool mIsPlayHoverSound = false;

		//�Ƿ������ס
		bool mIsMouseCaught = false;
		int mId;

	public:
		Widget(int id) { mId = id; };
		virtual ~Widget() { RemoveAllChild(); };

		void AttachToListener(WidgetListener* target) { mListener = target; };
		void Resize(float x, float y, float width, float height);
		void Resize(FloatRect& rect);

		void MoveToCenter(int screenWidth, int screenHeight);
		void MoveTo(int x, int y) { mRect.mX = x; mRect.mY = y; };
		bool GetVisible();

		void AppendChild(Widget* child);
		void RemoveChild(Widget* child);
		void RemoveAllChild();

		std::pair<int, int> GetExactPosition();
		FloatRect GetExactRect();

	public://���Ա��̳е�����
		virtual void Update() {};
		virtual void Draw(Graphics* graphics) {};

	};

	class TemplateButton: public Widget {
	public:
		sgf::String mLabel;

	public:
		TemplateButton(int buttonId);
		~TemplateButton();
	public:
		virtual void Draw(Graphics* graphics) override;
	};


}


#endif // !__SIMPLE_WIDGET_SYSTEM__
