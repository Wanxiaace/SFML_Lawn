
#ifndef __LAWN_BOARD_PLANT__
#define __LAWN_BOARD_PLANT__

#include "GameObject.h"
#include "BoardEnums.h"
#include "BoardAnimator.h"
#include "Board.h"



namespace Lawn {

    class Board;
    class Zombie;

    enum PlantState
    {
        STATE_NOTREADY = 0,
        STATE_READY = 1,
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

        union {
            int mFireCountDown = 0;
            int mProduceCountDown;
        };

		float mHealth = 0;
		int mDamage = 0;
        bool mCanShoot = true;
        union
        {
            bool mIsFire = false;
            bool mIsProduce;
        };

        bool mAvailable = true;
        float mReanimOffsetX;
        float mReanimOffsetY;

    public:
        Plant();
        ~Plant();

		void Init();
		void Fire();
		void Product();
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
