// -*- C++ -*- generated by wxGlade 0.5 on Wed Aug 08 00:32:20 2007 from C:\0save\marathon\EditorOnes\MapEditorWX\src\res\MapEditorWX.wxg

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
// end wxGlade


#ifndef DIALOGUES_H
#define DIALOGUES_H


class wxDialog: public wxDialog {
public:
    // begin wxGlade: wxDialog::ids
    // end wxGlade

    wxDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: wxDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: wxDialog::attributes
    wxStaticBox* sizer_31_staticbox;
    wxStaticBox* sizer_27_staticbox;
    wxStaticBox* sizer_28_staticbox;
    wxStaticBox* sizer_19_staticbox;
    wxStaticBox* sizer_12_staticbox;
    wxStaticBox* sizer_11_staticbox;
    wxStaticBox* sizer_10_staticbox;
    wxStaticText* label_1;
    wxChoice* choice_1;
    wxStaticText* label_2;
    wxTextCtrl* text_ctrl_1;
    wxChoice* choice_2;
    wxStaticText* label_3;
    wxTextCtrl* text_ctrl_2;
    wxChoice* choice_3;
    wxCheckBox* checkbox_7;
    wxStaticText* label_4;
    wxTextCtrl* text_ctrl_3;
    wxCheckBox* checkbox_8;
    wxStaticText* label_5;
    wxTextCtrl* text_ctrl_4;
    wxCheckBox* checkbox_1;
    wxCheckBox* checkbox_2;
    wxCheckBox* checkbox_3;
    wxCheckBox* checkbox_4;
    wxCheckBox* checkbox_5;
    wxCheckBox* checkbox_6;
    wxCheckBox* checkbox_9;
    wxStaticText* label_6;
    wxChoice* choice_4;
    wxRadioButton* radio_btn_1;
    wxRadioButton* radio_btn_2;
    wxRadioButton* radio_btn_3;
    wxCheckBox* checkbox_10;
    wxStaticText* label_7;
    wxChoice* choice_5;
    wxCheckBox* checkbox_15;
    wxCheckBox* checkbox_16;
    wxCheckBox* checkbox_17;
    wxCheckBox* checkbox_18;
    wxCheckBox* checkbox_19;
    wxStaticText* label_8;
    wxChoice* choice_6;
    wxRadioButton* radio_btn_4;
    wxRadioButton* radio_btn_5;
    wxRadioButton* radio_btn_6;
    wxCheckBox* checkbox_12;
    wxCheckBox* checkbox_13;
    wxCheckBox* checkbox_14;
    wxCheckBox* checkbox_20;
    wxCheckBox* checkbox_21;
    wxCheckBox* checkbox_22;
    wxCheckBox* checkbox_24;
    wxCheckBox* checkbox_23;
    wxCheckBox* checkbox_25;
    wxCheckBox* checkbox_26;
    wxButton* button_1;
    wxButton* button_2;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnChoiceType(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class HeightDialog1: public wxDialog {
public:
    // begin wxGlade: HeightDialog1::ids
    // end wxGlade

    HeightDialog1(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: HeightDialog1::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: HeightDialog1::attributes
    wxStaticText* label_9;
    wxTextCtrl* text_ctrl_5;
    wxSlider* slider_2;
    wxSlider* slider_3;
    wxStaticText* label_10;
    wxTextCtrl* text_ctrl_6;
    wxCheckBox* checkbox_11;
    // end wxGlade
}; // wxGlade: end class


class AnnotationDialog: public wxDialog {
public:
    // begin wxGlade: AnnotationDialog::ids
    // end wxGlade

    AnnotationDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: AnnotationDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: AnnotationDialog::attributes
    wxStaticText* label_12;
    wxChoice* choice_8;
    wxTextCtrl* text_ctrl_7;
    wxStaticText* label_11;
    wxChoice* choice_7;
    wxButton* button_3;
    wxButton* button_4;
    // end wxGlade
}; // wxGlade: end class


class ColorCustomize: public wxDialog {
public:
    // begin wxGlade: ColorCustomize::ids
    // end wxGlade

    ColorCustomize(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: ColorCustomize::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ColorCustomize::attributes
    wxStaticText* label_13;
    wxButton* button_7;
    wxPanel* panel_1;
    wxStaticText* label_14;
    wxButton* button_8;
    wxPanel* panel_2;
    wxStaticText* label_16;
    wxButton* button_9;
    wxPanel* panel_3;
    wxStaticText* label_15;
    wxButton* button_10;
    wxPanel* panel_4;
    wxStaticText* label_17;
    wxButton* button_11;
    wxPanel* panel_5;
    wxStaticText* label_18;
    wxButton* button_12;
    wxPanel* panel_6;
    wxButton* button_5;
    wxButton* button_6;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnBackground(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnGrid(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnLines(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPolygons(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnStrings(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoints(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnOk(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCancel(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class EditorPreferences: public wxDialog {
public:
    // begin wxGlade: EditorPreferences::ids
    // end wxGlade

    EditorPreferences(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: EditorPreferences::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: EditorPreferences::attributes
    wxStaticText* label_19;
    wxChoice* choice_9;
    wxCheckBox* checkbox_27;
    wxCheckBox* checkbox_28;
    wxCheckBox* checkbox_31;
    wxCheckBox* checkbox_30;
    wxCheckBox* checkbox_32;
    wxCheckBox* checkbox_33;
    wxCheckBox* checkbox_34;
    wxCheckBox* checkbox_35;
    wxCheckBox* checkbox_36;
    wxCheckBox* checkbox_29;
    wxStaticText* label_20;
    wxChoice* choice_10;
    wxCheckBox* checkbox_37;
    wxCheckBox* checkbox_38;
    wxButton* button_15;
    wxButton* button_13;
    wxButton* button_14;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnDefault(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnOk(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCancel(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class PlacementDialog: public wxDialog {
public:
    // begin wxGlade: PlacementDialog::ids
    // end wxGlade

    PlacementDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PlacementDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PlacementDialog::attributes
    wxStaticText* label_21;
    wxTextCtrl* text_ctrl_8;
    wxStaticText* label_22;
    wxTextCtrl* text_ctrl_9;
    wxStaticText* label_23;
    wxTextCtrl* text_ctrl_10;
    wxStaticText* label_24;
    wxTextCtrl* text_ctrl_11;
    wxStaticText* label_25;
    wxTextCtrl* text_ctrl_12;
    wxButton* button_17;
    wxButton* button_18;
    wxButton* button_19;
    wxCheckBox* checkbox_39;
    wxListCtrl* list_ctrl_1;
    wxButton* button_16;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnEditInitial(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditMinimum(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditMaximum(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditRandomCount(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnRandomChance(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnOk(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class TerminalDialog: public wxDialog {
public:
    // begin wxGlade: TerminalDialog::ids
    // end wxGlade

    TerminalDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: TerminalDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: TerminalDialog::attributes
    wxListBox* list_box_1;
    wxListBox* list_box_2;
    wxListBox* list_box_3;
    wxPanel* panel_7;
    wxTextCtrl* text_ctrl_13;
    wxButton* button_20;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnSelTerminal(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnSelScreen(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnSelInfo(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnOk(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class PointPropDialog: public wxDialog {
public:
    // begin wxGlade: PointPropDialog::ids
    // end wxGlade

    PointPropDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PointPropDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PointPropDialog::attributes
    wxStaticBox* sizer_52_staticbox;
    wxRadioButton* radio_btn_7;
    wxRadioButton* radio_btn_8;
    wxCheckBox* checkbox_40;
    wxStaticText* label_29;
    wxTextCtrl* text_ctrl_14;
    wxPanel* panel_8;
    wxStaticText* label_30;
    wxTextCtrl* text_ctrl_15;
    wxPanel* panel_9;
    wxStaticText* label_31;
    wxTextCtrl* text_ctrl_16;
    wxTextCtrl* text_ctrl_18;
    wxStaticText* label_32;
    wxTextCtrl* text_ctrl_17;
    wxTextCtrl* text_ctrl_19;
    wxStaticText* label_33;
    wxChoice* choice_11;
    wxButton* button_34;
    wxButton* button_37;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnSlidBtn(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnTransBtn(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnElevationBtn(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditFloor(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditCeiling(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditY(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditTX(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEditTY(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPolyChoice(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class PolygonPropDialog: public wxDialog {
public:
    // begin wxGlade: PolygonPropDialog::ids
    // end wxGlade

    PolygonPropDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PolygonPropDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PolygonPropDialog::attributes
    wxStaticText* label_34;
    wxTextCtrl* text_ctrl_20;
    wxStaticText* label_46;
    wxTextCtrl* text_ctrl_21;
    wxStaticText* label_35;
    wxChoice* choice_12;
    wxStaticText* label_47;
    wxTextCtrl* text_ctrl_22;
    wxStaticText* label_36;
    wxTextCtrl* text_ctrl_25;
    wxStaticText* label_48;
    wxTextCtrl* text_ctrl_23;
    wxStaticText* label_37;
    wxChoice* choice_13;
    wxStaticText* label_49;
    wxTextCtrl* text_ctrl_24;
    wxStaticText* label_38;
    wxChoice* choice_14;
    wxStaticText* label_26;
    wxTextCtrl* text_ctrl_33;
    wxStaticText* label_27;
    wxTextCtrl* text_ctrl_35;
    wxStaticText* label_39;
    wxTextCtrl* text_ctrl_26;
    wxStaticText* label_55;
    wxTextCtrl* text_ctrl_34;
    wxStaticText* label_28;
    wxTextCtrl* text_ctrl_36;
    wxStaticText* label_40;
    wxChoice* choice_19;
    wxStaticText* label_54;
    wxChoice* choice_15;
    wxStaticText* label_41;
    wxTextCtrl* text_ctrl_28;
    wxStaticText* label_53;
    wxChoice* choice_16;
    wxStaticText* label_42;
    wxTextCtrl* text_ctrl_29;
    wxStaticText* label_52;
    wxChoice* choice_20;
    wxStaticText* label_43;
    wxTextCtrl* text_ctrl_30;
    wxStaticText* label_51;
    wxChoice* choice_17;
    wxStaticText* label_44;
    wxTextCtrl* text_ctrl_31;
    wxStaticText* label_50;
    wxChoice* choice_18;
    wxButton* button_21;
    wxPanel* panel_10;
    wxPanel* panel_11;
    wxPanel* panel_12;
    wxStaticText* label_77;
    wxTextCtrl* text_ctrl_48;
    wxStaticText* label_78;
    wxTextCtrl* text_ctrl_49;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnIDEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFirstNeighborEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnTypeChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnNearCountEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPermuEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCenterXEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFloorLightEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCenterYEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCeilingLightEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFloorOriginXEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFloorOriginY(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnAreaEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFirstObjChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnMediaChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFirstExZoneEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnMediaLightChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnLineExZoneEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnSndSrcIndexChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFloorTransEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnAmbSndChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCeilingTransEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnRndSndChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPlatformBtn(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class PolygonNum: public wxDialog {
public:
    // begin wxGlade: PolygonNum::ids
    // end wxGlade

    PolygonNum(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PolygonNum::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PolygonNum::attributes
    wxBitmapButton* bitmap_button_1;
    wxBitmapButton* bitmap_button_2;
    wxBitmapButton* bitmap_button_3;
    wxBitmapButton* bitmap_button_4;
    wxBitmapButton* bitmap_button_5;
    wxBitmapButton* bitmap_button_6;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnPoly3(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoly4(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoly5(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoly6(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoly7(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnPoly8(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class ObjectPropDialog: public wxDialog {
public:
    // begin wxGlade: ObjectPropDialog::ids
    // end wxGlade

    ObjectPropDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: ObjectPropDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ObjectPropDialog::attributes
    wxStaticText* label_45;
    wxTextCtrl* text_ctrl_27;
    wxChoice* choice_21;
    wxStaticText* label_56;
    wxTextCtrl* text_ctrl_32;
    wxChoice* choice_22;
    wxCheckBox* checkbox_46;
    wxCheckBox* checkbox_47;
    wxCheckBox* checkbox_48;
    wxCheckBox* checkbox_49;
    wxCheckBox* checkbox_50;
    wxCheckBox* checkbox_51;
    wxStaticText* label_59;
    wxTextCtrl* text_ctrl_37;
    wxStaticText* label_58;
    wxTextCtrl* text_ctrl_38;
    wxPanel* panel_16;
    wxStaticText* label_60;
    wxChoice* choice_23;
    wxPanel* panel_17;
    wxStaticText* label_61;
    wxTextCtrl* text_ctrl_39;
    wxStaticText* label_63;
    wxTextCtrl* text_ctrl_41;
    wxStaticText* label_62;
    wxTextCtrl* text_ctrl_40;
    wxPanel* panel_18;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnTypeChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnIndexChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnHiddenCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCeilingCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnSeeCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnHearCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnAerialCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnNetCheck(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnLaunchChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnXEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnYEdit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnZEdit(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class SelectLevelDialog: public wxDialog {
public:
    // begin wxGlade: SelectLevelDialog::ids
    // end wxGlade

    SelectLevelDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: SelectLevelDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: SelectLevelDialog::attributes
    wxStaticText* label_57;
    wxListBox* list_box_4;
    wxButton* button_22;
    wxButton* button_23;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnList(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnOk(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCancel(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class LinePropDialog: public wxDialog {
public:
    // begin wxGlade: LinePropDialog::ids
    // end wxGlade

    LinePropDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: LinePropDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: LinePropDialog::attributes
    wxStaticBox* sizer_54_staticbox;
    wxStaticText* label_64;
    wxTextCtrl* text_ctrl_42;
    wxTextCtrl* text_ctrl_43;
    wxCheckBox* checkbox_45;
    wxCheckBox* checkbox_52;
    wxCheckBox* checkbox_53;
    wxCheckBox* checkbox_54;
    wxRadioBox* radio_box_1;
    wxStaticText* label_66;
    wxTextCtrl* text_ctrl_45;
    wxStaticText* label_67;
    wxTextCtrl* text_ctrl_46;
    wxStaticText* label_70;
    wxTextCtrl* text_ctrl_47;
    wxStaticText* label_71;
    wxTextCtrl* text_ctrl_50;
    wxStaticText* label_69;
    wxTextCtrl* text_ctrl_51;
    wxStaticText* label_72;
    wxTextCtrl* text_ctrl_52;
    wxStaticText* label_68;
    wxTextCtrl* text_ctrl_53;
    wxButton* button_35;
    wxButton* button_36;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnEndpoint1Edit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEndpoint2Edit(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnLandscape(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnElevation(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnVariableElev(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnHasTransSide(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnRadioSel(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnFloor(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCeiling(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class PolygonTypeDialog: public wxDialog {
public:
    // begin wxGlade: PolygonTypeDialog::ids
    // end wxGlade

    PolygonTypeDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PolygonTypeDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PolygonTypeDialog::attributes
    wxListCtrl* list_ctrl_2;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnSel(wxListEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class LevelInfoDialog: public wxDialog {
public:
    // begin wxGlade: LevelInfoDialog::ids
    // end wxGlade

    LevelInfoDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: LevelInfoDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: LevelInfoDialog::attributes
    wxStaticBox* sizer_59_staticbox;
    wxStaticBox* sizer_58_staticbox;
    wxStaticBox* sizer_60_staticbox;
    wxStaticText* label_65;
    wxTextCtrl* text_ctrl_44;
    wxStaticText* label_73;
    wxChoice* choice_28;
    wxStaticText* label_74;
    wxChoice* choice_29;
    wxCheckBox* checkbox_60;
    wxCheckBox* checkbox_62;
    wxCheckBox* checkbox_66;
    wxCheckBox* checkbox_63;
    wxCheckBox* checkbox_64;
    wxCheckBox* checkbox_65;
    wxCheckBox* checkbox_67;
    wxCheckBox* checkbox_61;
    wxCheckBox* checkbox_41;
    wxCheckBox* checkbox_42;
    wxCheckBox* checkbox_43;
    wxCheckBox* checkbox_44;
    wxCheckBox* checkbox_55;
    wxCheckBox* checkbox_56;
    wxCheckBox* checkbox_57;
    wxCheckBox* checkbox_58;
    wxCheckBox* checkbox_59;
    wxButton* button_28;
    wxButton* button_29;
    // end wxGlade
}; // wxGlade: end class


class HeightPaletteDialog: public wxDialog {
public:
    // begin wxGlade: HeightPaletteDialog::ids
    // end wxGlade

    HeightPaletteDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: HeightPaletteDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: HeightPaletteDialog::attributes
    wxButton* button_24;
    wxButton* button_25;
    wxListCtrl* list_ctrl_3;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnDelete(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEdit(wxListEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class MediaPaletteDialog: public wxDialog {
public:
    // begin wxGlade: MediaPaletteDialog::ids
    // end wxGlade

    MediaPaletteDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: MediaPaletteDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: MediaPaletteDialog::attributes
    wxButton* button_27;
    wxButton* button_26;
    wxListCtrl* list_ctrl_4;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnDelete(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnEdit(wxListEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class SidePropDialog: public wxDialog {
public:
    // begin wxGlade: SidePropDialog::ids
    // end wxGlade

    SidePropDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: SidePropDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: SidePropDialog::attributes
    wxStaticBox* sizer_57_copy_1_staticbox;
    wxStaticBox* sizer_57_copy_staticbox;
    wxStaticBox* sizer_57_staticbox;
    wxStaticBox* sizer_56_staticbox;
    wxStaticBox* sizer_55_staticbox;
    wxStaticText* label_80;
    wxTextCtrl* text_ctrl_55;
    wxStaticText* label_81;
    wxChoice* choice_27;
    wxCheckBox* checkbox_68;
    wxCheckBox* checkbox_69;
    wxCheckBox* checkbox_70;
    wxStaticText* label_87;
    wxChoice* choice_25;
    wxStaticText* label_88;
    wxChoice* choice_26;
    wxCheckBox* checkbox_71;
    wxCheckBox* checkbox_72;
    wxCheckBox* checkbox_73;
    wxCheckBox* checkbox_74;
    wxCheckBox* checkbox_75;
    wxStaticText* label_89;
    wxTextCtrl* text_ctrl_61;
    wxStaticText* label_93;
    wxTextCtrl* text_ctrl_65;
    wxStaticText* label_97;
    wxStaticText* label_90;
    wxTextCtrl* text_ctrl_62;
    wxStaticText* label_94;
    wxTextCtrl* text_ctrl_66;
    wxStaticText* label_98;
    wxStaticText* label_91;
    wxTextCtrl* text_ctrl_63;
    wxStaticText* label_95;
    wxTextCtrl* text_ctrl_67;
    wxStaticText* label_99;
    wxStaticText* label_92;
    wxTextCtrl* text_ctrl_64;
    wxStaticText* label_96;
    wxTextCtrl* text_ctrl_68;
    wxStaticText* label_100;
    wxStaticText* label_102;
    wxTextCtrl* text_ctrl_70;
    wxStaticText* label_82;
    wxTextCtrl* text_ctrl_57;
    wxStaticText* label_101;
    wxTextCtrl* text_ctrl_69;
    wxStaticText* label_83;
    wxTextCtrl* text_ctrl_58;
    wxStaticText* label_84;
    wxTextCtrl* text_ctrl_59;
    wxBitmapButton* bitmap_button_7;
    wxStaticText* label_85;
    wxTextCtrl* text_ctrl_60;
    wxStaticText* label_86;
    wxChoice* choice_24;
    wxStaticText* label_79;
    wxTextCtrl* text_ctrl_54;
    wxStaticText* label_103;
    wxTextCtrl* text_ctrl_56;
    wxBitmapButton* bitmap_button_7_copy;
    wxStaticText* label_85_copy;
    wxTextCtrl* text_ctrl_60_copy;
    wxStaticText* label_86_copy;
    wxChoice* choice_24_copy;
    wxStaticText* label_83_copy_1;
    wxTextCtrl* text_ctrl_58_copy_1;
    wxStaticText* label_84_copy_1;
    wxTextCtrl* text_ctrl_59_copy_1;
    wxBitmapButton* bitmap_button_7_copy_1;
    wxStaticText* label_85_copy_1;
    wxTextCtrl* text_ctrl_60_copy_1;
    wxStaticText* label_86_copy_1;
    wxChoice* choice_24_copy_1;
    wxButton* button_38;
    wxButton* button_39;
    // end wxGlade
}; // wxGlade: end class


class VisualDialog: public wxDialog {
public:
    // begin wxGlade: VisualDialog::ids
    // end wxGlade

    VisualDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: VisualDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: VisualDialog::attributes
    // end wxGlade
}; // wxGlade: end class


class LightPaletteDialog: public wxDialog {
public:
    // begin wxGlade: LightPaletteDialog::ids
    // end wxGlade

    LightPaletteDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: LightPaletteDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: LightPaletteDialog::attributes
    wxButton* button_31;
    wxButton* button_30;
    wxListCtrl* list_ctrl_5;
    // end wxGlade
}; // wxGlade: end class


class SoundPaletteDialog: public wxDialog {
public:
    // begin wxGlade: SoundPaletteDialog::ids
    // end wxGlade

    SoundPaletteDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: SoundPaletteDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: SoundPaletteDialog::attributes
    wxButton* button_32;
    wxButton* button_33;
    wxListCtrl* list_ctrl_6;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnDelete(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


class TextureDialog: public wxDialog {
public:
    // begin wxGlade: TextureDialog::ids
    // end wxGlade

    TextureDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: TextureDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: TextureDialog::attributes
    wxStaticText* label_75;
    wxChoice* choice_30;
    wxStaticText* label_76;
    wxChoice* choice_31;
    wxScrolledWindow* panel_13;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void OnType(wxCommandEvent &event); // wxGlade: <event_handler>
    void OnCollection(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // DIALOGUES_H
