#pragma once

#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"

#include <vector>

namespace Lawn {
	class Plant;
	class Zombie;
	class Projectile;

	typedef std::vector<Plant*> PlantVector;
	typedef std::vector<Zombie*> ZombieVector;
	typedef std::vector<Projectile*> ProjectileVector;
}

