#include "stdafx.h"
#include "MapEditorSDI.h"

#include "MapEditorSDIDoc.h"
#include "MapEditorSDIView.h"
#include ".\mapeditorsdiview.h"
#include "SelectLevelDialog.h"


static void checkSelectPoint(POINT& mousePoint,
                             int offsetViewX, int offsetViewY,
                             int offsetWorldX, int offsetWorldY,
                             int div,
                             int distance)
{
    for(int i = 0; i < (int)EndpointList.size(); i ++){
        endpoint_data* ep = &EndpointList[i];
        int x = ep->vertex.x;
        int y = ep->vertex.y;
        int drawPoint[2];
        getViewPointFromWorldPoint2D(ep->vertex, drawPoint);

        if(isSelectPoint(mousePoint.x, mousePoint.y, x, y,
            offsetViewX, offsetViewY, offsetWorldX, offsetWorldY, div, distance))
        {
            SelPoint pdata;
            pdata.index = i;
            pdata.offset[0] = drawPoint[0] - mousePoint.x;
            pdata.offset[1] = drawPoint[1] - mousePoint.y;
            theApp.selectGroupInformation.points.push_back(pdata);
            theApp.selectGroupInformation.setSelected(true);
            return;
        }
    }
}

void CMapEditorSDIView::setStartPointForSelectGroup(int px, int py){
    theApp.selectStartPoint.x = px;
    theApp.selectStartPoint.y = py;
    theApp.isSelectingGroup = true;
}

