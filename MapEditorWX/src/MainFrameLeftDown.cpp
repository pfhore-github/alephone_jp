#include "MapEditorMainFrame.h"
#include "AnnotationDialog.h"

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/**
    ���{�^�����������Ƃ��ɌĂ΂��
*/
void MapEditorMainFrame::OnLeftDown(wxMouseEvent &ev)
{
    //���E�_�E�I�u�W�F�N�g�E�|���S���ESide�̎w�����������
    linePropDialog.setLineIndex(NONE);
    pointPropDialog.setIndex(NONE);
    objPropDialog.setObjIndex(NONE);
    polyPropDialog.setPolyIndex(NONE);
    sidePropDialog.setIndex(NONE);

    //�J�[�\���ݒ�
    wxGetApp().setCursor();
    //�}�E�X���W�L�^
    wxGetApp().getViewGridManager()->setNewMousePoint(ev.m_x, ev.m_y);
    bool shift = ev.ShiftDown();
    bool ctrl = ev.ControlDown();

//    int toolType = wxGetApp().getEventManager()->getToolType();

    if(ctrl){
        Refresh();
        return;
    }
    switch(wxGetApp().getEventManager()->getEditModeType()){
    case EditModeType::EM_DRAW:
        doLButtonOnDrawMode(ev);
        break;
    case EditModeType::EM_POLYGON_TYPE:
        doLButtonOnPolygonMode(ev);
        break;
    case EditModeType::EM_FLOOR_HEIGHT:
        doLButtonOnFloorHeightMode(ev);
        break;
    case EditModeType::EM_CEILING_HEIGHT:
        doLButtonOnCeilingHeightMode(ev);
        break;
    case EditModeType::EM_FLOOR_LIGHT:
        doLButtonOnFloorLightMode(ev);
        break;
    case EditModeType::EM_CEILING_LIGHT:
        doLButtonOnCeilingLightMode(ev);
        break;
    case EditModeType::EM_MEDIA:
        doLButtonOnMediaMode(ev);
        break;
    case EditModeType::EM_FLOOR_TEXTURE:
        doLButtonOnFloorTextureMode(ev);
        break;
    case EditModeType::EM_CEILING_TEXTURE:
        doLButtonOnCeilingTextureMode(ev);
        break;
    }
    Refresh();
}
void MapEditorMainFrame::doLButtonOnDrawMode(wxMouseEvent& ev)
{
    int tool = wxGetApp().getEventManager()->getToolType();
    switch(tool){
    case ToolType::TI_ARROW:
        doLButtonOnArrowTool(ev);
        break;
    case ToolType::TI_FILL:
        doLButtonOnFillTool(ev);
        break;
    case ToolType::TI_HAND:
        doLButtonOnHandTool(ev);
        break;
    case ToolType::TI_LINE:
        doLButtonOnLineTool(ev);
        break;
    case ToolType::TI_MAGNIFY:
        doLButtonOnMagnifyTool(ev);
        break;
    case ToolType::TI_SKULL:
        doLButtonOnSkullTool(ev);
        break;
    case ToolType::TI_TEXT:
        doLButtonOnTextTool(ev);
        break;
    case ToolType::TI_POLYGON:
        doLButtonOnPolygonTool(ev);
        break;
    default:
        hpl::error::halt("Invalid tool type");
    }
}

