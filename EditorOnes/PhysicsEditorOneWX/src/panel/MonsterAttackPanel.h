#ifndef _MONSTER_ATTACK_PANEL_
#define _MONSTER_ATTACK_PANEL_

#include "../Common.h"

/**
	攻撃行動に関する設定
*/
class MonsterAttackPanel: public wxPanel{
private:
	MonsterAttackPanel(){}
public:
	MonsterAttackPanel(wxWindow* parent, wxWindowID id);
	virtual ~MonsterAttackPanel();
};

#endif
