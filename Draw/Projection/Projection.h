#ifndef DRAW_H
#define DRAW_H
#define _USE_MATH_DEFINES
#define ANGLE 45
#define WINDOW 400
#define COS(a) cos((float)a/(float)180*(float)M_PI)
#define SIN(a) sin((float)a/(float)180*(float)M_PI)
#define TAN(a) tan((float)a/(float)180*(float)M_PI)
#define  STANDART_TX L"standart_texture.bmp"
#include <cmath>
#include <functional>
#include <windows.h>
#include <vector>
#include "D:\c++\paint\3d engine\CoordStructures.h"

struct angle{
    int _1;
    int _2;
    void operator+=(angle in){
        this->_1+=in._1;
        this->_2+=in._2;
    }
    angle operator-(angle in){
        angle toReturn;
        toReturn._1=this->_1-in._1;
        toReturn._2=this->_2-in._2;
        return toReturn;
    }
};

struct polyAngles{
    angle _1;
    angle _2;
    angle _3;
    polyAngles operator-(polyAngles in){
        polyAngles toReturn;
        toReturn._1=this->_1-in._1;
        toReturn._2=this->_2-in._2;
        toReturn._3=this->_3-in._3;
        return toReturn;
    }
    polyAngles operator-(angle in){
        polyAngles toReturn;
        toReturn._1=this->_1-in;
        toReturn._2=this->_2-in;
        toReturn._3=this->_3-in;
        return toReturn;
    }
};
angle getAngle(double ,double ,double );

struct poly{
private:
    polyAngles angles;
    double distance_to_poly;
    HBITMAP* tx=nullptr;
public:
    COORD_3_POINT p1;
    COORD_3_POINT p2;
    COORD_3_POINT p3;
    poly();
    poly(COORD_3_POINT a,COORD_3_POINT b,COORD_3_POINT c){
        p1 = a;
        p2 = b;
        p3 = c;
    };
    void loadTexture(wchar_t*);
    HBITMAP getTexture();
    void updateAngles(COORD_3_POINT); 
    polyAngles getAngles();
    ~poly();
};
class polyProcessing{
private:
    angle cumRay={180,180};
    bool inView(angle*);
    void leadAngle(angle*);
    struct mesh{
    private:
        int pointsNum=0;
        int pointsNumConst=0;
        POINT* pointsEntry;
        POINT* points;
        HBITMAP texture;
    public:
        poly* polygon;
        mesh(POINT* _points,int _pointsNum):points(_points),pointsEntry(_points),pointsNum(_pointsNum),pointsNumConst(_pointsNum){}
        POINT* getPoint();
        POINT* getAllPoint();
        ~mesh();
        RECT getMeshBox();
    }; 
public:
    struct meshVector{
        private:
            int mashNum;
            mesh** meshes;
        public:
            void setPolyPointers(std::vector<poly*>);
            mesh* operator[](int); 
            ~meshVector();
            meshVector(mesh** _meshes,int _meshNum):meshes(_meshes),mashNum(_meshNum){}
    };
    angle updateCumAngle(angle);
    meshVector* getTriangles(polyAngles*,int);
    mesh* getTriangles(polyAngles);
};
#endif