void MapEditorMainFrame::doLButtonOnArrowTool(wxMouseEvent& ev)
{
    //�I���f�[�^
    hpl::aleph::map::HPLSelectData* sel = &wxGetApp().selectData;
    
    //�O���b�h�}�l�[�W���[
    hpl::aleph::view::HPLViewGridManager* vmgr = wxGetApp().getViewGridManager();
    //�r���[�I�t�Z�b�g
    int voffset[2];
    vmgr->getOffset(voffset);
    int div = vmgr->getZoomDivision();

    int zMin = vmgr->getViewHeightMin();
    int zMax = vmgr->getViewHeightMax();
    int mx = ev.m_x;
    int my = ev.m_y;

    if(sel->isSelected()){
        //���ɑI��

        //�}�E�X���W���I�𕔕��Ɋ܂܂�邩�`�F�b�N
        if(hpl::aleph::map::isPointInSelection(mx, my,
            voffset[0], voffset[1], OFFSET_X_WORLD, OFFSET_Y_WORLD,
            POINT_DISTANCE_EPSILON, LINE_DISTANCE_EPSILON, OBJECT_DISTANCE_EPSILON,
            sel, zMax, zMin, div))
        {
            //�I�𕔕����N���b�N���Ă���
            //���I�t�Z�b�g��ݒ肷��
            //<en> clicked on selection datas
            //-> set offsets
            hpl::aleph::map::setupSelectDataGroupOffsets(mx, my,
                &wxGetApp().selectData, voffset[0], voffset[1],
                OFFSET_X_WORLD, OFFSET_Y_WORLD, div);
            return;
        }else{
            //�N���b�N���Ă��Ȃ�
            //���I������
            //<en> no click on selection datas
            //-> release all selections
            sel->clear();
        }
    }
    //�����I�����Ă��Ȃ����
    //���I���ł��邩�����Ă݂܂�
    if(this->tryToSelectOneItem(ev)){
        //�I��͈͉͂������܂�
        wxGetApp().getEventManager()->setSelectingGroup(false);
    }else{
        //�I������Ȃ�����
        //�͈͑I���̊J�n
        wxGetApp().getEventManager()->setSelectGroupStartPoint(mx, my);
        //�I�����̉���
        sel->clear();
    }
}


