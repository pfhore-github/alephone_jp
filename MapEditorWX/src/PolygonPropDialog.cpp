#include "PolygonPropDialog.h"
#include "PlatformDialog.h"

#include "MapEditorWX.h"
namespace PolyProp{
enum{
    ID_ID,
    ID_FirstNeighbor,
    ID_Type,
    ID_NearCount,
    ID_Perm,
    ID_CenterX,
    ID_FloorLight,
    ID_CenterY,
    ID_CeilingLight,
    ID_Area,
    ID_FloorOriginX,
    ID_FloorOriginY,
    ID_CeilingOriginX,
    ID_CeilingOriginY,
    ID_FirstObj,
    ID_Media,
    ID_FirstExZone,
    ID_MediaLight,
    ID_LineExZone,
    ID_SndSrcIndex,
    ID_FloorTrans,
    ID_AmbSnd,
    ID_CeilingTrans,
    ID_RndSnd,
    ID_Platform,
	ID_FloorHeight,
	ID_CeilingHeight,

};
};
BEGIN_EVENT_TABLE(PolygonPropDialog, wxDialog)
    EVT_TEXT(PolyProp::ID_ID, PolygonPropDialog::OnIDEdit)
    EVT_TEXT(PolyProp::ID_FirstNeighbor, PolygonPropDialog::OnFirstNeighborEdit)
    EVT_CHOICE(PolyProp::ID_Type, PolygonPropDialog::OnTypeChoice)
    EVT_TEXT(PolyProp::ID_NearCount, PolygonPropDialog::OnNearCountEdit)
    EVT_TEXT(PolyProp::ID_Perm, PolygonPropDialog::OnPermuEdit)
    EVT_TEXT(PolyProp::ID_CenterX, PolygonPropDialog::OnCenterXEdit)
    EVT_CHOICE(PolyProp::ID_FloorLight, PolygonPropDialog::OnFloorLightEdit)
    EVT_TEXT(PolyProp::ID_CenterY, PolygonPropDialog::OnCenterYEdit)
    EVT_CHOICE(PolyProp::ID_CeilingLight, PolygonPropDialog::OnCeilingLightEdit)
    EVT_TEXT(PolyProp::ID_Area, PolygonPropDialog::OnAreaEdit)
    EVT_TEXT(PolyProp::ID_FloorOriginX, PolygonPropDialog::OnFloorOriginXEdit)
    EVT_TEXT(PolyProp::ID_FloorOriginY, PolygonPropDialog::OnFloorOriginYEdit)
    EVT_TEXT(PolyProp::ID_CeilingOriginX, PolygonPropDialog::OnCeilingOriginXEdit)
    EVT_TEXT(PolyProp::ID_CeilingOriginY, PolygonPropDialog::OnCeilingOriginYEdit)
    EVT_CHOICE(PolyProp::ID_FirstObj, PolygonPropDialog::OnFirstObjChoice)
    EVT_CHOICE(PolyProp::ID_Media, PolygonPropDialog::OnMediaChoice)
    EVT_TEXT(PolyProp::ID_FirstExZone, PolygonPropDialog::OnFirstExZoneEdit)
    EVT_CHOICE(PolyProp::ID_MediaLight, PolygonPropDialog::OnMediaLightChoice)
    EVT_TEXT(PolyProp::ID_LineExZone, PolygonPropDialog::OnLineExZoneEdit)
    EVT_CHOICE(PolyProp::ID_SndSrcIndex, PolygonPropDialog::OnSndSrcIndexChoice)
    EVT_TEXT(PolyProp::ID_FloorTrans, PolygonPropDialog::OnFloorTransEdit)
    EVT_CHOICE(PolyProp::ID_AmbSnd, PolygonPropDialog::OnAmbSndChoice)
    EVT_TEXT(PolyProp::ID_CeilingTrans, PolygonPropDialog::OnCeilingTransEdit)
    EVT_CHOICE(PolyProp::ID_RndSnd, PolygonPropDialog::OnRndSndChoice)
    EVT_BUTTON(PolyProp::ID_Platform, PolygonPropDialog::OnPlatformBtn)
