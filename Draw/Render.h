#ifndef RENDER_H
#define RENDER_H
#define POLYGON poly*
#include "Projection/Projection.h"
#include "pixelEngine/Pixel.h"
#include <iostream>
#include <wingdi.h>
#include <vector>
#include <Windows.h>
#include <winuser.h>

class Render{
private:
    polyProcessing polyProc;
    COORD_3_POINT cumPos={0,0,0};
    std::vector<POLYGON> polygons;
    polyAngles* getPolyAngles();
    void updatePolygon();
    Pixel* pixel;
public:
    Render(Pixel* );
    void setPoly(POLYGON);
    void draw(angle*,COORD_3_POINT);
    void draw(angle*,COORD_3_POINT* _cumPos=nullptr);
    void cumMove(COORD_3_POINT);
    void cumMove(COORD_3_POINT*);
};
#endif