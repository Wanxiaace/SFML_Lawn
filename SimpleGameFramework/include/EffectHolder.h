#pragma once
#ifndef __SGF_EFFECT_HOLDER__
#define __SGF_EFFECT_HOLDER__

#include "Common.h"
#include <functional>


namespace sgf {
	template<typename T>
	class EffectHolder {
	public:
		float mEffectCounter = 0;
		float mEffectCounterMax = 0;
		unsigned int mTickDelta = 0;
		std::function<void()> mNextFunc = 0;
		T* mSpot = nullptr;
		T mRangeMax;
		T mRangeMin;

	public:
		EffectHolder();
		~EffectHolder();

		void BindSpot(T* spot, T max, T min) { 
			mSpot = spot; mRangeMax = max; mRangeMin = min; };

	public:
		virtual void Update();
	};

	using EffectHolderFloat = EffectHolder<float>;
	using EffectHolderInt = EffectHolder<int>;
}

#endif // !__SGF_EFFECT_HOLDER__
