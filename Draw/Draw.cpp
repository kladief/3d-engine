#include "Draw.h"
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
void triangle::updateAngles(COORD_3_POINT cum){// высчитываем углы относительно позиции камеры до вершин полигона
    COORD_3_POINT ray1=p1-cum;
    COORD_3_POINT ray2=p2-cum;
    COORD_3_POINT ray3=p3-cum;
    double tangent1=sqrt(pow(ray1.x,2)+pow(ray1.y,2));// размер луча x
    double tangent2=sqrt(pow(ray2.x,2)+pow(ray2.y,2));
    double tangent3=sqrt(pow(ray3.x,2)+pow(ray3.y,2));
    double tangent1_z=sqrt(pow(ray1.x,2)+pow(ray1.y,2)+pow(ray1.z,2));// размер луча z
    double tangent2_z=sqrt(pow(ray2.x,2)+pow(ray2.y,2)+pow(ray2.z,2));
    double tangent3_z=sqrt(pow(ray3.x,2)+pow(ray3.y,2)+pow(ray3.z,2));
    angles._1= getAngle((double)ray1.x/tangent1,(double)ray1.y/tangent1,tangent1/tangent1_z,(double)ray1.z/tangent1_z);
    std::cout<<"angle to fist vertex="<<angles._1._1<<std::endl;
    angles._2= getAngle((double)ray2.x/tangent2,(double)ray2.y/tangent2,tangent2/tangent2_z,(double)ray2.z/tangent2_z);
    angles._3= getAngle((double)ray3.x/tangent3,(double)ray3.y/tangent3,tangent3/tangent3_z,(double)ray3.z/tangent3_z);
}
angleTriangle triangle::getAngles(){
    return angles;
}
bool Render::inView(angle* ray){
    if(ray->_1<=ANGLE || ray->_1>=360-ANGLE){
        if(ray->_2<=ANGLE || ray->_2>=360-ANGLE){
            if(ray->_1>ANGLE){
                ray->_1=ray->_1-(360-ANGLE);
            }
            else{
                ray->_1+=ANGLE;
            }
            if(ray->_2>ANGLE){
                ray->_2=ray->_2-(360-ANGLE);
            }
            else{
                ray->_2+=ANGLE;
            }
            return true;
        }
    }
    return false;
}
angle Render::updateCumAngle(angle newAngle){// изменяем угол наклона камеры
    cumRay+=newAngle;
    leadAngle(&cumRay);
    return cumRay;
}
void Render::leadAngle(angle* ang){
    if(ang->_1<0)
        ang->_1=360+ang->_1;
    if(ang->_2<0)
        ang->_2=360+ang->_2;
    if(ang->_1>360)
        ang->_1=ang->_1-360;
    if(ang->_2>360)
        ang->_2=ang->_2-360;    
}
COORD_TRIANGLE** Render::viewTriangle(angleTriangle** rays,int polygonNum){
    COORD_TRIANGLE** triangles=new COORD_TRIANGLE*[polygonNum];
    for(int i=0;i<polygonNum;i++){
        *(triangles+i)=viewTriangle(**(rays+i));
    }
    return triangles;
}
COORD_TRIANGLE* Render::viewTriangle(angleTriangle ray){
    angleTriangle leadRay=ray-cumRay;
    leadAngle(&(leadRay._1));
    leadAngle(&(leadRay._2));
    leadAngle(&(leadRay._3));
    if(inView(&(leadRay._1)) && inView(&(leadRay._2)) && inView(&(leadRay._3))){
        COORD_TRIANGLE* coordTriangle=new COORD_TRIANGLE;

        coordTriangle->_1.x=leadRay._1._1;
        coordTriangle->_1.y=leadRay._1._2;

        coordTriangle->_2.x=leadRay._2._1;
        coordTriangle->_2.y=leadRay._2._2;

        coordTriangle->_3.x=leadRay._3._1;
        coordTriangle->_3.y=leadRay._3._2;

        coordTriangle->_1.x=(double)coordTriangle->_1.x/(double)(2*ANGLE)*WINDOW;
        coordTriangle->_1.y=WINDOW-(double)coordTriangle->_1.y/(double)(2*ANGLE)*WINDOW;
        coordTriangle->_2.x=(double)coordTriangle->_2.x/(double)(2*ANGLE)*WINDOW;
        coordTriangle->_2.y=WINDOW-(double)coordTriangle->_2.y/(double)(2*ANGLE)*WINDOW;
        coordTriangle->_3.x=(double)coordTriangle->_3.x/(double)(2*ANGLE)*WINDOW;
        coordTriangle->_3.y=WINDOW-(double)coordTriangle->_3.y/(double)(2*ANGLE)*WINDOW;
        return coordTriangle;
    }
    return nullptr;
}