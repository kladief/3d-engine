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
    POINT* meshPoints=(*mesh)->getAllPoint();
    std::cout<<meshPoints->x<<"-"<<meshPoints->y<<std::endl;
    std::cout<<(meshPoints+1)->x<<"-"<<(meshPoints+1)->y<<std::endl;
    std::cout<<(meshPoints+2)->x<<"-"<<(meshPoints+2)->y<<std::endl;
    HRGN hReg=CreatePolygonRgn(meshPoints,3,ALTERNATE);
    HDC hDcTrianglepolyProcessing=GetDC(pixel->getWnd());
    // HDC tempHdc=CreateCompatibleDC(hDcTrianglepolyProcessing);
    //HBITMAP hBitmap=CreateCompatibleBitmap(hDcTrianglepolyProcessing,WINDOW,WINDOW);
    //SelectObject(tempHdc,testPng);
    int debug_1=SetWindowRgn(pixel->getWnd(),hReg,TRUE);
    RECT temp=pixel->getWindowRect();
    FillRect(hDcTrianglepolyProcessing,&temp,(HBRUSH)GetStockObject(BLACK_BRUSH));
    //int debug=SelectClipRgn(hDcTrianglepolyProcessing,hReg);
    int debug2=0;

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
        MoveToEx(hDcTrianglepolyProcessing,0,0,nullptr);// граница экрана
        LineTo(hDcTrianglepolyProcessing,WINDOW,0);
        LineTo(hDcTrianglepolyProcessing,WINDOW,WINDOW);
        LineTo(hDcTrianglepolyProcessing,0,WINDOW);
        LineTo(hDcTrianglepolyProcessing,0,0);
    }
    UpdateWindow(pixel->getWnd());
    ReleaseDC(pixel->getWnd(),hDcTrianglepolyProcessing);
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