/**
    @param ev
    @return �I���ɐ��������ꍇ�^
*/
bool MapEditorMainFrame::tryToSelectOneItem(wxMouseEvent& ev)
{
    //�V�t�g�L�[
    bool shift = ev.ShiftDown();

    hpl::aleph::map::HPLSelectData* sel = &wxGetApp().selectData;
    //�C�x���g�}�l�[�W���[
    hpl::aleph::HPLEventManager* emgr = wxGetApp().getEventManager();
    hpl::aleph::view::HPLViewGridManager* vmgr = wxGetApp().getViewGridManager();

    //�r���[�I�t�Z�b�g
    int voffset[2];
    vmgr->getOffset(voffset);
    int div = vmgr->getZoomDivision();

    int zMin = vmgr->getViewHeightMin();
    int zMax = vmgr->getViewHeightMax();
    int mx = ev.m_x;
    int my = ev.m_y;

    if(!shift){
        //�V�t�g�L�[���������ɃN���b�N�������U�������
        sel->clear();
        emgr->setSelectingGroup(false);
    }

    //�I���̗D�揇�ʂ�
    //1:�I�u�W�F�N�g
    //2:�_
    //3:��
    //4:�|���S��


    //�I�u�W�F�N�g
    for(int i = 0; i < (int)SavedObjectList.size(); i ++){
        if(wxGetApp().getStockManager()->delObjects[i]){
            continue;
        }
        map_object* obj = &SavedObjectList[i];
        int x = obj->location.x;
        int y = obj->location.y;
        int z = obj->location.z;

        //�������͈͊O���ǂ���
        if(z > zMax || z < zMin){
            continue;
        }

        //�_�I������
        if(hpl::aleph::map::isSelectPoint(mx, my,
            x, y, voffset[0], voffset[1],
            OFFSET_X_WORLD, OFFSET_Y_WORLD, div, OBJECT_DISTANCE_EPSILON))
        {
            //
            int vpoint[2];
            wxGetApp().getViewPointFromWorldPoint(x, y, vpoint);
            int offset[2];
            offset[0] = vpoint[0] - mx;
            offset[1] = vpoint[1] - my;
            sel->addSelObject(i, offset);
            //�I�u�W�F�N�g�̃v���p�e�B�E�_�C�A���O��\������
            //TODO
            this->objPropDialog.setObjIndex(i);
            this->objPropDialog.Show(true);
            return true;
        }
    }

    //no obj selected
    //TODO �I��ID�֘A�̎���
    this->objPropDialog.setObjIndex(NONE);


    //////////
    //�_
    for(int i = 0; i < (int)EndpointList.size(); i ++){
        if(wxGetApp().getStockManager()->delPoints[i]){
            continue;
        }
        endpoint_data* ep = get_endpoint_data(i);

        //�����`�F�b�N
        int floor = ep->highest_adjacent_floor_height;
        int ceiling = ep->lowest_adjacent_ceiling_height;
        if(floor > zMax || ceiling < zMin){
            continue;
        }
        if(hpl::aleph::map::isSelectPoint(mx, my,
            ep->vertex.x, ep->vertex.y,
            voffset[0], voffset[1], OFFSET_X_WORLD, OFFSET_Y_WORLD,
            div, POINT_DISTANCE_EPSILON))
        {
            //��������
            int vpoint[2];
            wxGetApp().getViewPointFromWorldPoint(ep->vertex, vpoint);
            int offset[2];
            offset[0] = vpoint[0] - mx;
            offset[1] = vpoint[1] - my;
            sel->addSelPoint(i, offset);
            return true;
        }
    }

    /////////////////////////
    //lines
    for(int i = 0; i < (int)LineList.size(); i ++){
        if(wxGetApp().getStockManager()->delLines[i]){
            continue;
        }
        line_data* line = get_line_data(i);
        endpoint_data* start = get_endpoint_data(line->endpoint_indexes[0]);
        endpoint_data* end = get_endpoint_data(line->endpoint_indexes[1]);

        //�����`�F�b�N
        int floor = line->highest_adjacent_floor;
        int ceiling = line->lowest_adjacent_ceiling;
        if(floor > zMax || ceiling < zMin){
            continue;
        }

        if(hpl::aleph::map::isSelectLine(mx, my,
            start->vertex.x, start->vertex.y, end->vertex.x, end->vertex.y,
            voffset[0], voffset[1], OFFSET_X_WORLD, OFFSET_Y_WORLD, div, LINE_DISTANCE_EPSILON))
        {
            //�I��
            int vstart[2];
            int vend[2];
            wxGetApp().getViewPointFromWorldPoint(start->vertex, vstart);
            wxGetApp().getViewPointFromWorldPoint(end->vertex, vend);

            int offset[2][2];
            offset[0][0] = vstart[0] - mx;
            offset[0][1] = vstart[1] - my;
            offset[1][0] = vend[0] - mx;
            offset[1][1] = vend[1] - my;
            sel->addSelLine(i, offset);
            return true;
        }
    }

    //�|���S��
    //TODO �������Ƀ\�[�g����
    int polyIndex = NONE;
    for(int i = 0; i < (int)PolygonList.size(); i ++){
        if(wxGetApp().getStockManager()->delPolygons[i]){
            continue;
        }
        if(hpl::aleph::map::isPointInPolygon(mx, my,
            i, OFFSET_X_WORLD, OFFSET_Y_WORLD, div,
            voffset[0], voffset[1]))
        {
            polyIndex = i;
            break;
        }
    }
    if(polyIndex != NONE){
        polygon_data* poly = get_polygon_data(polyIndex);
        int n = poly->vertex_count;
        int offset[MAXIMUM_VERTICES_PER_POLYGON][2];
        
        //�|���S���v���p�e�B�\��
        //TODO
        this->polyPropDialog.setPolyIndex(polyIndex);
        this->polyPropDialog.Show(true);

        //�I�t�Z�b�g
        for(int j = 0; j < n; j ++){
            int vpoint[2];
            wxGetApp().getViewPointFromWorldPoint(get_endpoint_data(poly->endpoint_indexes[j])->vertex, vpoint);
            offset[j][0] = vpoint[0] - mx;
            offset[j][1] = vpoint[1] - my;
        }

        sel->addSelPolygon(polyIndex, offset, n);
        return true;
    }

    return false;
}


