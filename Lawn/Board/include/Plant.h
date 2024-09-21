#pragma once
#ifndef __LAWN_BOARD_PLANT__
#define __LAWN_BOARD_PLANT__

#include "GameObject.h"
#include "BoardEnums.h"
#include "BoardAnimator.h"
#include "../Board.h"

namespace Lawn {

    class Board;
    class Zombie;

    enum PlantState
    {
        STATE_NOTREADY = 0,
        STATE_READY = 1,
        STATE_DOINGSPECIAL = 2,
        STATE_SQUASH_LOOK = 3,
        STATE_SQUASH_PRE_LAUNCH = 4,
        STATE_SQUASH_RISING = 5,
        STATE_SQUASH_FALLING = 6,
        STATE_SQUASH_DONE_FALLING = 7,
        STATE_GRAVEBUSTER_LANDING = 8,
        STATE_GRAVEBUSTER_EATING = 9,
        STATE_CHOMPER_BITING = 10,
        STATE_CHOMPER_BITING_GOT_ONE = 11,
        STATE_CHOMPER_BITING_MISSED = 12,
        STATE_CHOMPER_DIGESTING = 13,
        STATE_CHOMPER_SWALLOWING = 14,
        STATE_POTATO_RISING = 15,
        STATE_POTATO_ARMED = 16,
        STATE_POTATO_MASHED = 17,
        STATE_SPIKEWEED_ATTACKING = 18,
        STATE_SPIKEWEED_ATTACKING_2 = 19,
        STATE_SCAREDYSHROOM_LOWERING = 20,
        STATE_SCAREDYSHROOM_SCARED = 21,
        STATE_SCAREDYSHROOM_RAISING = 22,
        STATE_SUNSHROOM_SMALL = 23,
        STATE_SUNSHROOM_GROWING = 24,
        STATE_SUNSHROOM_BIG = 25,
        STATE_MAGNETSHROOM_SUCKING = 26,
        STATE_MAGNETSHROOM_CHARGING = 27,
        STATE_BOWLING_UP = 28,
        STATE_BOWLING_DOWN = 29,
        STATE_CACTUS_LOW = 30,
        STATE_CACTUS_RISING = 31,
        STATE_CACTUS_HIGH = 32,
        STATE_CACTUS_LOWERING = 33,
        STATE_DUPLICATOR_REFRESHING = 34,
        STATE_DUPLICATOR_READY = 35,
        STATE_DUPLICATOR_PLANTING = 36,
        STATE_TANGLEKELP_GRABBING = 37,
        STATE_COBCANNON_ARMING = 38,
        STATE_COBCANNON_LOADING = 39,
        STATE_COBCANNON_READY = 40,
        STATE_COBCANNON_FIRING = 41,
        STATE_KERNELPULT_BUTTER = 42,
        STATE_UMBRELLA_TRIGGERED = 43,
        STATE_UMBRELLA_REFLECTING = 44,
        STATE_IMITATER_MORPHING = 45,
        STATE_ZEN_GARDEN_WATERED = 46,
        STATE_ZEN_GARDEN_NEEDY = 47,
        STATE_ZEN_GARDEN_HAPPY = 48,
        STATE_MARIGOLD_ENDING = 49,
        STATE_FLOWERPOT_INVULNERABLE = 50,
        STATE_LILYPAD_INVULNERABLE = 51
    };

	class Plant : public GameObject {
	public:
		SeedType mSeedType = SeedType::SEED_NONE;
		SeedType mImitatorSeedType = SeedType::SEED_NONE;
        PlantState mState = PlantState::STATE_NOTREADY;
        Board* mBoard = nullptr;
        Zombie* mTargetZombie = nullptr;

        sgf::SimpleImage* mShadowImage = nullptr;
        sgf::FloatRect mShootBox;//Ë÷µÐÅö×²Ïä
        sgf::Animator mBodyReanim;
        sgf::Animator mHeadReanim;
        glm::mat4x4 mHeadReanimMatrix;

		int mFireCountDown = 0;
		int mProduceCountDown = 0;
		float mHealth = 0;
		int mDamage = 0;
        bool mCanShoot = true;
        //int mFireCD = 0;
        unsigned int mTickCache = 0;
        bool mIsFire = false;
        bool mAvailable = true;

    public:
        Plant();
        ~Plant();

		void PlantInit();
		void Fire();
        void AttachToBoard(Board* board) { mBoard = board; };
        bool IsOnBoard() const;
        Zombie* TryToFindTarget();
        void Die();
        void TakeDamage(float damage);
        void PlayTrack(const sgf::String& trackName,int blendTime = 100);

		static void InitPlantsDefinitions();

	public:
		virtual void Update() override;
		virtual void Draw(sgf::Graphics* g) override;
        virtual void MoveTo(float x, float y) override;
	};
}

#endif // !__LAWN_BOARD_PLANT__
