#include <WScom.h>
#include <WSCfunctionList.h>
#include <WSCbase.h>

#include <WSCoption.h>

#include "General.h"

//----------------------------------------------------------
//Function for the event procedure
//----------------------------------------------------------
void IntelligenceValueChFunc(WSCbase* object){
  //do something...
	//get selected index
	long index = ((WSCoption*)object)->getValue();
	//change value
	monster_definitions[selectedMonsterType].intelligence = index;
}
static WSCfunctionRegister  op("IntelligenceValueChFunc",(void*)IntelligenceValueChFunc);
