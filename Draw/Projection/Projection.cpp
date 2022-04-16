#include "Projection.h"
#include <iostream>
angle getAngle(double cos_1,double sin_1,double cos_2,double sin_2){// получаем угол из sin и cos
    std::function<int(double,double)> getangle=[](double cos_1,double sin_1)->int{
        int ang=acos(cos_1)*180/M_PI;
        if(sin_1<0 && cos_1<0){
            ang=180+abs(asin(sin_1)*180/M_PI);
        }
        else if(cos_1>0 && sin_1<0){
            ang=270+asin(cos_1)*180/M_PI;
        }
        return ang;
    };
    angle ang;
    ang._1=getangle(cos_1,sin_1);
    ang._2=getangle(cos_2,sin_2);
    return ang;
}
void poly::updateAngles(COORD_3_POINT cum){// высчитываем углы относительно позиции камеры до вершин полигона
    COORD_3_POINT ray1=p1-cum;
    COORD_3_POINT ray2=p2-cum;
    COORD_3_POINT ray3=p3-cum;
    double tangent1=sqrt(pow(ray1.x,2)+pow(ray1.y,2));// размер луча x
    double tangent2=sqrt(pow(ray2.x,2)+pow(ray2.y,2));
    double tangent3=sqrt(pow(ray3.x,2)+pow(ray3.y,2));
    double tangent1_z=sqrt(pow(ray1.x,2)+pow(ray1.y,2)+pow(ray1.z,2));// размер луча z
    double tangent2_z=sqrt(pow(ray2.x,2)+pow(ray2.y,2)+pow(ray2.z,2));
    double tangent3_z=sqrt(pow(ray3.x,2)+pow(ray3.y,2)+pow(ray3.z,2));
    angles._1= getAngle((double)ray1.x/tangent1,(double)ray1.y/tangent1,tangent1/tangent1_z,(double)ray1.z/tangent1_z);// получаем угол между точкой камеры и вершинами полигона
    angles._2= getAngle((double)ray2.x/tangent2,(double)ray2.y/tangent2,tangent2/tangent2_z,(double)ray2.z/tangent2_z);// см рис.1
    angles._3= getAngle((double)ray3.x/tangent3,(double)ray3.y/tangent3,tangent3/tangent3_z,(double)ray3.z/tangent3_z);
    distance_to_poly=(tangent1_z+tangent2_z+tangent3_z)/(double)3;
}
polyAngles poly::getAngles(){
    return angles;
}
bool polyProcessing::inView(angle* ray){
    bool toReturn=false;
    if(ray->_1<=180+ANGLE && ray->_1>=180-ANGLE){// проверка на видимость вершины
        if(ray->_2<=180+ANGLE && ray->_2>=180-ANGLE){
            toReturn=true;
        }
    }
    ray->_1=ray->_1-(180-ANGLE);// приводим углы к диапозону от 0 до 90, чтобы потом обрезать заходящие за края экрана части(те части угол которых <0 или >90)
    ray->_2=ray->_2-(180-ANGLE);
    return toReturn;
}
angle polyProcessing::updateCumAngle(angle newAngle){// изменяем угол наклона камеры
    cumRay+=newAngle;
    leadAngle(&cumRay);
    angle toReturn={(cumRay._1-180),(cumRay._2-180)};
    leadAngle(&toReturn);
    return toReturn;
}
void polyProcessing::leadAngle(angle* ang){// приводим угол к диапозону от 0 до 360
    if(ang->_1<0)
        ang->_1=360+ang->_1;
    if(ang->_2<0)
        ang->_2=360+ang->_2;
    if(ang->_1>360)
        ang->_1=ang->_1-360;
    if(ang->_2>360)
        ang->_2=ang->_2-360;    
}
polyProcessing::mesh** polyProcessing::getTriangles(polyAngles* rays,int polygonNum){
    mesh** triangles=new mesh*[polygonNum];//делаем проекции для массива полигонов
    for(int i=0;i<polygonNum;i++){
        *(triangles+i)=getTriangles(*(rays+i));
    }
    delete[] rays;
    return triangles;
}
polyProcessing::mesh::~mesh(){
    delete[] _points;
}
POINT* polyProcessing::mesh::getAllPoint(){
    _pointsNum=0;
    return _points;
}
POINT* polyProcessing::mesh::getPoint(){// возвращаем вершину полигона, с каждым вызовом будет возвращена следующаяя вершина и каждый вызов щетчик вершин уменьшается
    _pointsNum--;
    if(_pointsNum<0){//когда вершины закончатся вызываем деструктор
        this->~mesh();
        return nullptr;
    }
    return (_points+_pointsNum);//возвращаем вершины с конца
}

polyProcessing::mesh* polyProcessing::getTriangles(polyAngles ray){
    polyAngles leadRay=ray-cumRay;// вычитаем из углов лучей вершин полигона углы луча камеры  
    leadAngle(&(leadRay._1));//нормируем значения, т.е. приводим в диапозону от 0 до 360
    leadAngle(&(leadRay._2));
    leadAngle(&(leadRay._3));
    bool inV_1=inView(&(leadRay._1));// здесь мы проеряем есть ли хоть одна вершина в поле обзора
    bool inV_2=inView(&(leadRay._2));
    bool inV_3=inView(&(leadRay._3));
    if(inV_1 || inV_2 || inV_3){
        int vertexNum=4;

        POINT* coordTriangle=new POINT[vertexNum];
        (*coordTriangle).x=leadRay._1._1;
        (*coordTriangle).y=leadRay._1._2;
        (*(coordTriangle+1)).x=leadRay._2._1;
        (*(coordTriangle+1)).y=leadRay._2._2;
        (*(coordTriangle+2)).x=leadRay._3._1;
        (*(coordTriangle+2)).y=leadRay._3._2;

        (*coordTriangle).x=(double)(*coordTriangle).x/(double)(ANGLE*2)*WINDOW;// приводим углы лучей вершин треугольника к диапозону разрешения экрана 
        (*coordTriangle).y=WINDOW-(double)(*coordTriangle).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+1)).x=(double)(*(coordTriangle+1)).x/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+1)).y=WINDOW-(double)(*(coordTriangle+1)).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+2)).x=(double)(*(coordTriangle+2)).x/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+2)).y=WINDOW-(double)(*(coordTriangle+2)).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+3)).x=(*coordTriangle).x;
        (*(coordTriangle+3)).y=(*coordTriangle).y;
        mesh* projection=new mesh(coordTriangle,vertexNum);
        return projection;
    }
    return nullptr;
}