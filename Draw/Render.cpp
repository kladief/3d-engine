#include "Render.h"
polyAngles* Render::getPolyAngles(){
    polyAngles* angles=new polyAngles[polygons.size()];
    for(int i=0;i<polygons.size();i++){
        *(angles+i)=polygons[i]->getAngles();
    }
    return angles;
}
void Render::updatePolygon(){
    for(auto poly=polygons.begin();poly<polygons.end();poly++){
        (*poly)->updateAngles(cumPos);
    }
}
void Render::draw(angle* cumAngleChange,COORD_3_POINT _cumPos){
    cumPos=_cumPos;
    draw(cumAngleChange);
}
void Render::draw(angle* cumAngleChange,COORD_3_POINT* _cumPos){
    if(_cumPos)
        cumPos+=*_cumPos;
    cumPos.translateToUnsigned();

    if(_cumPos)
        *_cumPos=cumPos;
    
    updatePolygon();
    *cumAngleChange=polyProc.updateCumAngle(*cumAngleChange);// меняем угол камеры
    polyProcessing::mesh** mesh=polyProc.getTriangles(getPolyAngles(),polygons.size());// получаем проекцию полигона
    HBITMAP testPng=(HBITMAP)LoadImage(NULL,L"D:/c++/paint/3d engine/test.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HDC hDcMain=GetDC(pixel->getWnd());
    HRGN hReg;
    POINT* meshPoints=nullptr;
    if(*mesh)
        meshPoints=(*mesh)->getAllPoint();
    if(meshPoints){
        HDC hDcMeshDraw=CreateCompatibleDC(hDcMain);
        hReg=CreatePolygonRgn(meshPoints,3,ALTERNATE);
        RECT wndRect=pixel->getWindowRect();
        HBITMAP tempHbm=CreateCompatibleBitmap(hDcMain,WINDOW,WINDOW);
        SelectObject(hDcMeshDraw,tempHbm);
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(WHITE_BRUSH));
        int debug=SelectClipRgn(hDcMeshDraw,hReg);
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(BLACK_BRUSH));
        BitBlt(hDcMain,0,0,WINDOW,WINDOW,hDcMeshDraw,0,0,SRCCOPY);
        DeleteDC(hDcMeshDraw);
    }

    // HDC hDcTrianglepolyProcessing=GetDC(pixel->getWnd());
    // for(int i=0;i<2;i++){
    //     if(auto call=*(triangles+i);call){
    //         if(POINT* point=call->getPoint();point!=nullptr){
    //             MoveToEx(hDcTrianglepolyProcessing,point->x,point->y,nullptr);
    //         }
    //         for(POINT* point=call->getPoint();point!=nullptr;point=call->getPoint()){
    //             LineTo(hDcTrianglepolyProcessing,point->x,point->y);
    //         }
    //     }
    // }

    {
        MoveToEx(hDcMain,0,0,nullptr);// граница экрана
        LineTo(hDcMain,WINDOW,0);
        LineTo(hDcMain,WINDOW,WINDOW);
        LineTo(hDcMain,0,WINDOW);
        LineTo(hDcMain,0,0);
    }
    UpdateWindow(pixel->getWnd());
    ReleaseDC(pixel->getWnd(),hDcMain);
    DeleteObject(hReg);

}
void Render::setPoly(POLYGON poly){
    polygons.push_back(poly);
}
void Render::cumMove(COORD_3_POINT move){
    cumPos=move;
}
void Render::cumMove(COORD_3_POINT* move){
    cumPos+=*move;
    *move=cumPos;
}
Render::Render(Pixel* pixel){
    this->pixel=pixel;
}