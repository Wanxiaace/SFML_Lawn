
#ifndef __LAWN_BOARD_ENUMS__
#define __LAWN_BOARD_ENUMS__

#include <unordered_map>


namespace Lawn {
    enum SeedType : int
    {
        SEED_PEASHOOTER = 0,
        SEED_SUNFLOWER = 1,
        SEED_CHERRYBOMB = 2,
        SEED_WALLNUT = 3,
        SEED_POTATOMINE = 4,
        SEED_SNOWPEA = 5,
        SEED_CHOMPER = 6,
        SEED_REPEATER = 7,
        SEED_PUFFSHROOM = 8,
        SEED_SUNSHROOM = 9,
        SEED_FUMESHROOM = 10,
        SEED_GRAVEBUSTER = 11,
        SEED_HYPNOSHROOM = 12,
        SEED_SCAREDYSHROOM = 13,
        SEED_ICESHROOM = 14,
        SEED_DOOMSHROOM = 15,
        SEED_LILYPAD = 16,
        SEED_SQUASH = 17,
        SEED_THREEPEATER = 18,
        SEED_TANGLEKELP = 19,
        SEED_JALAPENO = 20,
        SEED_SPIKEWEED = 21,
        SEED_TORCHWOOD = 22,
        SEED_TALLNUT = 23,
        SEED_SEASHROOM = 24,
        SEED_PLANTERN = 25,
        SEED_CACTUS = 26,
        SEED_BLOVER = 27,
        SEED_SPLITPEA = 28,
        SEED_STARFRUIT = 29,
        SEED_PUMPKINSHELL = 30,
        SEED_MAGNETSHROOM = 31,
        SEED_CABBAGEPULT = 32,
        SEED_FLOWERPOT = 33,
        SEED_KERNELPULT = 34,
        SEED_INSTANT_COFFEE = 35,
        SEED_GARLIC = 36,
        SEED_UMBRELLA = 37,
        SEED_MARIGOLD = 38,
        SEED_MELONPULT = 39,
        SEED_GATLINGPEA = 40,
        SEED_TWINSUNFLOWER = 41,
        SEED_GLOOMSHROOM = 42,
        SEED_CATTAIL = 43,
        SEED_WINTERMELON = 44,
        SEED_GOLD_MAGNET = 45,
        SEED_SPIKEROCK = 46,
        SEED_COBCANNON = 47,
        SEED_IMITATER = 48,
        SEED_TIMESTOPPER = 49,
        SEED_ICEBERG = 50,
        SEED_CHERRYHOVERBOMB = 51,
        SEED_STINGER = 52,
        SEED_DUPLICATOR = 53,
        SEED_EXPLODE_O_NUT = 54,
        SEED_GIANT_WALLNUT = 55,
        SEED_SPROUT = 56,
        SEED_LEFTPEATER = 57,
        NUM_SEED_TYPES = 58,
        SEED_BEGHOULED_BUTTON_SHUFFLE = 59,
        SEED_BEGHOULED_BUTTON_CRATER = 60,
        SEED_SLOT_MACHINE_SUN = 61,
        SEED_SLOT_MACHINE_DIAMOND = 62,
        SEED_ZOMBIQUARIUM_SNORKLE = 63,
        SEED_ZOMBIQUARIUM_TROPHY = 64,
        SEED_ZOMBIE_NORMAL = 65,
        SEED_ZOMBIE_TRAFFIC_CONE = 66,
        SEED_ZOMBIE_POLEVAULTER = 67,
        SEED_ZOMBIE_PAIL = 68,
        SEED_ZOMBIE_LADDER = 69,
        SEED_ZOMBIE_DIGGER = 70,
        SEED_ZOMBIE_BUNGEE = 71,
        SEED_ZOMBIE_FOOTBALL = 72,
        SEED_ZOMBIE_BALLOON = 73,
        SEED_ZOMBIE_SCREEN_DOOR = 74,
        SEED_ZOMBONI = 75,
        SEED_ZOMBIE_POGO = 76,
        SEED_ZOMBIE_DANCER = 77,
        SEED_ZOMBIE_GARGANTUAR = 78,
        SEED_NONE = -1

    };

