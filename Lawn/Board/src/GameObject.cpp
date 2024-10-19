#include "../include/GameObject.h"
#include "../Lawn/include/LawnApp.h"

Lawn::GameObject::GameObject()
{
	mTick.BindToCounter(&gLawnApp->mTick);
}