END_EVENT_TABLE()
PolygonPropDialog::PolygonPropDialog()
{
}
PolygonPropDialog::~PolygonPropDialog()
{
}
bool PolygonPropDialog::Create(wxWindow* parent, wxWindowID id)
{
    bool result = wxDialog::Create(parent, id, _T("Polygon Properties"));
    label_34 = new wxStaticText(this, wxID_ANY, wxT("Polygon ID"));
    text_ctrl_20 = new wxTextCtrl(this, PolyProp::ID_ID, wxEmptyString);
    label_46 = new wxStaticText(this, wxID_ANY, wxT("First neighbour"));
    text_ctrl_21 = new wxTextCtrl(this, PolyProp::ID_FirstNeighbor, wxEmptyString);
    label_35 = new wxStaticText(this, wxID_ANY, wxT("Type"));
    choice_12 = new wxChoice(this, PolyProp::ID_Type);
    label_47 = new wxStaticText(this, wxID_ANY, wxT("Near count"));
    text_ctrl_22 = new wxTextCtrl(this, PolyProp::ID_NearCount, wxEmptyString);
    label_36 = new wxStaticText(this, wxID_ANY, wxT("Permutation"));
    text_ctrl_25 = new wxTextCtrl(this, PolyProp::ID_Perm, wxEmptyString);
    label_48 = new wxStaticText(this, wxID_ANY, wxT("Center X"));
    text_ctrl_23 = new wxTextCtrl(this, PolyProp::ID_CenterX, wxEmptyString);
    label_37 = new wxStaticText(this, wxID_ANY, wxT("Floor light"));
    choice_13 = new wxChoice(this, PolyProp::ID_FloorLight);
    label_49 = new wxStaticText(this, wxID_ANY, wxT("Center Y"));
    text_ctrl_24 = new wxTextCtrl(this, PolyProp::ID_CenterY, wxEmptyString);
    label_38 = new wxStaticText(this, wxID_ANY, wxT("Ceiling light"));
    choice_14 = new wxChoice(this, PolyProp::ID_CeilingLight);
    label_26 = new wxStaticText(this, wxID_ANY, wxT("Floor origin"));
    text_ctrl_33 = new wxTextCtrl(this, PolyProp::ID_FloorOriginX, wxEmptyString);
    label_27 = new wxStaticText(this, wxID_ANY, wxT("x"));
    text_ctrl_35 = new wxTextCtrl(this, PolyProp::ID_FloorOriginY, wxEmptyString);
    label_39 = new wxStaticText(this, wxID_ANY, wxT("Area"));
    text_ctrl_26 = new wxTextCtrl(this, PolyProp::ID_Area, wxEmptyString);
    label_55 = new wxStaticText(this, wxID_ANY, wxT("Ceiling origin"));
    text_ctrl_34 = new wxTextCtrl(this, PolyProp::ID_CeilingOriginX, wxEmptyString);
    label_28 = new wxStaticText(this, wxID_ANY, wxT("x"));
    text_ctrl_36 = new wxTextCtrl(this, PolyProp::ID_CeilingOriginY, wxEmptyString);
    label_40 = new wxStaticText(this, wxID_ANY, wxT("First object"));
    choice_19 = new wxChoice(this, PolyProp::ID_FirstObj);
    label_54 = new wxStaticText(this, wxID_ANY, wxT("Media"));
    choice_15 = new wxChoice(this, PolyProp::ID_Media);
    label_41 = new wxStaticText(this, wxID_ANY, wxT("First ex zone"));
    text_ctrl_28 = new wxTextCtrl(this, PolyProp::ID_FirstExZone, wxEmptyString);
    label_53 = new wxStaticText(this, wxID_ANY, wxT("Media light"));
    choice_16 = new wxChoice(this, PolyProp::ID_MediaLight);
    label_42 = new wxStaticText(this, wxID_ANY, wxT("Line ex zone"));
    text_ctrl_29 = new wxTextCtrl(this, PolyProp::ID_LineExZone, wxEmptyString);
    label_52 = new wxStaticText(this, wxID_ANY, wxT("Snd src index"));
    choice_20 = new wxChoice(this, PolyProp::ID_SndSrcIndex);
    label_43 = new wxStaticText(this, wxID_ANY, wxT("Floor trans"));
    text_ctrl_30 = new wxTextCtrl(this, PolyProp::ID_FloorTrans, wxEmptyString);
    label_51 = new wxStaticText(this, wxID_ANY, wxT("Amb snd"));
    choice_17 = new wxChoice(this, PolyProp::ID_AmbSnd);
    label_44 = new wxStaticText(this, wxID_ANY, wxT("Ceiling trans"));
    text_ctrl_31 = new wxTextCtrl(this, PolyProp::ID_CeilingTrans, wxEmptyString);
    label_50 = new wxStaticText(this, wxID_ANY, wxT("Rnd snd"));
    choice_18 = new wxChoice(this, PolyProp::ID_RndSnd);
    button_21 = new wxButton(this, PolyProp::ID_Platform, wxT("Platform settings"));
    panel_10 = new wxPanel(this, wxID_ANY);
    panel_11 = new wxPanel(this, wxID_ANY);
    panel_12 = new wxPanel(this, wxID_ANY);
	textFloorHeight = new wxTextCtrl(this, PolyProp::ID_FloorHeight);
	textCeilingHeight = new wxTextCtrl(this, PolyProp::ID_CeilingHeight);

    choice_12->SetMinSize(wxSize(100, 22));
    choice_13->SetMinSize(wxSize(100, 22));
    choice_14->SetMinSize(wxSize(100, 22));
    text_ctrl_33->SetMinSize(wxSize(30, 20));
    text_ctrl_35->SetMinSize(wxSize(30, 20));
    text_ctrl_34->SetMinSize(wxSize(30, 20));
    text_ctrl_36->SetMinSize(wxSize(30, 20));
    choice_19->SetMinSize(wxSize(100, 22));
    choice_15->SetMinSize(wxSize(100, 22));
    choice_16->SetMinSize(wxSize(100, 22));
    choice_20->SetMinSize(wxSize(100, 22));
    choice_17->SetMinSize(wxSize(100, 22));
    choice_18->SetMinSize(wxSize(100, 22));

    wxFlexGridSizer* grid_sizer_13 = new wxFlexGridSizer(14, 4, 0, 0);
    wxGridSizer* grid_sizer_9 = new wxGridSizer(1, 3, 0, 0);
    wxGridSizer* grid_sizer_8 = new wxGridSizer(1, 3, 0, 0);
    grid_sizer_13->Add(label_34, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_20, 0, 0, 0);
    grid_sizer_13->Add(label_46, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_21, 0, 0, 0);
    grid_sizer_13->Add(label_35, 0, 0, 0);
    grid_sizer_13->Add(choice_12, 0, 0, 0);
    grid_sizer_13->Add(label_47, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_22, 0, 0, 0);
    grid_sizer_13->Add(label_36, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_25, 0, 0, 0);
    grid_sizer_13->Add(label_48, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_23, 0, 0, 0);
    grid_sizer_13->Add(label_37, 0, 0, 0);
    grid_sizer_13->Add(choice_13, 0, 0, 0);
    grid_sizer_13->Add(label_49, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_24, 0, 0, 0);
    grid_sizer_13->Add(label_38, 0, 0, 0);
    grid_sizer_13->Add(choice_14, 0, 0, 0);
    grid_sizer_13->Add(label_26, 0, 0, 0);
    grid_sizer_8->Add(text_ctrl_33, 0, 0, 0);
    grid_sizer_8->Add(label_27, 0, 0, 0);
    grid_sizer_8->Add(text_ctrl_35, 0, 0, 0);
    grid_sizer_13->Add(grid_sizer_8, 1, wxEXPAND, 0);
    grid_sizer_13->Add(label_39, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_26, 0, 0, 0);
    grid_sizer_13->Add(label_55, 0, 0, 0);
    grid_sizer_9->Add(text_ctrl_34, 0, 0, 0);
    grid_sizer_9->Add(label_28, 0, 0, 0);
    grid_sizer_9->Add(text_ctrl_36, 0, 0, 0);
    grid_sizer_13->Add(grid_sizer_9, 1, wxEXPAND, 0);
    grid_sizer_13->Add(label_40, 0, 0, 0);
    grid_sizer_13->Add(choice_19, 0, 0, 0);
    grid_sizer_13->Add(label_54, 0, 0, 0);
    grid_sizer_13->Add(choice_15, 0, 0, 0);
    grid_sizer_13->Add(label_41, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_28, 0, 0, 0);
    grid_sizer_13->Add(label_53, 0, 0, 0);
    grid_sizer_13->Add(choice_16, 0, 0, 0);
    grid_sizer_13->Add(label_42, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_29, 0, 0, 0);
    grid_sizer_13->Add(label_52, 0, 0, 0);
    grid_sizer_13->Add(choice_20, 0, 0, 0);
    grid_sizer_13->Add(label_43, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_30, 0, 0, 0);
    grid_sizer_13->Add(label_51, 0, 0, 0);
    grid_sizer_13->Add(choice_17, 0, 0, 0);
    grid_sizer_13->Add(label_44, 0, 0, 0);
    grid_sizer_13->Add(text_ctrl_31, 0, 0, 0);
    grid_sizer_13->Add(label_50, 0, 0, 0);
    grid_sizer_13->Add(choice_18, 0, 0, 0);

    grid_sizer_13->Add(new wxStaticText(this, wxID_ANY, wxT("Floor Height")), 0, 0, 0);
    grid_sizer_13->Add(textFloorHeight, 0, 0, 0);
    grid_sizer_13->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND, 0);
    grid_sizer_13->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND, 0);
    grid_sizer_13->Add(new wxStaticText(this, wxID_ANY, wxT("Ceiling Height")), 0, 0, 0);
    grid_sizer_13->Add(textCeilingHeight, 0, 0, 0);
    grid_sizer_13->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND, 0);
    grid_sizer_13->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND, 0);

	grid_sizer_13->Add(button_21, 0, 0, 0);
    grid_sizer_13->Add(panel_10, 1, wxEXPAND, 0);
    grid_sizer_13->Add(panel_11, 1, wxEXPAND, 0);
    grid_sizer_13->Add(panel_12, 1, wxEXPAND, 0);
    SetSizer(grid_sizer_13);
    grid_sizer_13->Fit(this);
    Layout();
    this->setPolyIndex(NONE);

    //Typeに追加
    for(int i = 0; i < NUMBER_OF_POLYGON_TYPE; i ++){
        choice_12->Insert(wxConvertMB2WX(wxGetApp().polygonTypeInfo[i].jname.c_str()), i);
    }

    return result;
}

