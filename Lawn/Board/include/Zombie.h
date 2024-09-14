#pragma once
#ifndef __LAWN_BOARD_ZOMBIE__
#define __LAWN_BOARD_ZOMBIE__

#include "GameObject.h"
#include "BoardEnums.h"
#include "BoardAnimator.h"
#include "../Board.h"
#include "Plant.h"

namespace Lawn {
    class Board;
    class Plant;

    enum ZombiePhase
    {
        PHASE_ZOMBIE_NORMAL = 0,
        PHASE_ZOMBIE_DYING = 1,
        PHASE_ZOMBIE_BURNED = 2,
        PHASE_ZOMBIE_MOWERED = 3,
        PHASE_BUNGEE_DIVING = 4,
        PHASE_BUNGEE_DIVING_SCREAMING = 5,
        PHASE_BUNGEE_AT_BOTTOM = 6,
        PHASE_BUNGEE_GRABBING = 7,
        PHASE_BUNGEE_RISING = 8,
        PHASE_BUNGEE_HIT_OUCHY = 9,
        PHASE_BUNGEE_CUTSCENE = 10,
        PHASE_POLEVAULTER_PRE_VAULT = 11,
        PHASE_POLEVAULTER_IN_VAULT = 12,
        PHASE_POLEVAULTER_POST_VAULT = 13,
        PHASE_RISING_FROM_GRAVE = 14,
        PHASE_JACK_IN_THE_BOX_RUNNING = 15,
        PHASE_JACK_IN_THE_BOX_POPPING = 16,
        PHASE_WALKING_DOG = 17,
        PHASE_DOG_ON_LEASH = 18,
        PHASE_DOG_OFF_LEASH = 19,
        PHASE_BOBSLED_SLIDING = 20,
        PHASE_BOBSLED_BOARDING = 21,
        PHASE_BOBSLED_CRASHING = 22,
        PHASE_POGO_BOUNCING = 23,
        PHASE_POGO_HIGH_BOUNCE_1 = 24,
        PHASE_POGO_HIGH_BOUNCE_2 = 25,
        PHASE_POGO_HIGH_BOUNCE_3 = 26,
        PHASE_POGO_HIGH_BOUNCE_4 = 27,
        PHASE_POGO_HIGH_BOUNCE_5 = 28,
        PHASE_POGO_HIGH_BOUNCE_6 = 29,
        PHASE_POGO_FORWARD_BOUNCE_2 = 30,
        PHASE_POGO_FORWARD_BOUNCE_7 = 31,
        PHASE_NEWSPAPER_READING = 32,
        PHASE_NEWSPAPER_MADDENING = 33,
        PHASE_NEWSPAPER_MAD = 34,
        PHASE_DIGGER_TUNNELING = 35,
        PHASE_DIGGER_RISING = 36,
        PHASE_DIGGER_TUNNELING_PAUSE_WITHOUT_AXE = 37,
        PHASE_DIGGER_RISE_WITHOUT_AXE = 38,
        PHASE_DIGGER_STUNNED = 39,
        PHASE_DIGGER_WALKING = 40,
        PHASE_DIGGER_WALKING_WITHOUT_AXE = 41,
        PHASE_DIGGER_CUTSCENE = 42,
        PHASE_DANCER_DANCING_IN = 43,
        PHASE_DANCER_SNAPPING_FINGERS = 44,
        PHASE_DANCER_SNAPPING_FINGERS_WITH_LIGHT = 45,
        PHASE_DANCER_SNAPPING_FINGERS_HOLD = 46,
        PHASE_DANCER_DANCING_LEFT = 47,
        PHASE_DANCER_WALK_TO_RAISE = 48,
        PHASE_DANCER_RAISE_LEFT_1 = 49,
        PHASE_DANCER_RAISE_RIGHT_1 = 50,
        PHASE_DANCER_RAISE_LEFT_2 = 51,
        PHASE_DANCER_RAISE_RIGHT_2 = 52,
        PHASE_DANCER_RISING = 53,
        PHASE_DOLPHIN_WALKING = 54,
        PHASE_DOLPHIN_INTO_POOL = 55,
        PHASE_DOLPHIN_RIDING = 56,
        PHASE_DOLPHIN_IN_JUMP = 57,
        PHASE_DOLPHIN_WALKING_IN_POOL = 58,
        PHASE_DOLPHIN_WALKING_WITHOUT_DOLPHIN = 59,
        PHASE_SNORKEL_WALKING = 60,
        PHASE_SNORKEL_INTO_POOL = 61,
        PHASE_SNORKEL_WALKING_IN_POOL = 62,
        PHASE_SNORKEL_UP_TO_EAT = 63,
        PHASE_SNORKEL_EATING_IN_POOL = 64,
        PHASE_SNORKEL_DOWN_FROM_EAT = 65,
        PHASE_ZOMBIQUARIUM_ACCEL = 66,
        PHASE_ZOMBIQUARIUM_DRIFT = 67,
        PHASE_ZOMBIQUARIUM_BACK_AND_FORTH = 68,
        PHASE_ZOMBIQUARIUM_BITE = 69,
        PHASE_CATAPULT_LAUNCHING = 70,
        PHASE_CATAPULT_RELOADING = 71,
        PHASE_GARGANTUAR_THROWING = 72,
        PHASE_GARGANTUAR_SMASHING = 73,
        PHASE_IMP_GETTING_THROWN = 74,
        PHASE_IMP_LANDING = 75,
        PHASE_BALLOON_FLYING = 76,
        PHASE_BALLOON_POPPING = 77,
        PHASE_BALLOON_WALKING = 78,
        PHASE_LADDER_CARRYING = 79,
        PHASE_LADDER_PLACING = 80,
        PHASE_BOSS_ENTER = 81,
        PHASE_BOSS_IDLE = 82,
        PHASE_BOSS_SPAWNING = 83,
        PHASE_BOSS_STOMPING = 84,
        PHASE_BOSS_BUNGEES_ENTER = 85,
        PHASE_BOSS_BUNGEES_DROP = 86,
        PHASE_BOSS_BUNGEES_LEAVE = 87,
        PHASE_BOSS_DROP_RV = 88,
        PHASE_BOSS_HEAD_ENTER = 89,
        PHASE_BOSS_HEAD_IDLE_BEFORE_SPIT = 90,
        PHASE_BOSS_HEAD_IDLE_AFTER_SPIT = 91,
        PHASE_BOSS_HEAD_SPIT = 92,
        PHASE_BOSS_HEAD_LEAVE = 93,
        PHASE_YETI_RUNNING = 94,
        PHASE_SQUASH_PRE_LAUNCH = 95,
        PHASE_SQUASH_RISING = 96,
        PHASE_SQUASH_FALLING = 97,
        PHASE_SQUASH_DONE_FALLING = 98
    };

