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
		TickCounter mTick;
		std::function<void()> mNextFunc;
		T* mSpot = nullptr;
		T mRangeMax;
		T mRangeMin;
		T mSpeed;
		T mProcess;
		bool mIsRunning = false;
		bool mIsBindNextFunc = false;

	public:
		EffectHolder() {};
		~EffectHolder() {};

		void BindSpot(T* spot, T max, T min) { 
			mSpot = spot; mRangeMax = max; mRangeMin = min;
			if(mSpeed > 0)
				mProcess = mRangeMin;
			else 
				mProcess = mRangeMax;

			Start();
		};
		void SetSpeed(T speed) { mSpeed = speed; };

	public:
		void Update() {
			mTick.Update();
			if (!mIsRunning)
				return;
			T tar = mProcess + (T)mTick.GetDeltaTick() / 1000.0f * mSpeed;

			if (mSpeed >= 0) {
				if (tar > mRangeMax) {
					mProcess = mRangeMax;
					mIsRunning = false;
					if (mIsBindNextFunc)
					{
						mIsBindNextFunc = false;
						mNextFunc();
					}
						
				}
				else {
					mProcess = tar;
				}
			}
			else {
				if (tar < mRangeMin) {
					mProcess = mRangeMin;
					mIsRunning = false;
					if (mIsBindNextFunc)
					{
						mIsBindNextFunc = false;
						mNextFunc();
					}
				}
				else {
					mProcess = tar;
				}
			}

			if (mSpot)
				*mSpot = mProcess;
		};

		void Start() { mIsRunning = true; };
		void SetNextFunction(std::function<void()> func) { mNextFunc = func; mIsBindNextFunc = true; };
	};

	using EffectHolderFloat = EffectHolder<float>;
	using EffectHolderInt = EffectHolder<int>;
}

#endif // !__SGF_EFFECT_HOLDER__
