#include "PlacementDialog.h"
#include "MapEditorWX.h"

enum{
    ID_INITIAL,
    ID_MINIMUM,
    ID_MAXIMUM,
    ID_RANDOM_COUNT,
    ID_RANDOM_CHANCE,
    ID_INF,
    ID_ALL_INF,
    ID_SET_INITIAL,
};

BEGIN_EVENT_TABLE(PlacementDialog, wxDialog)
    EVT_TEXT(ID_INITIAL, PlacementDialog::OnEditInitial)
    EVT_TEXT(ID_MINIMUM, PlacementDialog::OnEditMinimum)
    EVT_TEXT(ID_MAXIMUM, PlacementDialog::OnEditMaximum)
    EVT_TEXT(ID_RANDOM_COUNT, PlacementDialog::OnEditRandomCount)
    EVT_TEXT(ID_RANDOM_CHANCE, PlacementDialog::OnRandomChance)
    EVT_BUTTON(ID_INF, PlacementDialog::OnOk)
    EVT_BUTTON(ID_ALL_INF, PlacementDialog::OnOk)
    EVT_BUTTON(ID_SET_INITIAL, PlacementDialog::OnOk)
    EVT_BUTTON(wxID_OK, PlacementDialog::OnOk)
END_EVENT_TABLE()

PlacementDialog::PlacementDialog():wxDialog()
{
}

PlacementDialog::~PlacementDialog()
{
}

const int COLUMN_NUM = 7;

