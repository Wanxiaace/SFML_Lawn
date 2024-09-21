#pragma once
#ifndef __LAWN_CONSTANT__
#define __LAWN_CONSTANT__

#define _LS(x) gLawnApp->mDictionary.GetString(x)

constexpr int LAWN_GAME_WINDOW_WIDTH = 1077;
constexpr int LAWN_GAME_WINDOW_HEIGHT = 600;

constexpr int LAWN_WIDGET_LOADING_PAGE_ID = 0;
constexpr int LAWN_WIDGET_GAME_SELECTOR_ID = 1;
constexpr int LAWN_WIDGET_BOARD = 2;

constexpr int LAWN_WIDGET_START_BUTTON = 50;
constexpr int LAWN_WIDGET_CONFIG_BUTTON = 51;
constexpr int LAWN_WIDGET_EXIT_BUTTON = 52;
constexpr int LAWN_WIDGET_STONE_TEST_BUTTON = 53;

constexpr int LAWN_WIDGET_BUTTON_OK = 500;
constexpr int LAWN_WIDGET_BUTTON_CANCEL = 501;
constexpr int LAWN_WIDGET_BUTTON_MENU = 502;

constexpr int LAWN_SEED_BANK= 1000;

constexpr int LAWN_DIALOG_SETTING = 5000;


constexpr int LAWN_WIDGET_STONE_TEST_DIALOG = 2500;

constexpr char LAWN_GAME_WINDOW_CAPTAIN[] = "Demo1 v0.1";

constexpr int DIALOG_HEADER_OFFSET = 45;


constexpr int BOARD_SEED_BAND_SIZE_MAX = 10;



#endif // !__LAWN_CONSTANT__


