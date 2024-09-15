#pragma once
#ifndef __LAWN_PLAYER__
#define __LAWN_PLAYER__

#include "PlayerInfo.h"

namespace Lawn {
	class LawnPlayerInfo: public sgf::PlayerInfo {
	public:
		sgf::String mUserName;

	public:
		LawnPlayerInfo();
		~LawnPlayerInfo();

	public:
		virtual void LoadDatas() override;
		virtual void CreateDefaultPlayerInfo(const sgf::String& path) override;
	};
}

#endif //!__LAWN_PLAYER__