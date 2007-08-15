#include "SelectLevelDialog.h"
#include "MapEditorWX.h"
enum{
    ID_List,
};

BEGIN_EVENT_TABLE(SelectLevelDialog, wxDialog)
    EVT_LISTBOX(ID_List, SelectLevelDialog::OnList)
//    EVT_BUTTON(wxID_OK, SelectLevelDialog::OnOk)
    EVT_BUTTON(wxID_CANCEL, SelectLevelDialog::OnCancel)
END_EVENT_TABLE()
SelectLevelDialog::SelectLevelDialog()
{
}
SelectLevelDialog::~SelectLevelDialog()
{
}
bool SelectLevelDialog::Create(wxWindow* parent, wxWindowID id)
{
    bool result = wxDialog::Create(parent, id, _T("Select Level"));
    label_57 = new wxStaticText(this, wxID_ANY, wxT("Select level"));

    list_box_4 = new wxListBox(this, ID_List);
//    button_22 = new wxButton(this, wxID_OK, wxEmptyString);
    button_23 = new wxButton(this, wxID_CANCEL, wxEmptyString);

    list_box_4->SetMinSize(wxSize(320, 320));

    wxFlexGridSizer* grid_sizer_17 = new wxFlexGridSizer(3, 1, 0, 0);
    wxFlexGridSizer* grid_sizer_25 = new wxFlexGridSizer(1, 2, 0, 0);
    grid_sizer_17->Add(label_57, 0, 0, 0);
    grid_sizer_17->Add(list_box_4, 0, wxEXPAND, 0);
//    grid_sizer_25->Add(button_22, 0, wxRIGHT|wxALIGN_RIGHT, 0);
    grid_sizer_25->Add(button_23, 0, wxRIGHT|wxALIGN_RIGHT, 0);
    grid_sizer_17->Add(grid_sizer_25, 1, wxALIGN_RIGHT, 0);
    SetSizer(grid_sizer_17);
    grid_sizer_17->Fit(this);
    Layout();

    for(int i = 0; i < (int)wxGetApp().levelNameList.size(); i ++){
        list_box_4->Insert(getString("%d_%s", i,
			wxConvertMB2WX(wxGetApp().levelNameList[i].c_str())), i);
    }
    if(wxGetApp().levelNameList.size() == 0){
        list_box_4->Insert(_T("unknown"), 0);
    }
    return result;
}
void SelectLevelDialog::OnList(wxCommandEvent &event)
{
    int sel = event.GetSelection();
    this->selectLevel = sel;

    //�I��
    SetReturnCode(wxID_OK);
    Destroy();
}
int SelectLevelDialog::getSelectLevel()
{
    return this->selectLevel;
}


void SelectLevelDialog::OnOk(wxCommandEvent &event)
{
    event.Skip();
    std::cout<<"Event handler (SelectLevelDialog::OnOk) not implemented yet"<<std::endl; //notify the user that he hasn't implemented the event handler yet
}


void SelectLevelDialog::OnCancel(wxCommandEvent &event)
{
    SetReturnCode(wxID_CANCEL);
    Destroy();
}