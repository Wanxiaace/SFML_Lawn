#include "../include/LawnPlayerInfo.h"

Lawn::LawnPlayerInfo::LawnPlayerInfo()
{

}

Lawn::LawnPlayerInfo::~LawnPlayerInfo()
{

}

void Lawn::LawnPlayerInfo::LoadDatas()
{
	GetElement<sgf::String>("Username", mUserName);


}

void Lawn::LawnPlayerInfo::CreateDefaultPlayerInfo(const sgf::String& path)
{
	SetElementDefault<sgf::String>("Username",mUserName,"None");
}