    enum ZombieType
    {
        ZOMBIE_INVALID = -1,
        ZOMBIE_NORMAL = 0,
        ZOMBIE_FLAG = 1,
        ZOMBIE_TRAFFIC_CONE = 2,
        ZOMBIE_POLEVAULTER = 3,
        ZOMBIE_PAIL = 4,
        ZOMBIE_NEWSPAPER = 5,
        ZOMBIE_DOOR = 6,
        ZOMBIE_FOOTBALL = 7,
        ZOMBIE_DANCER = 8,
        ZOMBIE_BACKUP_DANCER = 9,
        ZOMBIE_DUCKY_TUBE = 10,
        ZOMBIE_SNORKEL = 11,
        ZOMBIE_ZAMBONI = 12,
        ZOMBIE_BOBSLED = 13,
        ZOMBIE_DOLPHIN_RIDER = 14,
        ZOMBIE_JACK_IN_THE_BOX = 15,
        ZOMBIE_BALLOON = 16,
        ZOMBIE_DIGGER = 17,
        ZOMBIE_POGO = 18,
        ZOMBIE_YETI = 19,
        ZOMBIE_BUNGEE = 20,
        ZOMBIE_LADDER = 21,
        ZOMBIE_CATAPULT = 22,
        ZOMBIE_GARGANTUAR = 23,
        ZOMBIE_IMP = 24,
        ZOMBIE_BOSS = 25,
        ZOMBIE_DOG_WALKER = 26,
        ZOMBIE_DOG = 27,
        ZOMBIE_PROPELLER = 28,
        ZOMBIE_PEA_HEAD = 29,
        ZOMBIE_WALLNUT_HEAD = 30,
        ZOMBIE_JALAPENO_HEAD = 31,
        ZOMBIE_GATLING_HEAD = 32,
        ZOMBIE_SQUASH_HEAD = 33,
        ZOMBIE_TALLNUT_HEAD = 34,
        NUM_ZOMBIE_TYPES = 35
    };

    enum ProjectileType
    {
        PROJECTILE_NONE = -1,
        PROJECTILE_PEA = 0,
        PROJECTILE_SNOWPEA = 1,
        PROJECTILE_BEE = 2,
        PROJECTILE_CABBAGE = 3,
        PROJECTILE_MELON = 4,
        PROJECTILE_PUFF = 5,
        PROJECTILE_COB = 6,
        PROJECTILE_POPCORN = 7,
        PROJECTILE_WINTERMELON = 8,
        PROJECTILE_ICEBERG = 9,
        PROJECTILE_FIREBALL = 10,
        PROJECTILE_STAR = 11,
        PROJECTILE_SPIKE = 12,
        PROJECTILE_BASKETBALL = 13,
        PROJECTILE_KERNEL = 14,
        PROJECTILE_COBBIG = 15,
        PROJECTILE_BUTTER = 16,
        PROJECTILE_ZOMBIE_PEA = 17,
        NUM_PROJECTILES = 18
    };

    enum ZombieDamageType {
       ZOMBIE_DAMAGE_NORMAL,
    };

    enum LawnMoverType {
        LAWN_MOVER_NORMAL,
    };

    enum SceneObjectType {
        SCENE_OBJECT_SUN,
    };

    struct PlantDefinition {
        SeedType mSeedType;
        sgf::String mReanimationName;
        sgf::String mPlantName;
        sgf::String mPlantDescription;
        int mCost;
        int mDamage;
        float mReanimOffsetX;
        float mReanimOffsetY;
    };

    struct ZombieDefinition {
        ZombieType mZombieType = ZOMBIE_INVALID;
        sgf::String mReanimationName;
        sgf::String mZombieName;
        sgf::String mZombieDescription;
        float mSpeedMin = 0;
        float mSpeedMax = 0;
        int mPickWeight = 1;
        int mReanimOffsetX = 0;
        int mReanimOffsetY = 0;
        int mMinimumWave = 0;//最低可出场波数
    };

    enum SeedPackBackground {
        SEEDPACK_NORMAL,
        SEEDPACK_NONE,
    };


    struct ProjectileDefinition {
        ProjectileType mZombieType;
        int mDamage;
        int mImageOffsetX;
        int mImageOffsetY;
        sgf::String mProjectileName;
        sgf::String mImageID;
    };



   
    extern std::unordered_map<SeedType, PlantDefinition> gPlantsDefinitions;
    extern std::unordered_map<ZombieType, ZombieDefinition> gZombiesDefinitions;
    extern std::unordered_map<ProjectileType, ProjectileDefinition> gProjectilesDefinitions;
}
#endif // !__LAWN_BOARD_ENUMS__
