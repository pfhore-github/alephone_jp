// MapEditorSDI.h : MapEditorSDI �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "HPLLibMFC.h"
#include "AlephOne/header.h"
#include "AlephOne/extensions.h"
#include "AlephOne/FileHandler.h"
#include "AlephOne/map.h"
#include "AlephOne/game_wad.h"
#include "AlephOne/weapons.h"
#include "AlephOne/items.h"
#include "AlephOne/monsters.h"
#include "AlephOne/scenery_definitions.h"
#include "AlephOne/mysound.h"
#include "AlephOne/computer_interface.h"

//#include "AlephOne/placement.h"

#include "MonsterPropertyDialog.h"
#include "PolygonTypeDialog.h"
#include "HeightDialog.h"
#include "LevelParameterDialog.h"
#include "TerminalDialog.h"

// CMapEditorSDIApp:
// ���̃N���X�̎����ɂ��ẮAMapEditorSDI.cpp ���Q�Ƃ��Ă��������B
//

const int NUMBER_OF_POLYGON_TYPE = 24;
const int ZOOM_DIVISION_STEP = 5;
const int ZOOM_DIVISION_MAX = 200;
const int ZOOM_DIVISION_MIN = 1;
const int ZOOM_DIVISION_DEFAULT = 100;
const int OFFSET_X_WORLD = 32768;
const int OFFSET_Y_WORLD = 32768;
const int NUMBER_OF_GLID = 5;

const int NUMBER_OF_OBJECT_TYPES = 6;
const int NUMBER_OF_ACTIVATE_TYPES = 4;
const int NUMBER_OF_MAP_OBJECT_TYPES = 6;

const int NUMBER_OF_LANDSPACES = 4;
//�_�ƃN���b�N�n�_�̋���������ȉ��ł���ΑI������B
//���X�g���ɒT������
const int POINT_DISTANCE_EPSILON = 5;

//�I���������̂̎��
enum{
    _no_selected,
    _selected_point,
    _selected_line,
    _selected_polygon,
    _selected_object,
};

//����
typedef struct Information_tag{
    CString jname;
}Information;
//�l������
typedef struct InformationBinded_tag{
    CString jname;
    int bind;
}InformationBinded;

struct selectInformation{
    bool isSelected(){
        return isSelected_;
    }
    void setSelected(bool sel){
        isSelected_ = sel;
    }
    void clear(){
        endpointIndexList.clear();
        lineIndexList.clear();
        polygonIndexList.clear();
        objectIndexList.clear();
        setSelected(false);
    }
    //�_�̃��X�g
    vector<int> endpointIndexList;

    //���̃��X�g
    vector<int> lineIndexList;

    //�|���S���̃��X�g
    vector<int> polygonIndexList;

    vector<int> objectIndexList;
private:
    bool isSelected_;

};

/**
    MapEditorOne���C���A�v���P�[�V����
*/
class CMapEditorSDIApp : public CWinApp
{
public:
	CMapEditorSDIApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
public:
    //���x���̖���
    CStringArray LevelNameList;
    
    //�I�𒆂̔{��
    int zoomDivision;

    //�O���b�h�\���p�Ԋu(���E�����ɑΉ����A�g��k���ő傫���ω�)
    int gridIntervals[NUMBER_OF_GLID];
    int nowGridInterval;

    //�ʒu�I�t�Z�b�g
    POINT offset;

    POINT nowMousePoint;
    POINT oldMousePoint;

    //�I���������̂̎��
    int selectType;
    //�I���������̂̃C���f�b�N�X
    int selectIndex;

    //�O���b�h
    //�{�^�������Ă�H
    bool isPressLButtonWithShift;

    //�I�𒆂̃��x���C���f�b�N�X
    int editLevelIndex;
    //�|���S���^�C�v�̐F
    COLORREF polygonTypeColor[NUMBER_OF_POLYGON_TYPE];

    //���[�h���X�����X�^�[�v���p�e�B�_�C�A���O
    CMonsterPropertyDialog *objectPropertyDialog;
    //���[�h���X�|���S���^�C�v�_�C�A���O
    CPolygonTypeDialog *polygonTypeDialog;
    //���[�h���X�\�����x�_�C�A���O
    CHeightDialog *heightDialog;

    //���
    Information objectTypeInformations[NUMBER_OF_MAP_OBJECT_TYPES];
    //�����X�^�[�̎��
    Information monsterTypeInformations[NUMBER_OF_MONSTER_TYPES];
    //�I�u�W�F�̎��
    Information sceneryTypeInformations[NUMBER_OF_SCENERY_DEFINITIONS];
    //�A�C�e���̎��
    Information itemTypeInformations[NUMBER_OF_DEFINED_ITEMS];
    //��
    Information soundSourceInformations[NUMBER_OF_AMBIENT_SOUND_DEFINITIONS];
    //�N��
    Information activateTypeInformations[NUMBER_OF_ACTIVATE_TYPES];
    //�t���O
    InformationBinded objectFlagInformations[NUMBER_OF_OBJECT_FLAGS];

    //��
    Information environmentInformations[NUMBER_OF_ENVIRONMENTS];
    //�w�i
    Information landscapeInformations[NUMBER_OF_LANDSPACES];
    //�Q�[���^�C�v
    InformationBinded gameTypeInformations[NUMBER_OF_GAME_TYPES];
    //���^�C�v
    InformationBinded environmentTypeInformations[NUMBER_OF_ENV_TYPES];
    //�~�b�V�����^�C�v
    InformationBinded missionTypeInformations[NUMBER_OF_MISSION_TYPES];

    //�^�[�~�i���O���[�v
    Information terminalGroupTypeInformations[NUMBER_OF_GROUP_TYPES];

// ����
	afx_msg void OnAppAbout();
    BOOL isObjectPropertyDialogShow;
    BOOL isHeightDialogShow;
    BOOL isPolygonTypeDialogShow;

    //�_�u���o�b�t�@�����O
    CDC doubleBufferDC;
    CBitmap doubleBuffserBitmap;

    //�|���S��������������Ƃ��̑��|�C���g����̃I�t�Z�b�g
    //���΍��W
    POINT polygonPoints[8];
    int polygonPointNum;

    //�I��͈͎n�_
    POINT selectStartPoint;
    bool isSelectingGroup;

    //�����I�����̑I�𕨃��X�g
    struct selectInformation selectGroupInformation;


	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFileOpen();
};

extern CMapEditorSDIApp theApp;