void PolygonPropDialog::setPolyIndex(int index)
{
    this->polyIndex = index;
    setupDialog();
    //タイトル変更
    SetTitle(getString("Polygon prop [index:%d]", this->getPolyIndex()));
}
int PolygonPropDialog::getPolyIndex()
{
    return polyIndex;
}

/**
	コンボボックスの再構成
	時間がかかるので変化が起きた場合にのみ呼び出す
*/
void PolygonPropDialog::updateCombo()
{
    //CHOICEをセットアップしなおす
    //light floor/ceiling/media-light
    choice_13->Clear();
    choice_14->Clear();
    choice_16->Clear();
    for(int i = 0; i < (int)LightList.size(); i ++){
        choice_13->Insert(getString("%d", i), i);
        choice_14->Insert(getString("%d", i), i);
        choice_16->Insert(getString("%d", i), i);
    }
    choice_13->Insert(_T("NONE"), (int)LightList.size());
    choice_14->Insert(_T("NONE"), (int)LightList.size());
    choice_16->Insert(_T("NONE"), (int)LightList.size());

    //first object
    choice_19->Clear();
    for(int i = 0; i < (int)SavedObjectList.size(); i ++){
        choice_19->Insert(getString("%d", i), i);
    }
    choice_19->Insert(_T("NONE"), (int)LightList.size());

    //Media
    choice_15->Clear();
    for(int i = 0; i < (int)MediaList.size(); i ++){
        choice_15->Insert(getString("%d", i), i);
    }
    choice_15->Insert(_T("NONE"), (int)MediaList.size());
    
    //TODO snd src
    choice_20->Clear();
/*    for(int i = 0; i < (int).size(); i ++){
        choice_20->Insert(getString("%d", i), i);
    }
    choice_20->Insert(_T("NONE"), .size());*/
    choice_17->Clear();
    for(int i = 0; i < (int)AmbientSoundImageList.size(); i ++){
        choice_17->Insert(getString("%d", i), i);
    }
    choice_17->Insert(_T("NONE"), (int)AmbientSoundImageList.size());
    //random sound
    choice_18->Clear();
/*    for(int i = 0; i < (int).size(); i ++){
        choice_18->Insert(getString("%d", i), i);
    }
    choice_18->Insert(_T("NONE"), .size());*/
}