/**
    左ボタンをドローモードで押す
*/
void CMapEditorSDIView::doLButtonDownDrawMode(UINT nFlags, CPoint &point)
{
    int OFFSET_X_VIEW = theApp.offset.x;
    int OFFSET_Y_VIEW = theApp.offset.y;
    int DIV = theApp.zoomDivision;
    if(theApp.selectingToolType == TI_ARROW){
        //selecting tool = TI_ARROW
        if(/*!(nFlags & MK_SHIFT) && */!(nFlags & MK_CONTROL)){
            //clik without modify key

            //group selected
            if(theApp.isSelectingGroup && theApp.selectGroupInformation.isSelected()){

                //is point in objects/points/lines/polygons?
                if(isPointInSelection(point.x, point.y,
                    OFFSET_X_VIEW, OFFSET_Y_VIEW, OFFSET_X_WORLD, OFFSET_Y_WORLD,
                    POINT_DISTANCE_EPSILON, LINE_DISTANCE_EPSILON, OBJECT_DISTANCE_EPSILON,
                    &theApp.selectGroupInformation, theApp.viewHeightMax, theApp.viewHeightMin,
                    DIV))
                {
                    //click on selecting group
                    //->remember offset, ready to move

                    //points
                    for(int i = 0; i < (int)theApp.selectGroupInformation.points.size(); i ++){
                        endpoint_data* ep = &EndpointList[theApp.selectGroupInformation.points[i].index];
                        //set pos to view
                        int x = ep->vertex.x;
                        int y = ep->vertex.y;
                        int drawX = (x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                        int drawY = (y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;

                        //sub to offset
                        theApp.selectGroupInformation.points[i].offset[0] = drawX - point.x;
                        theApp.selectGroupInformation.points[i].offset[1] = drawY - point.y;
                    }

                    //lines
                    for(int i = 0; i < (int)theApp.selectGroupInformation.lines.size(); i ++){
                        line_data* line = &LineList[theApp.selectGroupInformation.lines[i].index];
                        endpoint_data* begin = &EndpointList[line->endpoint_indexes[0]];
                        endpoint_data* end = &EndpointList[line->endpoint_indexes[1]];
                        //set offsets
                        theApp.selectGroupInformation.lines[i].offsets[0][0] =
                            (begin->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW
                            - point.x;
                        theApp.selectGroupInformation.lines[i].offsets[0][1] =
                            (begin->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW
                            - point.y;
                        theApp.selectGroupInformation.lines[i].offsets[1][0] =
                            (end->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW
                            - point.x;
                        theApp.selectGroupInformation.lines[i].offsets[1][1] =
                            (end->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW
                            - point.y;

                    }

                    //polygons
                    for(int i = 0; i < (int)theApp.selectGroupInformation.polygons.size(); i ++){
                        //set offset
                        polygon_data *polygon = &PolygonList[theApp.selectGroupInformation.polygons[i].index];
                        int num = polygon->vertex_count;
                        theApp.selectGroupInformation.polygons[i].num = num;
                        for(int j = 0; j < num; j ++){
                            int drawX = (EndpointList[polygon->endpoint_indexes[j]].vertex.x + OFFSET_X_WORLD) / DIV
                                + OFFSET_X_VIEW;
                            int drawY = (EndpointList[polygon->endpoint_indexes[j]].vertex.y + OFFSET_Y_WORLD) / DIV
                                + OFFSET_Y_VIEW;
                            theApp.selectGroupInformation.polygons[i].offsets[j][0] = drawX - point.x;
                            theApp.selectGroupInformation.polygons[i].offsets[j][1] = drawY - point.y;
                        }
                    }

                    //objects
                    for(int i = 0; i < (int)theApp.selectGroupInformation.selObjects.size(); i ++){
                        map_object* obj = &SavedObjectList[theApp.selectGroupInformation.selObjects[i].index];
                        //set pos to view
                        int x = obj->location.x;
                        int y = obj->location.y;
                        int drawX = (x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                        int drawY = (y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;

                        //sub to offset
                        theApp.selectGroupInformation.selObjects[i].offset[0] = drawX - point.x;
                        theApp.selectGroupInformation.selObjects[i].offset[1] = drawY - point.y;
                    }
                }else{
                    //release all selection
                    theApp.selectGroupInformation.clear();
                }

            }else {
                //shiftを押さずにクリック→選択
                theApp.selectGroupInformation.clear();

                theApp.isSelectingGroup = false;

                //objects
                {
                    for(int i = 0; i < (int)SavedObjectList.size(); i ++){
                        map_object* obj = &(SavedObjectList[i]);
                        int type = obj->type;
                        int facing = obj->facing;
                        int x = obj->location.x;
                        int y = obj->location.y;
                        int z = obj->location.z;
                        if(z > theApp.viewHeightMax ||
                            z < theApp.viewHeightMin){
                                continue;
                        }
                        if(isSelectPoint(point.x, point.y,
                            x, y, OFFSET_X_VIEW, OFFSET_Y_VIEW,
                            OFFSET_X_WORLD, OFFSET_Y_WORLD, DIV, OBJECT_DISTANCE_EPSILON))
                        {
                            int drawX = (x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                            int drawY = (y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;

                            SelObject odata;
                            odata.index = i;
                            odata.offset[0] = drawX - point.x;
                            odata.offset[1] = drawY - point.y;
                            theApp.selectGroupInformation.selObjects.push_back(odata);
                            theApp.selectGroupInformation.setSelected(true);
                            //選択したオブジェクトの情報を表示
                            theApp.objectPropertyDialog->setupDialog(i);
                            break;
                        }
                    }
                }
                if(theApp.selectGroupInformation.isSelected()){
                    //object property
                    theApp.objectPropertyDialog->ShowWindow(TRUE);

                }else{
                    //no selection
                    theApp.objectPropertyDialog->setSelectedObjectIndex(-1);
                }

                ////////////////////
                //points
                if(!theApp.selectGroupInformation.isSelected()){
                    checkSelectPoint(point, OFFSET_X_VIEW, OFFSET_Y_VIEW,
                        OFFSET_X_WORLD, OFFSET_Y_WORLD, DIV, POINT_DISTANCE_EPSILON);
                }
                if(theApp.selectGroupInformation.isSelected()){
                    //show point property dialog TODO
                }

                //////////////////
                //lines
                if(!theApp.selectGroupInformation.isSelected()){
                    for(int i = 0; i < (int)LineList.size(); i ++){
                        line_data* line = &LineList[i];
                        endpoint_data* begin = &EndpointList[line->endpoint_indexes[0]];
                        endpoint_data* end = &EndpointList[line->endpoint_indexes[1]];
                        if(isSelectLine(point.x, point.y, begin->vertex.x, begin->vertex.y,
                            end->vertex.x, end->vertex.y, OFFSET_X_VIEW, OFFSET_Y_VIEW,
                            OFFSET_X_WORLD, OFFSET_Y_WORLD, DIV, LINE_DISTANCE_EPSILON)){
                            //接近している
                            SelLine ldata;
                            ldata.index = i;

                            //オフセット記録
                            ldata.offsets[0][0] = 
                                (begin->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW
                                - point.x;
                            ldata.offsets[0][1] = 
                                (begin->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW
                                - point.y;
                            ldata.offsets[1][0] = 
                                (end->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW
                                - point.x;
                            ldata.offsets[1][1] = 
                                (end->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW
                                - point.y;
                            theApp.selectGroupInformation.lines.push_back(ldata);
                            theApp.selectGroupInformation.setSelected(true);

                            break;
                        }
                    }
                }

                if(theApp.selectGroupInformation.isSelected()){
                    //線プロパティ表示
                }else{
                    //非表示
                }

                /////////////////
                //polygons
                if(!theApp.selectGroupInformation.isSelected()){
                    struct world_point2d world_point = getWorldPoint2DFromViewPoint(point.x, point.y);
                    int polygonIndex = getPolygonIdPointIn(world_point);
                    if(polygonIndex != NONE){
                        polygon_data *polygon = &PolygonList[polygonIndex];

                        SelPolygon polydata;
                        polydata.index = polygonIndex;
                        polydata.num = polygon->vertex_count;

                        
                        //show polygon property
                        theApp.polygonPropertyDialog->ShowWindow(TRUE);
                        theApp.polygonPropertyDialog->setupDialog(polygonIndex);

                        //set offset
                        for(int j = 0; j < polygon->vertex_count; j ++){
                            int drawPoint[2];
                            getViewPointFromWorldPoint2D(EndpointList[polygon->endpoint_indexes[j]].vertex,
                                drawPoint);
                            int drawX = drawPoint[0];
                            int drawY = drawPoint[1];
                            polydata.offsets[j][0] = drawX - point.x;
                            polydata.offsets[j][1] = drawY - point.y;
                        }

                        theApp.selectGroupInformation.polygons.push_back(polydata);
                        theApp.selectGroupInformation.setSelected(true);
                    }
                }

                //////////////////////////////////
                //ここからの処理はほかよりも後に書く
                if(!theApp.selectGroupInformation.isSelected()){
                    //範囲選択
                    //始点登録
                    setStartPointForSelectGroup(point.x, point.y);

                    theApp.selectGroupInformation.clear();
                }else{
                    theApp.isSelectingGroup = false;
                }
            }
        }
    }else if(theApp.selectingToolType == TI_FILL){
        //TODO

        //塗れるポリゴンっぽい線の集合を探す
        int* lines = new int[8];
        delete lines;
    }else if(theApp.selectingToolType == TI_HAND){
        //全体を移動
    }else if( theApp.selectingToolType == TI_LINE){
        if(nFlags & MK_LBUTTON){
            theApp.selectGroupInformation.clear();
            world_point2d worldPoint = getWorldPoint2DFromViewPoint(point.x, point.y);
            int settledPointIndex = addPoint(worldPoint);
            if(settledPointIndex == NONE){
                //追加失敗
                theApp.isFirstOfLineToAdd = true;
            }else{
                if(theApp.isFirstOfLineToAdd){
                    theApp.isFirstOfLineToAdd = false;
                    //add point(first)
                }else{
                    //add point and line
                    //add line
                    addLine(theApp.previousPointIndex, settledPointIndex);
                }
                theApp.previousPointIndex = settledPointIndex;
            }
        }
    }else if(theApp.selectingToolType == TI_MAGNIFY){
        //zoom in/out
        if(nFlags & MK_CONTROL){
            //with ctrl
            //->zoom out
            OnZoomOut();
        }else{
            //zoom in
            OnZoomIn();
        }
    }else if(theApp.selectingToolType == TI_SKULL){
        struct world_point2d world_point = getWorldPoint2DFromViewPoint(point.x, point.y);

        int polygonIndex = getPolygonIdPointIn(world_point);
        if(polygonIndex != NONE){
            polygon_data *polygon = &PolygonList[polygonIndex];
            //invalid height -> skip
            if(polygon->floor_height > theApp.viewHeightMax ||
                polygon->ceiling_height < theApp.viewHeightMin){
            }else{
                //add object on the polygon
                int objectIndex = addObject(world_point, polygonIndex);

                //選択状態にする
                theApp.selectGroupInformation.clear();
                SelObject odata;
                odata.index = objectIndex;
                odata.offset[0] = odata.offset[1] = 0;
                theApp.selectGroupInformation.setSelected(true);
                theApp.selectGroupInformation.selObjects.push_back(odata);

                //選択したオブジェクトの情報を表示
                theApp.objectPropertyDialog->setupDialog(objectIndex);
            }
        }
    }else if(theApp.selectingToolType == TI_TEXT){
        //add annotation
        //show dialog
        CAnnotationDialog dlg(this);
        if(dlg.DoModal() == IDOK){
            struct world_point2d world_point = getWorldPoint2DFromViewPoint(point.x, point.y);

            int polygonIndex = getPolygonIdPointIn(world_point);

            if(dlg.isOnlyDisplayWhenPolygonShown){
            }else{
                polygonIndex = NONE;
            }
            char cstr[MAXIMUM_ANNOTATION_TEXT_LENGTH];
            strToChar(dlg.annotationText, cstr);
            addAnnotationText(world_point, string(cstr));

        }
        //focus this window
        this->SetFocus();
    }else if(theApp.selectingToolType == TI_POLYGON){
        //set start point
        setStartPointForSelectGroup(point.x, point.y);
    }
}

void checkPolygonSelect(UINT nFlags, CPoint point)
{
    theApp.selectGroupInformation.clear();

    struct world_point2d world_point = getWorldPoint2DFromViewPoint(point.x, point.y);
    int polygonIndex = getPolygonIdPointIn(world_point);
    if(polygonIndex != NONE){
        polygon_data *polygon = &PolygonList[polygonIndex];

        SelPolygon polydata;
        polydata.index = polygonIndex;
        polydata.num = polygon->vertex_count;

        
        //show polygon property
        theApp.polygonPropertyDialog->ShowWindow(TRUE);
        theApp.polygonPropertyDialog->setupDialog(polygonIndex);

        theApp.selectGroupInformation.polygons.push_back(polydata);
        theApp.selectGroupInformation.setSelected(true);
    }
}

//左ボタン下げ
void CMapEditorSDIView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
    if(nFlags & MK_CONTROL){
        //コントロールボタンを押している
        theApp.isPressLButtonWithCtrl = true;
    }else{
        theApp.isPressLButtonWithCtrl = false;
    }
    theApp.nowMousePoint = point;
    theApp.oldMousePoint = point;

    int OFFSET_X_VIEW = theApp.offset.x;
    int OFFSET_Y_VIEW = theApp.offset.y;
    int DIV = theApp.zoomDivision;


    switch(theApp.getEditMode()){
    case EM_DRAW:
        if(theApp.selectingToolType != TI_LINE){
            theApp.isFirstOfLineToAdd = true;
            theApp.previousPointIndex = NONE;
        }
        if(theApp.selectingToolType == TI_HAND){
            theApp.isPressLButtonWithCtrl = true;
        }
        if(nFlags & MK_CONTROL){
        }else{
            doLButtonDownDrawMode(nFlags, point);
        }
        break;
    case EM_POLYGON_TYPE:
        checkPolygonSelect(nFlags, point);
        break;
    case EM_FLOOR_HEIGHT:
        //change height
        break;
    case EM_CEILING_HEIGHT:
        //change height
        break;
    case EM_FLOOR_LIGHT:
        //change light
        break;
    case EM_CEILING_LIGHT:
        //change light
        break;
    case EM_MEDIA:
        //change media
        break;
    case EM_FLOOR_TEXTURE:
        //change texture
        break;
    case EM_CEILING_TEXTURE:
        //change texture
        break;
    default:
        AfxMessageBox(L"Illigal edit mode!");
    }
    this->Invalidate(FALSE);
    CView::OnLButtonDown(nFlags, point);
    SetCapture();
}

void CMapEditorSDIView::moveMapOffset(int newPx, int newPy){
    int deltaX = newPx - theApp.oldMousePoint.x;
    int deltaY = newPy - theApp.oldMousePoint.y;
    theApp.offset.x += deltaX;
    theApp.offset.y += deltaY;
}
/****************************************************************/
//マウス移動
void CMapEditorSDIView::OnMouseMove(UINT nFlags, CPoint point)
{
    setCursor();

    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
    theApp.nowMousePoint = point;

    const int DIV = theApp.zoomDivision;

    if(nFlags & MK_LBUTTON && ((nFlags & MK_CONTROL && theApp.isPressLButtonWithCtrl) ||
        (theApp.getEditMode() == EM_DRAW && theApp.selectingToolType == TI_HAND))){
        //Control+L=move map view
        moveMapOffset(point.x, point.y);
    }else{
        if(theApp.getEditMode() == EM_DRAW){
            if(theApp.selectingToolType == TI_ARROW){
                if(!theApp.isPressLButtonWithCtrl && 
                    (nFlags & MK_LBUTTON) && /*!(nFlags & MK_CONTROL) && */
                    !(nFlags & MK_CONTROL))
                {
                    //選択物の移動
                    //world position of mouse pointer
                    int x = (point.x - theApp.offset.x) * theApp.zoomDivision - OFFSET_X_WORLD;
                    int y = (point.y - theApp.offset.y) * theApp.zoomDivision - OFFSET_Y_WORLD;
                    if(theApp.selectGroupInformation.isSelected()){
                        //グループを選択している
                        //->グループを移動
                        //points
                        for(int i = 0; i < (int)theApp.selectGroupInformation.points.size(); i ++){
                            struct SelPoint *selData = &theApp.selectGroupInformation.points[i];
                            EndpointList[selData->index].vertex.x = x + selData->offset[0] * DIV;
                            EndpointList[selData->index].vertex.y = y + selData->offset[1] * DIV;
                        }
                        //lines
                        for(int i = 0; i < (int)theApp.selectGroupInformation.lines.size(); i ++){
                            struct SelLine *selData = &theApp.selectGroupInformation.lines[i];
                            EndpointList[LineList[selData->index].endpoint_indexes[0]].vertex.x =
                                x + selData->offsets[0][0] * DIV;
                            EndpointList[LineList[selData->index].endpoint_indexes[0]].vertex.y =
                                y + selData->offsets[0][1] * DIV;
                            EndpointList[LineList[selData->index].endpoint_indexes[1]].vertex.x =
                                x + selData->offsets[1][0] * DIV;
                            EndpointList[LineList[selData->index].endpoint_indexes[1]].vertex.y =
                                y + selData->offsets[1][1] * DIV;
                        }
                        //polygons
                        for(int i = 0; i < (int)theApp.selectGroupInformation.polygons.size(); i ++){
                            struct SelPolygon *selData = &theApp.selectGroupInformation.polygons[i];
                            for(int j = 0; j < selData->num; j ++){
                                EndpointList[PolygonList[selData->index].endpoint_indexes[j]].vertex.x =
                                    x + selData->offsets[j][0] * DIV;
                                EndpointList[PolygonList[selData->index].endpoint_indexes[j]].vertex.y =
                                    y + selData->offsets[j][1] * DIV;
                            }
                        }
                        //objects
                        for(int i = 0; i < (int)theApp.selectGroupInformation.selObjects.size(); i ++){
                            struct SelObject *selData = &theApp.selectGroupInformation.selObjects[i];
                            SavedObjectList[selData->index].location.x = x + selData->offset[0] * DIV;
                            SavedObjectList[selData->index].location.y = y + selData->offset[1] * DIV;
                        }
                        
                    }
                }
            }else if(theApp.selectingToolType == TI_FILL){
            }else if(theApp.selectingToolType == TI_HAND){
                if(nFlags & MK_LBUTTON){
                    moveMapOffset(point.x, point.y);
                }
            }else if(theApp.selectingToolType == TI_LINE){
                //点を踏んでないか確認
                int endpointIndex = NONE;
                bool found = false;
                world_point2d world_point = getWorldPoint2DFromViewPoint(point.x, point.y);
                for(int i = 0; i < (int)EndpointList.size(); i ++){
                    endpoint_data *point = &EndpointList[i];
                    //check height
                    if( point->highest_adjacent_floor_height > theApp.viewHeightMax ||
                        point->lowest_adjacent_ceiling_height < theApp.viewHeightMin)
                    {
                        continue;
                    }
                    if(isSelectPoint(world_point, point->vertex, POINT_DISTANCE_EPSILON * theApp.zoomDivision))
                    {
                        theApp.isNowOnThePoint = true;
                        found = true;
                        break;
                    }
                }
                if(!found){
                    theApp.isNowOnThePoint = false;
                }
            }else if(theApp.selectingToolType == TI_MAGNIFY){
            }else if(theApp.selectingToolType == TI_POLYGON){
            }else if(theApp.selectingToolType == TI_SKULL){
            }else if(theApp.selectingToolType == TI_TEXT){
            }
        }else{
        }
    }// if/not ctrled
    Invalidate(FALSE);
    theApp.oldMousePoint = point;

    CView::OnMouseMove(nFlags, point);
}

//左ボタン上げ
void CMapEditorSDIView::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
    theApp.isPressLButtonWithCtrl = false;

    if(theApp.selectingToolType == TI_ARROW){
        bool okSelect = false;
        if(theApp.selectGroupInformation.isSelectOneObject()){
            //オブジェクト情報更新
            theApp.objectPropertyDialog->setupDialog(theApp.selectGroupInformation.selObjects[0].index);
        }else if(theApp.selectGroupInformation.isSelectOnePoint()){
        }else if(theApp.selectGroupInformation.isSelectOneLine()){
        }else if(theApp.selectGroupInformation.isSelectOnePolygon()){
            theApp.polygonPropertyDialog->setupDialog(theApp.selectGroupInformation.polygons[0].index);
        }
        if(theApp.isSelectingGroup){
            /*if(isNearbyPoints(point.x, point.y, 
                theApp.selectStartPoint.x, theApp.selectStartPoint.y, SELECT_GROUP_DISTANCE_THRESHOLD))
            {
                //
                theApp.selectGroupInformation.setSelected(false);
            }else{
                okSelect = true;
            }*/
            okSelect = true;
        }
        if(okSelect){
            //if selecting is only 1 object. setup property dialog

            theApp.selectGroupInformation.clear();

            int DIV = theApp.zoomDivision;
            int OFFSET_X_VIEW = theApp.offset.x;
            int OFFSET_Y_VIEW = theApp.offset.y;
            //選択されているものをリストに登録する
            //点
            for(int i = 0; i < (int)EndpointList.size(); i ++){
                endpoint_data* ep = &EndpointList[i];
                int x = ep->vertex.x;
                int y = ep->vertex.y;
                int drawX = (x + OFFSET_X_WORLD)/DIV + OFFSET_X_VIEW;
                int drawY = (y + OFFSET_Y_WORLD)/DIV + OFFSET_Y_VIEW;
                //チェック
                if(isPointInRect<int>(drawX, drawY, point.x, point.y,
                    theApp.selectStartPoint.x, theApp.selectStartPoint.y))
                {
                    //追加
                    struct SelPoint selData;
                    selData.index = i;
                    //オフセットは次に左ボタンを下げたときに設定する
                    theApp.selectGroupInformation.points.push_back(selData);
                }
            }
            //線
            for(int i = 0; i < (int)LineList.size(); i ++){
                line_data* line = &LineList[i];
                endpoint_data* begin = &EndpointList[line->endpoint_indexes[0]];
                endpoint_data* end = &EndpointList[line->endpoint_indexes[1]];
                int x0 = (begin->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                int y0 = (begin->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;
                int x1 = (end->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                int y1 = (end->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;
                if(isLineInRect(x0, y0, x1, y1, point.x, point.y,
                    theApp.selectStartPoint.x, theApp.selectStartPoint.y))
                {
                    struct SelLine selData;
                    selData.index = i;
                    //オフセットは次に左ボタンを下げたときに設定する
                    theApp.selectGroupInformation.lines.push_back(selData);
                }
            }

            //ポリゴン
            //POINT points[MAXIMUM_VERTICES_PER_POLYGON][2];
            for(int i = 0; i < (int)PolygonList.size(); i ++){
                struct polygon_data* polygon = &PolygonList[i];
                int vertexCount = polygon->vertex_count;
                bool inner = true;
                if(vertexCount == 0){
                    inner = false;
                }
                for(int j = 0; j < vertexCount && inner; j ++){
                    endpoint_data* ep = &EndpointList[polygon->endpoint_indexes[j]];
                    int drawX = (ep->vertex.x + OFFSET_X_WORLD) / DIV + OFFSET_X_VIEW;
                    int drawY = (ep->vertex.y + OFFSET_Y_WORLD) / DIV + OFFSET_Y_VIEW;
                    if(!isPointInRect<int>(drawX, drawY, point.x, point.y,
                        theApp.selectStartPoint.x, theApp.selectStartPoint.y))
                    {
                        inner = false;
                        break;
                    }
                }
                if(inner){
                    //登録
                    struct SelPolygon selData;
                    selData.index = i;
                    //オフセットは次に左ボタンを下げたときに設定する
                    theApp.selectGroupInformation.polygons.push_back(selData);
                }
            }

            //objects
            for(int i = 0; i < (int)SavedObjectList.size(); i ++){
                map_object* obj = &SavedObjectList[i];
                int x = obj->location.x;
                int y = obj->location.y;
                int drawX = (x + OFFSET_X_WORLD)/DIV + OFFSET_X_VIEW;
                int drawY = (y + OFFSET_Y_WORLD)/DIV + OFFSET_Y_VIEW;

                //チェック
                if(isPointInRect<int>(drawX, drawY, point.x, point.y,
                    theApp.selectStartPoint.x, theApp.selectStartPoint.y))
                {
                    //追加
                    struct SelObject selData;
                    selData.index = i;
                    //オフセットは次に左ボタンを下げたときに設定する
                    theApp.selectGroupInformation.selObjects.push_back(selData);
                }
            }

            //選択したかどうか
            bool isSelected = false;
            if(theApp.selectGroupInformation.points.size() > 0 ||
                theApp.selectGroupInformation.selObjects.size() > 0){
                    isSelected = true;
            }
            theApp.selectGroupInformation.setSelected(isSelected);
        }
    }else if(theApp.selectingToolType == TI_FILL){
    }else if(theApp.selectingToolType == TI_HAND){
    }else if(theApp.selectingToolType == TI_LINE){
    }else if(theApp.selectingToolType == TI_MAGNIFY){
    }else if(theApp.selectingToolType == TI_POLYGON){
        if(theApp.isSelectingGroup){
            //create preset polygon

        }
    }else if(theApp.selectingToolType == TI_SKULL){
    }else if(theApp.selectingToolType == TI_TEXT){
    }

    theApp.isSelectingGroup = false;
    Invalidate(FALSE);
    ReleaseCapture();

    CView::OnLButtonUp(nFlags, point);
}

// press event of Right button
void CMapEditorSDIView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

    theApp.nowMousePoint = point;
    theApp.oldMousePoint = point;

    int OFFSET_X_VIEW = theApp.offset.x;
    int OFFSET_Y_VIEW = theApp.offset.y;
    int DIV = theApp.zoomDivision;

    if(theApp.selectingToolType != TI_LINE){
        theApp.isFirstOfLineToAdd = true;
        theApp.previousPointIndex = NONE;
    }

    if(theApp.selectingToolType == TI_ARROW){
    }else if(theApp.selectingToolType == TI_FILL){
    }else if(theApp.selectingToolType == TI_HAND){
    }else if( theApp.selectingToolType == TI_LINE){
        if(nFlags & MK_RBUTTON){
            //stop adding points
            theApp.isFirstOfLineToAdd = true;
            theApp.previousPointIndex = NONE;
        }
    }else if(theApp.selectingToolType == TI_MAGNIFY){
    }else if(theApp.selectingToolType == TI_SKULL){
    }else if(theApp.selectingToolType == TI_TEXT){
    }else if(theApp.selectingToolType == TI_POLYGON){

    }
    Invalidate(FALSE);
    CView::OnRButtonDown(nFlags, point);
    SetCapture();
}

void CMapEditorSDIView::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

    Invalidate(FALSE);
    ReleaseCapture();
    CView::OnRButtonUp(nFlags, point);
}

