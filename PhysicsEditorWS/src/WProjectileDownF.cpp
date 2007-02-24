#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "General.h"
//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void WProjectileDownF(WSCbase* object){
	int type = selectedWeaponType;
	bool isMax = true;
	int index = weapon_definitions[type].weapons_by_trigger[0].projectile_type;
	long ret = selectFromDialog(&index, object, stockProjectiles, isMax);
	if(ret == WS_DIALOG_OK){
		weapon_definitions[type].weapons_by_trigger[0].projectile_type = index;
		setupDialog();
	}
}
static WSCfunctionRegister  op("WProjectileDownF",(void*)WProjectileDownF);
