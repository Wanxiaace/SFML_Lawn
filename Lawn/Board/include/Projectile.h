#pragma once
#ifndef __LAWN_BOARD_PROJECTILE__
#define __LAWN_BOARD_PROJECTILE__

#include "GameObject.h"
#include "BoardEnums.h"
#include "BoardAnimator.h"
#include "../Board.h"

namespace Lawn {
    class Board;
    class Zombie;
    class Plant;

    enum ProjectileMotionType {
        PROJECTILE_MOTION_NONE = -1,
        PROJECTILE_MOTION_NORMAl,
        PROJECTILE_MOTION_PHYSIC,
    };



    class Projectile : public GameObject {
    public:
        sgf::SimpleImage* mProjectileImage = nullptr;
        sgf::SimpleImage* mShadowImage = nullptr;
        ProjectileMotionType mMotionType = PROJECTILE_MOTION_NORMAl;
        ProjectileType mProjectileType = PROJECTILE_NONE;
        Board* mBoard = nullptr;
        float mSpeedX = 0;
        float mSpeedY = 0;
        float mSpeedZ = 0;
        float mGravity = 0.05f;
        float mScale = 1.0f;
        float mZ = 40;
        int mDamage = 0;
        unsigned int mTickCache = 0;
        int mImageOffsetX = 0;
        int mImageOffsetY = 0;
        bool mAvailable = true;

    public:
        Projectile();
        ~Projectile();

        void Init();
        void DoEffect(Zombie* zom);
        void Die();

        static void InitProjectilesDefinitions();
        Zombie* TryToFindTarget();

    public:
        virtual void Update() override;
        virtual void Draw(sgf::Graphics* g) override;
    };
}

#endif // !__LAWN_BOARD_PROJECTILE__
