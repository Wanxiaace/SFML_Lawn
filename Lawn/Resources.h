
#ifndef __LAWN_APP_RESOURCES__
#define __LAWN_APP_RESOURCES__

#include "SimpleImage.h"
#include "Font.h"
#include "Reanimation.h"
#include "Particle.h"
#include "Common.h"

using namespace sgf;


namespace RES_FONT {
	extern Font* FONT_BASEFONT;
	extern Font* FONT_FONT1;
	extern Font* FONT_FONT2;
	extern Font* FONT_FONT3;
};

namespace RES_IMAGE {
	extern SimpleImage* IMAGE_FLAGMETER;
	extern SimpleImage* IMAGE_FLAGMETERPARTS;
	extern SimpleImage* IMAGE_OPTIONS_SLIDERSLOT;
	extern SimpleImage* IMAGE_OPTIONS_SLIDERKNOB2;
	extern SimpleImage* IMAGE_OPTIONS_CHECKBOX0;
	extern SimpleImage* IMAGE_OPTIONS_CHECKBOX1;
	extern SimpleImage* IMAGE_TITLESCREEN2;
	extern SimpleImage* IMAGE_PLANTSHADOW;
	extern SimpleImage* IMAGE_PACKET_PLANTS_PLUS;
	extern SimpleImage* IMAGE_SEEDPACKETS;
	extern SimpleImage* IMAGE_SEEDCHOOSE;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_CONE3;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_CONE2;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_BUCKET3;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_BUCKET2;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
	extern SimpleImage* IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
	extern SimpleImage* IMAGE_OPTIONS_BACKTOGAMEBUTTON0;
	extern SimpleImage* IMAGE_OPTIONS_BACKTOGAMEBUTTON2;
	extern SimpleImage* IMAGE_OPTIONS_MENUBACK;
	extern SimpleImage* IMAGE_NEWSUNBANK;

	extern SimpleImage* IMAGE_LAWNBAK1;
	extern SimpleImage* IMAGE_BACKGROUND2;
	extern SimpleImage* IMAGE_BUTTON_LEFT;
	extern SimpleImage* IMAGE_BUTTON_MIDDLE;
	extern SimpleImage* IMAGE_BUTTON_RIGHT;
	extern SimpleImage* IMAGE_DIALOG_TOPLEFT;
	extern SimpleImage* IMAGE_DIALOG_TOPMIDDLE;
	extern SimpleImage* IMAGE_DIALOG_TOPRIGHT;
	extern SimpleImage* IMAGE_DIALOG_CENTERLEFT;
	extern SimpleImage* IMAGE_DIALOG_CENTERMIDDLE;
	extern SimpleImage* IMAGE_DIALOG_CENTERRIGHT;
	extern SimpleImage* IMAGE_DIALOG_BOTTOMLEFT;
	extern SimpleImage* IMAGE_DIALOG_BOTTOMMIDDLE;
	extern SimpleImage* IMAGE_DIALOG_BOTTOMRIGHT;
	extern SimpleImage* IMAGE_DIALOG_HEADER;
	extern SimpleImage* IMAGE_SEEDCHOOSER_BACKGROUND;
};

namespace RES_RAXML {
	extern Reanimation* RAXML_ZOMBIESWON;
	extern Reanimation* RAXML_LAWNMOWER;
	extern Reanimation* RAXML_SUN;
	extern Reanimation* RAXML_STARTREADYSETPLANT;
};

namespace RES_PAXML {
	extern Emitter* PAXML_PEAPARTICLETHROW;
	extern Emitter* PAXML_PEASPLATS;
};

void LoadAllResourcesLink();


#endif // !__LAWN_APP_RESOURCES__