void PolygonPropDialog::setupDialog()
{


    if(this->getPolyIndex() == NONE){
        return;
    }
    //ポリゴン情報をダイアログに設定
    polygon_data* poly = get_polygon_data(this->getPolyIndex());
    text_ctrl_20->SetValue(getString("%d", this->getPolyIndex()));
    choice_12->SetSelection(poly->type);
    text_ctrl_25->SetValue(getString("%d", poly->permutation));
    choice_13->SetSelection(poly->floor_lightsource_index);
    choice_14->SetSelection(poly->ceiling_lightsource_index);
    text_ctrl_26->SetValue(getString("%d", poly->permutation));
    int index = poly->first_object;
    if(index == NONE){
        index = (int)SavedObjectList.size();
    }
    choice_19->SetSelection(index);
    text_ctrl_28->SetValue(getString("%d", poly->first_exclusion_zone_index));
    text_ctrl_29->SetValue(getString("%d", poly->line_exclusion_zone_count));
    text_ctrl_30->SetValue(getString("%d", poly->floor_transfer_mode));
    text_ctrl_31->SetValue(getString("%d", poly->ceiling_transfer_mode));

    text_ctrl_21->SetValue(getString("%d", poly->first_neighbor_index));
    text_ctrl_22->SetValue(getString("%d", poly->neighbor_count));
    text_ctrl_23->SetValue(getString("%d", poly->center.x));
    text_ctrl_24->SetValue(getString("%d", poly->center.y));
    text_ctrl_33->SetValue(getString("%d", poly->floor_origin.x));
    text_ctrl_35->SetValue(getString("%d", poly->floor_origin.y));
    text_ctrl_34->SetValue(getString("%d", poly->ceiling_origin.x));
    text_ctrl_36->SetValue(getString("%d", poly->ceiling_origin.y));
    index = poly->media_index;
    if(index == NONE){
        index = (int)MediaList.size();
    }
    choice_15->SetSelection(index);
    index = poly->media_lightsource_index;
    if(index == NONE){
        index = (int)LightList.size();
    }
    choice_16->SetSelection(index);
    /* TODO snd src
    index = poly->;
    if(index == NONE){
        index = .size();
    }
    choice_20->SetSelection(index);*/
    index = poly->ambient_sound_image_index;
    if(index == NONE){
        index = (int)AmbientSoundImageList.size();
    }
    choice_17->SetSelection(index);
/*
    TODO rnd snd
    index = poly->;
    if(index == NONE){
        index = .size();
    }
    choice_18->SetSelection(index);
    */
    textFloorHeight->SetValue(getString("%d", poly->floor_height));
    textCeilingHeight->SetValue(getString("%d", poly->ceiling_height));
}
void PolygonPropDialog::OnIDEdit(wxCommandEvent &event)
{
	//変更不可
}


