#include "GameObject.h"
#include "../LawnApp.h"

Lawn::GameObject::GameObject()
{
	mTick.BindToCounter(&gLawnApp->mTick);
}