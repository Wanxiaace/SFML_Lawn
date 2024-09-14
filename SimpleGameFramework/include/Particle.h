#pragma once
#ifndef __SIMPLE_PARTICLE__
#define __SIMPLE_PARTICLE__

#include "SimpleImage.h"
#include "Graphics.h"
#include <vector>

namespace sgf {
	enum ParticleMotionType {
		PARTICLE_MOTION_NORMAL,
		PARTICLE_MOTION_THROW,
		PARTICLE_MOTION_THROW_FAST,
	};

	enum ParticleImageType {
		PARTICLE_IMAGE_NONE,
		PARTICLE_IMAGE_SINGLE,
		PARTICLE_IMAGE_GROUP_ANIMARION,
		PARTICLE_IMAGE_GROUP_RANDOM,
	};

	enum EmitterMotionType {
		EMITTER_NORMAL,//直接不动
		EMITTER_THROW,//抛物线投掷
		EMITTER_THROW_FAST,//直接减少lifetime的快速投掷
		
		//EMITTER_BOMB,//四散爆开（不规则）
		//EMITTER_BOMB_PHYSIC,//四散爆开（不规则），物理掉落

	};
	class Graphics;


	class Particle {
	public:
		float mX;
		float mY;
		float mZ;
		unsigned int mLifeTime;
		unsigned int mLifeTimeMax;
		unsigned int mLifeTimeTick;
		unsigned int mEachFrameStride = 100;

		bool mCostingLifeTime;
		bool mMoving;
		bool mVisible;

		unsigned int mTickCache;
		SimpleImage** mImages;
		ParticleMotionType mMotionType;
		ParticleImageType mImageType;
		int mImageNumber = 1;
		float mSpeedX;
		float mSpeedY;
		float mSpeedZ;
		float mImageOffsetX = 0;
		float mImageOffsetY = 0;
		bool mHasShadowed;
		float mGravity;//重力加速度
		float mEnergyLossRate;//能量损耗率，决定回弹高度，范围0~1
		float mRotateSpeed;
		float mAngle;
		float mScaleF = 1.0f;
		float mTransScaleF = 0.0f;
		bool mUseFade = true;
		SimpleImage* mShadowImage;

	public:
		Particle();
		~Particle() {};

		void Init();

		void Update();
		void Draw(Graphics* g) const;
		void TryToRenderImage(Graphics* g) const;
		
	};

	class Emitter {
	public:
		EmitterMotionType mMotionType = EMITTER_NORMAL;
		ParticleImageType mImageType;
		sgf::String* mImageIDs = nullptr;
		std::vector<SimpleImage*> mImages;
		int mImageNumber = 0;
		SimpleImage* mShadowImage = nullptr;
		unsigned int mLifeTimeMax;
		int mX = 0;
		int mY = 0;
		int mZ = 0;
		float mAngleMin = 0;
		float mAngleMax = 0;
		float mPowerMax = 0;
		float mPowerMin = 0;
		float mSpeedX = 0;
		float mSpeedY = 0;
		float mSpeedZ = 0;
		float mLossRate = 0;
		float mImageOffsetX = 0;
		float mImageOffsetY = 0;
		float mScaleF = 1.0f;
		float mTransScaleF = 0.0f;
		float mGravity = 0.1f;
		float mImageScaleRangeF = 0;
		bool mUseFade = true;

	public:

		void Init(EmitterMotionType motionType);
		void InitDefault();
		void SetAngleRange(float angleMin, float angleMax) { mAngleMin = angleMin; mAngleMax = angleMax; };
		Particle* Emitt();
		void AttachImage(SimpleImage* image) { mImages.push_back(image); };

		void AttachShadowImage(SimpleImage* image) { mShadowImage = image; };
		void MoveTo(int targetX, int targetY, int targetZ) { mX = targetX; mY = targetY; mZ = targetZ; };
		//加载paxml
		void LoadFromFile(const char* paxmlPath);
	};

	class ParticleManager {
	public:
		std::vector<Particle*> mParticles;
		int mX = 0;
		int mY = 0;
		int mZ = 0;

	public:
		ParticleManager();
		~ParticleManager();

		void Update();
		void Draw(sgf::Graphics* g);
		void MoveTo(int x, int y, int z) { mX = x; mY = y; mZ = z; };
		void MoveTo(int x,int y) { mX = x; mY = y;};

		Particle* EmittParticle(Emitter* srcEmitter);
		void EmittParticles(Emitter* srcEmitter, int number);

	};
}


#endif // !__SIMPLE_PARTICLE__
