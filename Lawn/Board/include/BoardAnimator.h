#pragma once
#ifndef __LAWN_BOARD_ANIMATOR__
#define __LAWN_BOARD_ANIMATOR__

#include "Animator.h"

namespace Lawn {
	class BoardAnimator : public sgf::Animator {
	public:
		BoardAnimator(sgf::Reanimation* reanim);
		~BoardAnimator();

		int GetAnimatorIndex();
	};
}

#endif // !__LAWN_BOARD_ANIMATOR__
