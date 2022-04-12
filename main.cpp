#include "pixelEngine/Pixel.h"
#include "Draw/Draw.h"
#include "CoordStructures.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <Windowsx.h>
#define WINDOWSIZE 50
#define C_TOP 1
#define C_BOTTOM 2
#define C_LEFT 3
#define C_RIGHT 4
#define C_UP 5
#define C_DOWN 6
BOOL CreateConsole(void)
{
    FreeConsole(); //на всякий случай
    if ( AllocConsole() )
    {
        int hCrt = _open_osfhandle((intptr_t)
        GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        *stdout = *(::_fdopen(hCrt, "w"));
        ::setvbuf(stdout, NULL, _IONBF, 0);
        *stderr = *(::_fdopen(hCrt, "w"));
        ::setvbuf(stderr, NULL, _IONBF, 0);
        return TRUE;
    }
    return FALSE;
}

int CALLBACK wWinMain(HINSTANCE hInst,HINSTANCE ,PWSTR szCmdLine, int nCmdShow){
    using namespace std;
    CreateConsole();

    Pixel pixel;
    pixel.beginPaint(hInst);

    Render render;
    COORD_3_POINT p1= { 40 , 40 , 0};
    COORD_3_POINT p3= { 40 , 40 , 10};
    COORD_3_POINT p2= { 40 , 50, 0};
    triangle t1 = {p1,p2,p3};// создаем полигон
    t1.updateAngles({0,0,0});
    COORD_3_POINT cumPos={0,0,0};//изменение позиции камеры
    angle cumAngle={0,0};// угол поворота камеры
    WORD keyboard;
    while(true){
        angle angleChange={0,0};
        COORD_TRIANGLE * triang=render.viewTriangle(t1.getAngles());// получаем проекцию полигона
        HDC hDcTriangleRender=GetDC(pixel.getWnd());
        if(triang){ // трисовываем полигон
            MoveToEx(hDcTriangleRender,triang->_1.x,triang->_1.y,nullptr);
            LineTo(hDcTriangleRender,triang->_2.x,triang->_2.y);
            LineTo(hDcTriangleRender,triang->_3.x,triang->_3.y);
            LineTo(hDcTriangleRender,triang->_1.x,triang->_1.y);
        }
        MoveToEx(hDcTriangleRender,0,0,nullptr);// граница экрана
        LineTo(hDcTriangleRender,WINDOW,0);
        LineTo(hDcTriangleRender,WINDOW,WINDOW);
        LineTo(hDcTriangleRender,0,WINDOW);
        LineTo(hDcTriangleRender,0,0);
        ReleaseDC(pixel.getWnd(),hDcTriangleRender);
        UpdateWindow(pixel.getWnd());
        delete triang;// удаляем проекцию полигона
        switch(pixel.getMsg().message){ // координаты камеры
            case WM_CHAR:
                switch(pixel.getMsg().wParam){//поворот камеры
                    case 'a':
                        angleChange._1=-5;
                        break;
                    case 's':
                        angleChange._2=-5;
                        break;
                    case 'd':
                        angleChange._1=5;
                        break;
                    case 'w':
                        angleChange._2=5;
                        break;
                }
                break;
            case WM_KEYDOWN:
                switch (pixel.getMsg().wParam){//перемещение камеры
                    case VK_DOWN:
                        keyboard=C_BOTTOM;
                        break;
                    case VK_LEFT:
                        keyboard=C_LEFT;
                        break;
                    case VK_RIGHT:
                        keyboard=C_RIGHT;
                        break;
                    case VK_UP:
                        keyboard=C_TOP;
                        break;
                    case VK_SPACE:
                        keyboard=C_UP;
                        break;
                    case VK_TAB:
                        keyboard=C_DOWN;
                        break;

                }
                break;
            case WM_KEYUP:
                switch (pixel.getMsg().wParam){
                    case VK_DOWN:
                        keyboard=NULL;
                        break;
                    case VK_LEFT:
                        keyboard=NULL;
                        break;
                    case VK_RIGHT:
                        keyboard=NULL;
                        break;
                    case VK_UP:
                        keyboard=NULL;
                        break;
                    case VK_SPACE:
                        keyboard=NULL;
                        break;
                    case VK_TAB:
                        keyboard=NULL;
                        break;
                }
                break;
        }
        switch(keyboard){
            case C_TOP:
                cumPos.y+=round(sin(cumAngle._1*M_PI/180));
                cumPos.x+=round(cos(cumAngle._1*M_PI/180));
                break;
            case C_BOTTOM:
                cumPos.y-=round(sin(cumAngle._1*M_PI/180));
                cumPos.x-=round(cos(cumAngle._1*M_PI/180));
                break;
            case C_RIGHT:
                cumPos.y+=round(sin((cumAngle._1+90)*M_PI/180));
                cumPos.x+=round(cos((cumAngle._1+90)*M_PI/180));
                break;
            case C_LEFT:
                cumPos.y-=round(sin((cumAngle._1+90)*M_PI/180));
                cumPos.x-=round(cos((cumAngle._1+90)*M_PI/180));                
                break;
            case C_UP:
                cumPos.z++;
                break;
            case C_DOWN:
                cumPos.z--;
                break;
        } 
        cumPos.translateToUnsigned();
        t1.updateAngles(cumPos);// обновляем координаты полигона
        cumAngle=render.updateCumAngle(angleChange);// меняем угол камеры
        {
            HDC hDc=GetDC(pixel.getWnd()); // миникарта
            MoveToEx(hDc,+cumPos.x,+cumPos.y,NULL);
            LineTo(hDc,+cumPos.x+COS(cumAngle._1)*40,+cumPos.y+SIN(cumAngle._1)*40);
            Ellipse(hDc,+p1.x,+p1.y,+p1.x+10,+p1.y+10);

            ReleaseDC(pixel.getWnd(),hDc);
        }
        std::cout<<"cum pos x="<<cumPos.x<<std::endl;// отадка
        std::cout<<"cum pos y="<<cumPos.y<<std::endl;
        std::cout<<"cum pos z="<<cumPos.z<<std::endl;
        std::cout<<"cum angle x="<<cumAngle._1<<std::endl;
        std::cout<<"cum angle y="<<cumAngle._2<<std::endl;
        std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        Sleep(10);
        if(!pixel.process()){
            return 0;
        }
        HDC hDc=GetDC(pixel.getWnd());
        RECT temp=pixel.getWindowRect();
        FillRect(hDc,&temp,(HBRUSH)GetStockObject(WHITE_BRUSH));// очистка экрана
        ReleaseDC(pixel.getWnd(),hDc);
    }
    system("pause");
    return 0;
}