static void addListItem(wxListCtrl* lstctrl, object_frequency_definition* placement, wxString type,
                        int id){
    wxString str[COLUMN_NUM];
    str[0] = type;
    str[1] = getString("%d", placement->minimum_count);
    str[2] = getString("%d", placement->maximum_count);
    str[3] = getString("%d", placement->initial_count);
    str[4] = getString("%d", placement->random_count);
    str[5] = getString("%d", placement->random_chance);
    str[6] = getString("%d", placement->flags);

    for(int j = 0; j < COLUMN_NUM; j ++){
        wxListItem item;
        item.SetId(id);
        item.SetColumn(j);
        item.SetText(str[j]);
        if(j == 0){
            lstctrl->InsertItem(item);
        }else{
            lstctrl->SetItem(item);
        }
    }
}
bool PlacementDialog::Create(wxWindow* parent, wxWindowID id)
{
    bool result = wxDialog::Create(parent, id, _T("Placement"));
    if(!result){
        return result;
    }
    label_21 = new wxStaticText(this, wxID_ANY, wxT("Initial"));
    text_ctrl_8 = new wxTextCtrl(this, ID_INITIAL, wxEmptyString);
    label_22 = new wxStaticText(this, wxID_ANY, wxT("Minimum"));
    text_ctrl_9 = new wxTextCtrl(this, ID_MINIMUM, wxEmptyString);
    label_23 = new wxStaticText(this, wxID_ANY, wxT("Maximum"));
    text_ctrl_10 = new wxTextCtrl(this, ID_MAXIMUM, wxEmptyString);
    label_24 = new wxStaticText(this, wxID_ANY, wxT("Random count"));
    text_ctrl_11 = new wxTextCtrl(this, ID_RANDOM_COUNT, wxEmptyString);
    label_25 = new wxStaticText(this, wxID_ANY, wxT("Random chance"));
    text_ctrl_12 = new wxTextCtrl(this, ID_RANDOM_CHANCE, wxEmptyString);
    button_17 = new wxButton(this, ID_INF, wxT("Supply this infinitly"));
    button_18 = new wxButton(this, ID_ALL_INF, wxT("We cannot exterminate all monsters"));
    button_19 = new wxButton(this, ID_SET_INITIAL, wxT("Set the number of monster to initial"));
    checkbox_39 = new wxCheckBox(this, wxID_ANY, wxT("place at random location"));
    list_ctrl_1 = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);
    button_16 = new wxButton(this, wxID_OK, wxT("OK"));

    list_ctrl_1->SetMinSize(wxSize(600, 300));

    wxFlexGridSizer* grid_sizer_1 = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* grid_sizer_4 = new wxFlexGridSizer(4, 1, 0, 0);
    wxFlexGridSizer* grid_sizer_3 = new wxFlexGridSizer(5, 2, 0, 0);
    grid_sizer_3->Add(label_21, 0, 0, 0);
    grid_sizer_3->Add(text_ctrl_8, 0, 0, 0);
    grid_sizer_3->Add(label_22, 0, 0, 0);
    grid_sizer_3->Add(text_ctrl_9, 0, 0, 0);
    grid_sizer_3->Add(label_23, 0, 0, 0);
    grid_sizer_3->Add(text_ctrl_10, 0, 0, 0);
    grid_sizer_3->Add(label_24, 0, 0, 0);
    grid_sizer_3->Add(text_ctrl_11, 0, 0, 0);
    grid_sizer_3->Add(label_25, 0, 0, 0);
    grid_sizer_3->Add(text_ctrl_12, 0, 0, 0);
    grid_sizer_2->Add(grid_sizer_3, 1, wxEXPAND, 0);
    grid_sizer_4->Add(button_17, 0, 0, 0);
    grid_sizer_4->Add(button_18, 0, 0, 0);
    grid_sizer_4->Add(button_19, 0, 0, 0);
    grid_sizer_4->Add(checkbox_39, 0, 0, 0);
    grid_sizer_2->Add(grid_sizer_4, 1, wxEXPAND, 0);
    grid_sizer_1->Add(grid_sizer_2, 1, wxEXPAND, 0);
    grid_sizer_1->Add(list_ctrl_1, 1, wxEXPAND, 0);
    grid_sizer_1->Add(button_16, 0, wxALIGN_RIGHT, 0);
    SetSizer(grid_sizer_1);
    grid_sizer_1->Fit(this);
    Layout();

    //�f�[�^�ݒ�
    char columnNames[][100] = {
        "Type", "Min", "Max", "Initial", 
        "Rnd Count", "Rnd Chance", "Flags"
    };
    for(int i = 0; i < COLUMN_NUM; i ++){
        list_ctrl_1->InsertColumn(i, wxConvertMB2WX(columnNames[i]));
    }
    for(int i = 0; i < NUMBER_OF_DEFINED_ITEMS; i ++){
        object_frequency_definition* placement = hpl::aleph::map::getPlacementData(_saved_item, i);
        addListItem(this->list_ctrl_1, placement, 
            wxConvertMB2WX(wxGetApp().itemTypeInfo[i].jname.c_str()), i);
    }
    for(int i = 0; i < NUMBER_OF_MONSTER_TYPES; i ++){
        object_frequency_definition* placement = hpl::aleph::map::getPlacementData(_saved_monster, i);
        addListItem(this->list_ctrl_1, placement, 
            wxConvertMB2WX(wxGetApp().monsterTypeInfo[i].jname.c_str()),
            i + NUMBER_OF_DEFINED_ITEMS);
    }
    return result;
}
//OK�{�^����������
void PlacementDialog::OnOk(wxCommandEvent& ev)
{
    SetReturnCode(wxID_OK);
    Destroy();
}
void PlacementDialog::OnInf(wxCommandEvent &ev)
{
    //TODO
}
void PlacementDialog::OnAllInf(wxCommandEvent &ev)
{
    //TODO
}
void PlacementDialog::OnSetInitial(wxCommandEvent &ev)
{
    //TODO
}
void PlacementDialog::OnEditInitial(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (PlacementDialog::OnEditInitial) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PlacementDialog::OnEditMinimum(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (PlacementDialog::OnEditMinimum) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PlacementDialog::OnEditMaximum(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (PlacementDialog::OnEditMaximum) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PlacementDialog::OnEditRandomCount(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (PlacementDialog::OnEditRandomCount) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void PlacementDialog::OnRandomChance(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (PlacementDialog::OnRandomChance) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}