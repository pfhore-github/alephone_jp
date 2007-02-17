#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>

#include "General.h"

//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void BtnAllUncheckDownFunc(WSCbase* object){
	//enemies & friends
	WSCcheckGroup* lstEnemies = (WSCcheckGroup*)getObject("WSCcheckGroup", "ListEnemies");
	WSCcheckGroup* lstFriends = (WSCcheckGroup*)getObject("WSCcheckGroup", "ListFriends");
	int type = selectedMonsterType;
	if(type >= 0 && type < NUMBER_OF_MONSTER_TYPES){
		int enemies = monster_definitions[type].enemies;
		int friends = monster_definitions[type].friends;
		for(int i = 0; i < NUMBER_OF_CLASS_INFORMATIONS; i ++){
			WSCvtoggle* enemy = (WSCvtoggle*)lstEnemies->getItem(i);
			WSCvtoggle* _friend = (WSCvtoggle*)lstFriends->getItem(i);
			enemy->setStatus(False);
			_friend->setStatus(False);
		}
		//変更呼び出し
		lstEnemies->onValueChange();
		lstFriends->onValueChange();
	}
}
static WSCfunctionRegister  op("BtnAllUncheckDownFunc",(void*)BtnAllUncheckDownFunc);