void PolygonPropDialog::OnFirstNeighborEdit(wxCommandEvent &event)
{
	//変更不可
}


void PolygonPropDialog::OnTypeChoice(wxCommandEvent &event)
{
	polygon_data* poly = get_polygon_data(this->getPolyIndex());
    if(poly == NULL)	return ;
	int sel = event.GetSelection();
	if(sel >= 0){
		poly->type = sel;
	}
}


void PolygonPropDialog::OnNearCountEdit(wxCommandEvent &event)
{
	//auto calculate. not ediablt
}


void PolygonPropDialog::OnPermuEdit(wxCommandEvent &event)
{
	//TODO what's the permutation?
	//置換・配列・順序???
}


void PolygonPropDialog::OnCenterXEdit(wxCommandEvent &event)
{
	//auto calculation. not editable
}


void PolygonPropDialog::OnFloorLightEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFloorLightEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnCenterYEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnCenterYEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnCeilingLightEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnCeilingLightEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnAreaEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnAreaEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}
void PolygonPropDialog::OnFloorOriginXEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFloorOriginXEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}
void PolygonPropDialog::OnFloorOriginYEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFloorOriginYEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}
void PolygonPropDialog::OnCeilingOriginXEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnCeilingOriginXEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}
void PolygonPropDialog::OnCeilingOriginYEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnCeilingOriginYEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnFirstObjChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFirstObjChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnMediaChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnMediaChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnFirstExZoneEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFirstExZoneEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnMediaLightChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnMediaLightChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnLineExZoneEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnLineExZoneEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnSndSrcIndexChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnSndSrcIndexChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnFloorTransEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnFloorTransEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnAmbSndChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnAmbSndChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnCeilingTransEdit(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnCeilingTransEdit) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnRndSndChoice(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    std::cout<<"Event handler (PolygonPropDialog::OnRndSndChoice) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PolygonPropDialog::OnPlatformBtn(wxCommandEvent &event)
{
    if(this->getPolyIndex() == NONE){
        return ;
    }
    PlatformDialog dlg;
    dlg.Create(this, wxID_ANY, this->getPolyIndex());
    if(dlg.ShowModal() == wxID_OK){
        //TODO
        //設定変更
    }
}
void PolygonPropDialog::OnFloorHeightEdit(wxCommandEvent &event)
{
	//TODO
}
void PolygonPropDialog::OnceilingHeightEdit(wxCommandEvent &event)
{
	//TODO
}
