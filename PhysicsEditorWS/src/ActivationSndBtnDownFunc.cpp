#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "General.h"

//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void ActivationSndBtnDownFunc(WSCbase* object){
	//選択して値設定
	int type = selectedMonsterType;
	bool isMaxNONE = true;//*none*有り
	int index = monster_definitions[type].activation_sound;
	long ret = selectFromDialog(&index, object, stockSounds, isMaxNONE);
	if(ret == WS_DIALOG_OK){
		monster_definitions[type].activation_sound = index;
		setupDialog();
	}
}
static WSCfunctionRegister  op("ActivationSndBtnDownFunc",(void*)ActivationSndBtnDownFunc);
