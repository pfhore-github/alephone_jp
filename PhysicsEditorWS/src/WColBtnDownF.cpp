#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>
#include "General.h"

//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void WColBtnDownF(WSCbase* object){
	int type = selectedWeaponType;
	bool isMax = false;
	int16 collection = weapon_definitions[type].collection;
	int16 col = GET_COLLECTION(collection);
	int16 clut = GET_COLLECTION_CLUT(collection);
	int ncol = (int)col;
	long ret = selectFromDialog(&ncol, object, stockCollections, isMax);
	if(ret == WS_DIALOG_OK){
		col = (int16)ncol;
		collection = BUILD_COLLECTION(col, clut);
		weapon_definitions[type].collection = collection;
		setupDialog();
	}
}
static WSCfunctionRegister  op("WColBtnDownF",(void*)WColBtnDownF);
