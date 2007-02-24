#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "General.h"

//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void ImpactEffectBtnDownFunc(WSCbase* object){
	//選択して値設定
	int type = selectedMonsterType;
	bool isMaxNONE = true;//*none*有り
	int index = monster_definitions[type].impact_effect;
	long ret = selectFromDialog((int*)&index,
		object, stockEffects, isMaxNONE);
	if(ret == WS_DIALOG_OK){
		monster_definitions[type].impact_effect = index;
		setupDialog();
	}
}
static WSCfunctionRegister  op("ImpactEffectBtnDownFunc",(void*)ImpactEffectBtnDownFunc);
