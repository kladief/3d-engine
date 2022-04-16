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
    polyProcessing::meshVector* meshes=polyProc.getTriangles(getPolyAngles(),polygons.size());// получаем проекцию полигона
    meshes->setPolyPointers(polygons);
    // HBITMAP testPng=(HBITMAP)LoadImage(NULL,L"D:/c++/paint/3d engine/test.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HDC hDcMain=GetDC(pixel->getWnd());
    HRGN hReg;
    POINT* meshPoints=(*meshes)[0]->getAllPoint();
    if(meshPoints){
        HDC hDcMeshDraw=CreateCompatibleDC(hDcMain);
        hReg=CreatePolygonRgn(meshPoints,3,ALTERNATE);
        RECT wndRect=pixel->getWindowRect();
        HBITMAP tempHbm=CreateCompatibleBitmap(hDcMain,WINDOW,WINDOW);
        SelectObject(hDcMeshDraw,tempHbm);
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(WHITE_BRUSH));
        int debug=SelectClipRgn(hDcMeshDraw,hReg);
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(BLACK_BRUSH));
        HDC imageHdc=CreateCompatibleDC(hDcMeshDraw);
        SelectObject(imageHdc,(*meshes)[0]->polygon->getTexture());
        RECT meshBox=(*meshes)[0]->getMeshBox();;
        BITMAP imageBM;
        ZeroMemory(&imageBM,sizeof(BITMAP));
        GetObject((*meshes)[0]->polygon->getTexture(),sizeof(BITMAP),&imageBM);
        StretchBlt(hDcMeshDraw,meshBox.left,meshBox.top,meshBox.right-meshBox.left,meshBox.bottom-meshBox.top,imageHdc,0,0,imageBM.bmHeight,imageBM.bmWidth,SRCCOPY);
        BitBlt(hDcMain,0,0,WINDOW,WINDOW,hDcMeshDraw,0,0,SRCCOPY);
        DeleteDC(hDcMeshDraw);
        DeleteObject(tempHbm);
    }
    else{
        std::cout<<"Error!"<<std::endl;
    }

    {
        MoveToEx(hDcMain,0,0,nullptr);// граница экрана
        LineTo(hDcMain,WINDOW,0);
        LineTo(hDcMain,WINDOW,WINDOW);
        LineTo(hDcMain,0,WINDOW);
        LineTo(hDcMain,0,0);
    }
    delete meshes;
    UpdateWindow(pixel->getWnd());
    ReleaseDC(pixel->getWnd(),hDcMain);
    DeleteObject(hReg);
    // DeleteObject(testPng);
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