/////////////////////////////////////////////////////////////
void MapEditorMainFrame::doLButtonOnFillTool(wxMouseEvent& ev)
{
    hpl::aleph::HPLStockManager* smgr = wxGetApp().getStockManager();

    int mx = ev.m_x;
    int my = ev.m_y;
    world_point2d wmp = wxGetApp().getWorldPointFromViewPoint(mx, my);

    hpl::aleph::view::HPLViewGridManager* vmgr = wxGetApp().getViewGridManager();
    int zMin = vmgr->getViewHeightMin();
    int zMax = vmgr->getViewHeightMax();
    //TODO
    //�h��Ԃ��\�ȃ|���S���f�[�^���擾���܂�
    std::vector<polygon_data> polyDatas = hpl::aleph::map::searchValidPolygon(
        wmp, smgr, zMin, zMax);
    if(polyDatas.size() == 0){
        //���s
        hpl::error::caution("No frame to fill as a VALID polygon found (it seems to be a illegal polygon or a none)");
    }else{
        //�ǉ�
        hpl::aleph::map::addPolygon(polyDatas[0]);
        //�X�V
        smgr->updateDeletes();
    }
}
void MapEditorMainFrame::doLButtonOnHandTool(wxMouseEvent& ev)
{
    //MouseMotion�Ɋۓ���
    
}
void MapEditorMainFrame::doLButtonOnLineTool(wxMouseEvent& ev)
{
    //���ǉ�
#ifdef MAP_VIEWER
#else
    //�I������
    wxGetApp().selectData.clear();

    hpl::aleph::view::HPLViewGridManager* vmgr = wxGetApp().getViewGridManager();

    int mx = ev.m_x;
    int my = ev.m_y;
    //���E���W�ɂ���
    world_point2d wpoint = wxGetApp().getWorldPointFromViewPoint(mx, my);

    bool isFirst = wxGetApp().isFirstOfLineToAdd;

    hpl::aleph::HPLEventManager* emgr = wxGetApp().getEventManager();
    
    int voffset[2];
    vmgr->getOffset(voffset);
    int div = wxGetApp().getViewGridManager()->getZoomDivision();
    int zMin = vmgr->getViewHeightMin();
    int zMax = vmgr->getViewHeightMax();

    hpl::aleph::HPLStockManager* smgr = wxGetApp().getStockManager();

    //�d�Ȃ�_�����邩�ǂ����`�F�b�N
    int pointIndex = hpl::aleph::map::getSelectPointIndex(
        wpoint, POINT_DISTANCE_EPSILON, zMin, zMax, div, smgr);
    //�d�Ȃ�������邩���肷��
    int lineIndex = hpl::aleph::map::getSelectLineIndex(
        wpoint, LINE_DISTANCE_EPSILON, zMin, zMax, div, smgr);

    if(pointIndex != NONE)
    {
        //�����̓_���N���b�N���Ă���
        /*
        1:�_�̏�ŃN���b�N
	        1:�ŏ��̃N���b�N
		        �O��N���b�N�_�̍X�V
	        2:���ڈȍ~�̃N���b�N
		        1:���ɐ������݂��Ă���
			        �������Ȃ�
		        2:���͑��݂��Ă��Ȃ�
			        ���̒ǉ�
		        �O��N���b�N�_�̍X�V
        */
        if(wxGetApp().isFirstOfLineToAdd){
            //�ŏ��̃N���b�N
            //�n�_�Ƃ���
            wxGetApp().prevPointIndex = pointIndex;

        }else{
            //2��ڈȍ~�̃N���b�N
            //�����_���N���b�N���Ă��邩�H
            if(wxGetApp().prevPointIndex == pointIndex){
                //�n�_�Ƃ���
            }else{
                //���ɐ������݂��Ă��邩�H
                int lineIndex = hpl::aleph::map::getLineIndexFromTwoLPoints(
                    wxGetApp().prevPointIndex, pointIndex);
                if(lineIndex != NONE){
                    //���ɐ������݂��Ă���
                    //�������Ȃ�
                }else{
                    //�łȂ��ΐ����쐬����
                    line_data line;
                    hpl::aleph::map::createLine(wxGetApp().prevPointIndex,
                        pointIndex, &line);
                    int newLineIndex = hpl::aleph::map::addLine(line);
                }
                wxGetApp().prevPointIndex = pointIndex;
            }
        }

        //�ŏ��̃N���b�N�ł͂Ȃ��Ȃ���
        wxGetApp().isFirstOfLineToAdd = false;
    }else{
        //���𓥂�ł��Ȃ����`�F�b�N���܂�
        if(lineIndex != NONE){
            /*
                2:���̏�ŃN���b�N
	                1:�ŏ��̃N���b�N
		                1:�|���S���ɏ��������
			                �x�����o���ĉ������Ȃ�
		                2:�������Ȃ���
			                ���̐����폜
			                �_��ǉ�
			                ���̎n�_���V�_���I�_�Ƃ����V����ǉ�
			                �V�_�����̏I�_���I�_�Ƃ����V����ǉ�
			                �O��N���b�N�_�̍X�V
	                2:���ڈȍ~�̃N���b�N
		                ����
            */
            line_data* line = get_line_data(lineIndex);
            assert(line != NULL);

            //�|���S���ɏ������Ă��邩�m���߂܂�
            if(line->clockwise_polygon_owner != NONE || 
                line->counterclockwise_polygon_owner != NONE)
            {
                //�|���S���ɑ������
                //�x�����o���B���𕪒f�����肵�Ȃ�
                hpl::error::caution("cannot divide line , because line belongs to polygon (line's index:%d, clockwise polygon's index:%d, counterclockwise polygon's index:%d)",
                    lineIndex, line->clockwise_polygon_owner, line->counterclockwise_polygon_owner);
                //�Ɨ������_��ǉ�����
                endpoint_data ep;
                hpl::aleph::map::createPoint(wpoint, &ep);
                int newPointIndex = hpl::aleph::map::addEndpoint(ep);
                assert(newPointIndex != NONE);
            }else{
                //�n�_�A�I�_�̏��擾
                //endpoint_data* begin = get_endpoint_data(line->endpoint_indexes[0]);
                //endpoint_data* end = get_endpoint_data(line->endpoint_indexes[1]);
                int epStartIndex = line->endpoint_indexes[0];
                int epEndIndex = line->endpoint_indexes[1];

                //�����폜
                smgr->deleteLine(lineIndex);
                smgr->updateDeletes();

                //�_��ǉ�
                endpoint_data ep;
                hpl::aleph::map::createPoint(wpoint, &ep);
                int newPointIndex = hpl::aleph::map::addEndpoint(ep);
#ifdef __WXDEBUG__
                wxASSERT(newPointIndex != NONE);
#endif
                endpoint_data *newEp = get_endpoint_data(newPointIndex);

                //�n�_���_�̐���ǉ�
                line_data newLine1;
                hpl::aleph::map::createLine(epStartIndex,
                    newPointIndex, &newLine1);
                int newLine1Index = hpl::aleph::map::addLine(newLine1);
#ifdef __WXDEBUG__
                wxASSERT(newLine1Index != NONE);
#endif

                //�_���I�_�̐���ǉ�
                line_data newLine2;
                hpl::aleph::map::createLine(newPointIndex, epEndIndex,
                    &newLine2);
                int newLine2Index = hpl::aleph::map::addLine(newLine2);
#ifdef __WXDEBUG__
                wxASSERT(newLine2Index != NONE);
#endif


                if(wxGetApp().isFirstOfLineToAdd){
                    //�ŏ��Ȃ�Ȃɂ����Ȃ�
                }else{
                    //�O�񁨍���̓_�̊Ԃɐ���ǉ�
                    line_data newLine;
                    hpl::aleph::map::createLine(wxGetApp().prevPointIndex, newPointIndex, &newLine);
                    int lineIndex = hpl::aleph::map::addLine(newLine);
                }
                wxGetApp().prevPointIndex = newPointIndex;
            }
            wxGetApp().isFirstOfLineToAdd = false;

        }else{
            //�V�K�ǉ�
            endpoint_data ep;
            hpl::aleph::map::createPoint(wpoint, &ep);
            int newPointIndex = hpl::aleph::map::addEndpoint(ep);
            if(wxGetApp().isFirstOfLineToAdd){
                //�ŏ��̓_�Ȃ̂Œǉ����Ȃ�
            }else{
                //����ǉ�����
                line_data line;
                hpl::aleph::map::createLine(wxGetApp().prevPointIndex, newPointIndex, &line);
                int newLineIndex = hpl::aleph::map::addLine(line);
            }
            
            wxGetApp().isFirstOfLineToAdd = false;
            wxGetApp().prevPointIndex = newPointIndex;
        }
    }

    //�����X�V����
    wxGetApp().getStockManager()->updateDeletes();
    
#endif
}
void MapEditorMainFrame::doLButtonOnMagnifyTool(wxMouseEvent& ev)
{
    bool shift = ev.ShiftDown();
    wxCommandEvent dummy;
    if(shift){
        //�k��
        this->OnZoomOut(dummy);
    }else{
        this->OnZoomIn(dummy);
    }
}
void MapEditorMainFrame::doLButtonOnSkullTool(wxMouseEvent& ev)
{
    //�I�u�W�F�N�g�z�u
#ifdef MAP_VIEWER
#else
    int mx = ev.m_x;
    int my = ev.m_y;
    world_point2d wpoint = wxGetApp().getWorldPointFromViewPoint(mx, my);

    hpl::aleph::HPLStockManager* smgr = wxGetApp().getStockManager();

    for(int i = 0; i < (int)PolygonList.size(); i ++){
        if(smgr->delPolygons[i]){
            continue;
        }
        polygon_data* poly = get_polygon_data(i);
        if(hpl::aleph::map::isPointInPolygon(wpoint, i)){
            //�_���܂܂��
            //���̃|���S����ɒu��
            //TODO
            /*int flags = 0;
            map_object objv = this->objPropDialog.getObject();
            int type = this->objPropDialog.getObject();
            hpl::aleph::map::createObject(wpoint, i, &obj, flags, type, index);
            int newIndex = hpl::aleph::map::addMapSavedObject(obj);*/
        }
    }
#endif
}
void MapEditorMainFrame::doLButtonOnTextTool(wxMouseEvent& ev)
{
#ifdef MAP_VIEWER
#else
    //�A�m�e�[�V�����ǉ�
    //�_�C�A���O����
    AnnotationDialog dlg;
    map_annotation sample;
    //������
    memset(&sample, 0, SIZEOF_map_annotation);

    //�}�E�X���W�̈ʒu�ɒǉ�����
    int viewX = ev.m_x;
    int viewY = ev.m_y;
    world_point2d wpoint = wxGetApp().getWorldPointFromViewPoint(viewX, viewY);
    sample.location.x = wpoint.x;
    sample.location.y = wpoint.y;
    sample.polygon_index = NONE;

    //�_�C�A���O�\��
    dlg.Create(this, ID_ANNOTATION_DIALOG, sample);
    if(dlg.ShowModal() == wxID_OK){
        //����
        //�A�m�e�[�V�����f�[�^���擾
        map_annotation newAnnotation = dlg.getAnnotation();
        //�ǉ�
        hpl::aleph::map::addAnnotation(newAnnotation);
    }
    //�����X�V����
    wxGetApp().getStockManager()->updateDeletes();

#endif
}
void MapEditorMainFrame::doLButtonOnPolygonTool(wxMouseEvent& ev)
{
#ifdef MAP_VIEWER
#else
    //�͈͑I���J�n
    //TODO
    wxGetApp().getEventManager()->setSelectingGroup(true);
    wxGetApp().getEventManager()->setSelectGroupStartPoint(ev.m_x, ev.m_y);
#endif
}

void MapEditorMainFrame::doLButtonOnPolygonMode(wxMouseEvent& ev)
{
#ifdef MAP_VIEWER
#else
#endif
}
void MapEditorMainFrame::doLButtonOnFloorHeightMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnCeilingHeightMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnFloorLightMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnCeilingLightMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnMediaMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnFloorTextureMode(wxMouseEvent& ev)
{
}
void MapEditorMainFrame::doLButtonOnCeilingTextureMode(wxMouseEvent& ev)
{
}