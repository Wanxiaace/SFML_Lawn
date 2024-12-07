#include "Resources.h"
#include "LawnApp.h"

using namespace RES_FONT;
Font* RES_FONT::FONT_BASEFONT = nullptr;
Font* RES_FONT::FONT_FONT1 = nullptr;
Font* RES_FONT::FONT_FONT2 = nullptr;
Font* RES_FONT::FONT_FONT3 = nullptr;

using namespace RES_RAXML;
Reanimation* RES_RAXML::RAXML_ZOMBIESWON = nullptr;
Reanimation* RES_RAXML::RAXML_LAWNMOWER = nullptr;
Reanimation* RES_RAXML::RAXML_SUN = nullptr;
Reanimation* RES_RAXML::RAXML_STARTREADYSETPLANT = nullptr;

using namespace RES_PAXML;
Emitter* RES_PAXML::PAXML_PEAPARTICLETHROW = nullptr;
Emitter* RES_PAXML::PAXML_PEASPLATS = nullptr;

using namespace RES_IMAGE;
SimpleImage* RES_IMAGE::IMAGE_FLAGMETER = nullptr;
SimpleImage* RES_IMAGE::IMAGE_FLAGMETERPARTS = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_SLIDERKNOB2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_SLIDERSLOT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_CHECKBOX0 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_CHECKBOX1 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_TITLESCREEN2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_PLANTSHADOW = nullptr;
SimpleImage* RES_IMAGE::IMAGE_PACKET_PLANTS_PLUS = nullptr;
SimpleImage* RES_IMAGE::IMAGE_SEEDPACKETS = nullptr;
SimpleImage* RES_IMAGE::IMAGE_SEEDCHOOSE = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_CONE3 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_CONE2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_BUCKET3 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_BUCKET2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_SCREENDOOR3 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_REANIM_ZOMBIE_SCREENDOOR2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_BACKTOGAMEBUTTON0 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_BACKTOGAMEBUTTON2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_OPTIONS_MENUBACK = nullptr;
SimpleImage* RES_IMAGE::IMAGE_BACKGROUND1 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_BACKGROUND2 = nullptr;
SimpleImage* RES_IMAGE::IMAGE_BUTTON_LEFT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_BUTTON_MIDDLE = nullptr;
SimpleImage* RES_IMAGE::IMAGE_BUTTON_RIGHT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_TOPLEFT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_TOPMIDDLE = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_TOPRIGHT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_CENTERLEFT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_CENTERMIDDLE = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_CENTERRIGHT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_BOTTOMLEFT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_BOTTOMMIDDLE = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_BOTTOMRIGHT = nullptr;
SimpleImage* RES_IMAGE::IMAGE_DIALOG_HEADER = nullptr;
SimpleImage* RES_IMAGE::IMAGE_SEEDCHOOSER_BACKGROUND = nullptr;


void LoadAllResourcesLink()
{
	FONT_BASEFONT = gLawnApp->mResourceManager.GetResourceFast<Font>("FONT_BASEFONT");
	FONT_FONT1 = gLawnApp->mResourceManager.GetResourceFast<Font>("FONT_FONT1");
	FONT_FONT2 = gLawnApp->mResourceManager.GetResourceFast<Font>("FONT_FONT2");
	FONT_FONT3 = gLawnApp->mResourceManager.GetResourceFast<Font>("FONT_FONT3");

	RAXML_ZOMBIESWON = gLawnApp->mResourceManager.GetResourceFast<Reanimation>("RAXML_ZOMBIESWON");
	RAXML_LAWNMOWER = gLawnApp->mResourceManager.GetResourceFast<Reanimation>("RAXML_LAWNMOWER");
	RAXML_SUN = gLawnApp->mResourceManager.GetResourceFast<Reanimation>("RAXML_SUN");
	RAXML_STARTREADYSETPLANT = gLawnApp->mResourceManager.GetResourceFast<Reanimation>("RAXML_STARTREADYSETPLANT");

	PAXML_PEAPARTICLETHROW = gLawnApp->mResourceManager.GetResourceFast<Emitter>("PAXML_PEAPARTICLETHROW");
	PAXML_PEASPLATS = gLawnApp->mResourceManager.GetResourceFast<Emitter>("PAXML_PEASPLATS");
	

	IMAGE_FLAGMETER = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_FLAGMETER");
	IMAGE_FLAGMETERPARTS = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_FLAGMETERPARTS");
	IMAGE_OPTIONS_SLIDERSLOT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_SLIDERSLOT");
	IMAGE_OPTIONS_SLIDERKNOB2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_SLIDERKNOB2");
	IMAGE_OPTIONS_CHECKBOX0 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_CHECKBOX0");
	IMAGE_OPTIONS_CHECKBOX1 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_CHECKBOX1");
	IMAGE_TITLESCREEN2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_TITLESCREEN2");
	IMAGE_PLANTSHADOW = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_PLANTSHADOW");
	IMAGE_PACKET_PLANTS_PLUS = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_PACKET_PLANTS_PLUS");
	IMAGE_SEEDPACKETS = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_SEEDPACKETS");
	IMAGE_SEEDCHOOSE = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_SEEDCHOOSE");
	IMAGE_REANIM_ZOMBIE_CONE3 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_CONE3");
	IMAGE_REANIM_ZOMBIE_CONE2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_CONE2");
	IMAGE_REANIM_ZOMBIE_BUCKET3 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_BUCKET3");
	IMAGE_REANIM_ZOMBIE_BUCKET2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_BUCKET2");
	IMAGE_REANIM_ZOMBIE_SCREENDOOR3 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_SCREENDOOR3");
	IMAGE_REANIM_ZOMBIE_SCREENDOOR2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_REANIM_ZOMBIE_SCREENDOOR2");
	IMAGE_OPTIONS_BACKTOGAMEBUTTON0 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_BACKTOGAMEBUTTON0");
	IMAGE_OPTIONS_BACKTOGAMEBUTTON2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_BACKTOGAMEBUTTON2");
	IMAGE_OPTIONS_MENUBACK = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_OPTIONS_MENUBACK");
	IMAGE_SEEDCHOOSER_BACKGROUND = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_SEEDCHOOSER_BACKGROUND");

	IMAGE_BACKGROUND1 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_BACKGROUND1");
	IMAGE_BACKGROUND2 = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_BACKGROUND2");
	IMAGE_BUTTON_LEFT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_BUTTON_LEFT");
	IMAGE_BUTTON_MIDDLE = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_BUTTON_MIDDLE");
	IMAGE_BUTTON_RIGHT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_BUTTON_RIGHT");
	IMAGE_DIALOG_TOPLEFT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_TOPLEFT");
	IMAGE_DIALOG_TOPMIDDLE = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_TOPMIDDLE");
	IMAGE_DIALOG_TOPRIGHT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_TOPRIGHT");
	IMAGE_DIALOG_CENTERLEFT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_CENTERLEFT");
	IMAGE_DIALOG_CENTERMIDDLE = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_CENTERMIDDLE");
	IMAGE_DIALOG_CENTERRIGHT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_CENTERRIGHT");
	IMAGE_DIALOG_BOTTOMLEFT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_BOTTOMLEFT");
	IMAGE_DIALOG_BOTTOMMIDDLE = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_BOTTOMMIDDLE");
	IMAGE_DIALOG_BOTTOMRIGHT = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_BOTTOMRIGHT");
	IMAGE_DIALOG_HEADER = gLawnApp->mResourceManager.GetResourceFast<SimpleImage>("IMAGE_DIALOG_HEADER");
}
