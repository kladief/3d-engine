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
    HDC hDcMain=GetDC(pixel->getWnd());
    HRGN hReg;
    polygons[0]->loadTexture(L"test.bmp");
    POINT* meshPoints=(*meshes)[0]->getAllPoint();
    if(meshPoints){
        HDC hDcMeshDraw=CreateCompatibleDC(hDcMain);// HDC который будет буффером между HDC с полигоном и основным HDC
        hReg=CreatePolygonRgn(meshPoints,3,ALTERNATE);// создаем регион с помощью координат мэша
        RECT wndRect=pixel->getWindowRect();// размер окна программы
        HBITMAP tempHbm=CreateCompatibleBitmap(hDcMain,WINDOW,WINDOW);// HBITMAP для hDcMeshDraw
        SelectObject(hDcMeshDraw,tempHbm);
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(WHITE_BRUSH));// очищаем hDcMeshDraw
        int debug=SelectClipRgn(hDcMeshDraw,hReg);// задаем облать рисования(это область мэша) в hDcMeshDraw
        FillRect(hDcMeshDraw,&wndRect,(HBRUSH)GetStockObject(BLACK_BRUSH)); // закрашиваем область мэша, на всякий случай, если с текстурой что-то не так
        HDC imageHdc=CreateCompatibleDC(hDcMeshDraw);// HDC для текстуры
        SelectObject(imageHdc,(*meshes)[0]->polygon->getTexture());// загружаем текстуру в imageHdc
        RECT meshBox=(*meshes)[0]->getMeshBox();// получаем ограничивающий мэш прямоугольник
        BITMAP imageBM;// данные о текстуре, из них мы получаем её размер
        ZeroMemory(&imageBM,sizeof(BITMAP));
        GetObject((*meshes)[0]->polygon->getTexture(),sizeof(BITMAP),&imageBM);

        StretchBlt(hDcMeshDraw,meshBox.left,meshBox.top,meshBox.right-meshBox.left,meshBox.bottom-meshBox.top,imageHdc,0,0,imageBM.bmHeight,imageBM.bmWidth,SRCCOPY);
        BitBlt(hDcMain,0,0,WINDOW,WINDOW,hDcMeshDraw,0,0,SRCCOPY);// копируем hDcMeshDraw в основной HDC
        DeleteDC(hDcMeshDraw);// очистка памяти
        DeleteDC(imageHdc);
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