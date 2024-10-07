#pragma once

#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"
#include "LawnMover.h"

#include <vector>

namespace Lawn {
	class Plant;
	class Zombie;
	class Projectile;
	class LawnMover;

	typedef std::vector<Plant*> PlantVector;
	typedef std::vector<Zombie*> ZombieVector;
	typedef std::vector<Projectile*> ProjectileVector;
	typedef std::vector<LawnMover*> LawnMoverVector;
}

