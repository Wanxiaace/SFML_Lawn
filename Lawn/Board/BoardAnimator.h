
#ifndef __LAWN_BOARD_ANIMATOR__
#define __LAWN_BOARD_ANIMATOR__

#include "Animator.h"
#include "../LawnApp.h"

namespace Lawn {
	class LawnApp;

	class BoardAnimator : public sgf::Animator {
	public:
		BoardAnimator(sgf::Reanimation* reanim,Lawn::LawnApp* app);
		~BoardAnimator();

		int GetAnimatorIndex();
	};
}

#endif // !__LAWN_BOARD_ANIMATOR__
