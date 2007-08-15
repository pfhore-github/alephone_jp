#ifndef _HEIGHT_PALLET_DIALOG_
#define _HEIGHT_PALLET_DIALOG_

#include "DlgCommon.h"

class HeightPaletteDialog: public wxDialog{
    wxButton* button_24;
    wxButton* button_25;
    wxListCtrl* list_ctrl_3;

    bool isFloor_;

public:
    HeightPaletteDialog();
    bool Create(wxWindow* parent, wxWindowID id);
    virtual ~HeightPaletteDialog();
    //�C�x���g�e�[�u���쐬<en>declare
    DECLARE_EVENT_TABLE()
    void OnAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnDelete(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEdit(wxListEvent &event); // wxGlade: <event_handler>

    wxColor getColorFromHeight(double height);
public:
    /**
        ���݂̃}�b�v�̍������������܂�
        ���܂ł̂��͍̂폜����܂�
    */
    void updateHeights();

    /**
        ���Ȃ̂��V��Ȃ̂�
        <en>Floor or Ceiling
    */
    void setFloor(bool floor);
    bool isFloor();

	/**
		�w�肵�������ɊY������A�C�e����I����Ԃɂ��܂�
	*/
	void setSelection(int height);
};

#endif