    class Zombie : public GameObject {
    public:
        Board* mBoard = nullptr;
        
        ZombieType mZombieType;
        ZombiePhase mState;
        sgf::Animator mBodyReanim;
        sgf::SimpleImage* mShadowImage = nullptr;


        float mSpeedMin = 0;
        float mSpeedMax = 0;
        float mFlashCounter = 0;

        float mHealth = 270;
        float mHealthMax = 270;

        bool mHasHead = true;
        bool mHasArm = true;
        bool mDoDeathReanim = false;
        int mReanimOffsetX = 0;
        int mReanimOffsetY = 0;
        
        bool mAvailable = true;
        bool mIsLive = true;
        bool mIsEating = false;
        int mEatingChunkCounter = 0;

    public:
        Zombie();
        void Init();
        void InitNormalZombieReanim();
        //根据速度的范围得到新的速度
        void ResetZomSpeed();
        void DieNoLoot();
        void TakeDamage(ZombieDamageType damageType, int damage);
        void CheckIsDie();
        void DropArm();
        void DropHead();
        void DoDeathReanim();
        void PlayTrack(const sgf::String& trackName, int blendTime = 100);
        void UpdateEating(Plant* target);

        Plant* FindPlant();

        static void InitZombiesDefinitions();

    public:
        virtual void Update() override;
        virtual void Draw(sgf::Graphics* g) override;
    };
}

#endif // !__LAWN_BOARD_ZOMBIE__
