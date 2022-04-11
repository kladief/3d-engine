#include "pixelEngine/Pixel.h"
#include "Draw/Draw.h"
#include "CoordStructures.h"
#include <cmath>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <Windowsx.h>
#define WINDOWSIZE 50
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
    COORD_3_POINT p2= { 40 , 40 , 10};
    COORD_3_POINT p3= { 40 , 50, 0};
    triangle t1 = {p1,p2,p3};// создаем полигон
    t1.updateAngles({0,0,0});
    COORD_3_POINT cumPos={0,0,0};//изменение позиции камеры
    COORD keyboard={0,0};
    angle totalCumPos={0,0};// абсолютная позиция камеры
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
                        angleChange._1=-10;
                        break;
                    case 's':
                        angleChange._2=-10;
                        break;
                    case 'd':
                        angleChange._1=10;
                        break;
                    case 'w':
                        angleChange._2=10;
                        break;
                }
                break;
            case WM_KEYDOWN:
                switch (pixel.getMsg().wParam){//перемещение камеры
                    case VK_LEFT:
                        keyboard.X=-1;
                        break;
                    case VK_RIGHT:
                        keyboard.X=1;
                        break;
                    case VK_UP:
                        keyboard.Y=1;
                        break;
                    case VK_DOWN:
                        keyboard.Y=-1;
                        break;

                }
                break;
            case WM_KEYUP:
                switch (pixel.getMsg().wParam){
                    case VK_LEFT:
                        keyboard.X=0;
                        break;
                    case VK_RIGHT:
                        keyboard.X=0;
                        break;
                    case VK_UP:
                        keyboard.Y=0;
                        break;
                    case VK_DOWN:
                        keyboard.Y=0;
                        break;

                }
                break;
        }
        if(keyboard.X==-1){
            cumPos.y-=round(sin((totalCumPos._1+90)*M_PI/180));
            cumPos.x-=round(cos((totalCumPos._1+90)*M_PI/180));
        }
        if(keyboard.X==1){
            cumPos.y+=round(sin((totalCumPos._1+90)*M_PI/180));
            cumPos.x+=round(cos((totalCumPos._1+90)*M_PI/180));
        }
        if(keyboard.Y==1){
            cumPos.y+=round(sin(totalCumPos._1*M_PI/180));
            cumPos.x+=round(cos(totalCumPos._1*M_PI/180));
        }
        if(keyboard.Y==-1){
            cumPos.y-=round(sin(totalCumPos._1*M_PI/180));
            cumPos.x-=round(cos(totalCumPos._1*M_PI/180));
        }
        cumPos.translateToUnsigned();
        t1.updateAngles({cumPos.x,cumPos.y,0});// обновляем координаты полигона
        totalCumPos=render.updateCumAngle(angleChange);// меняем угол камеры
        {
            HDC hDc=GetDC(pixel.getWnd());// миникарта
            MoveToEx(hDc,300+cumPos.x,300+cumPos.y,NULL);
            LineTo(hDc,300+cumPos.x+COS(totalCumPos._1)*40,300+cumPos.y+SIN(totalCumPos._1)*40);
            Ellipse(hDc,300+p1.x,300+p1.y,300+p1.x+10,300+p1.y+10);

            ReleaseDC(pixel.getWnd(),hDc);
        }
        printf("cx= %i \n",cumPos.x);// отадка
        printf("cy= %i \n-------------------------\n",cumPos.y);
        printf("ax= %i \n",totalCumPos._1);
        printf("ay= %i \n\n",totalCumPos._2);
        Sleep(10);
        if(!pixel.process()){
            return 0;
        }
        HDC hDc=GetDC(pixel.getWnd());
        RECT temp=pixel.getWindowRect();
        FillRect(hDc,&temp,(HBRUSH)GetStockObject(WHITE_BRUSH));// очистка
        ReleaseDC(pixel.getWnd(),hDc);
    }
    system("pause");
    return 0;
}