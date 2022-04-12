#include "Draw.h"
#include <iostream>
angle getAngle(double Xangle,double Yangle,double Zangle){// получаем угол из sin и cos
    angle ang;
    ang._2=asin(Zangle)*180/M_PI;
    ang._1=acos(Xangle)*180/M_PI;
    if(Yangle<0 && Xangle<0){
        ang._1=180+abs(asin(Yangle)*180/M_PI);
    }
    else if(Xangle>0 && Yangle<0){
        ang._1=270+asin(Xangle)*180/M_PI;
    }
    return ang;
}
void triangle::updateAngles(COORD_3_POINT cum){// высчитываем углы относительно позиции камеры до вершин полигона
    COORD_3_POINT ray1=p1-cum;
    COORD_3_POINT ray2=p2-cum;
    COORD_3_POINT ray3=p3-cum;
    double tangent1=sqrt(pow(ray1.x,2)+pow(ray1.y,2));// размер луча
    double tangent2=sqrt(pow(ray2.x,2)+pow(ray2.y,2));
    double tangent3=sqrt(pow(ray3.x,2)+pow(ray3.y,2));
    angles._1= getAngle((double)ray1.x/tangent1,(double)ray1.y/tangent1,(double)ray1.z/tangent1);
    std::cout<<"angle to fist vertex="<<angles._1._1<<std::endl;
    angles._2= getAngle((double)ray2.x/tangent2,(double)ray2.y/tangent2,(double)ray2.z/tangent2);
    angles._3= getAngle((double)ray3.x/tangent3,(double)ray3.y/tangent3,(double)ray3.z/tangent3);
}
angleTriangle triangle::getAngles(){
    return angles;
}
bool Render::inView(angle ray){
    angle leadRay=ray-cumRay;
    leadAngle(&leadRay);
    if(leadRay._1<=45 || leadRay._1>=315){
        if(leadRay._2<=45 || leadRay._2>=315){
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
        ang->_2=360+ang->_1;
    if(ang->_1>360)
        ang->_1=ang->_1-360;
    if(ang->_2>360)
        ang->_2=ang->_1-360;    
}
COORD_TRIANGLE* Render::viewTriangle(angleTriangle ray){
    if(inView(ray._1) && inView(ray._2) && inView(ray._3)){
        COORD_TRIANGLE* coordTriangle=new COORD_TRIANGLE;
        angleTriangle leadRay=ray-cumRay;
        leadAngle(&(leadRay._1));
        leadAngle(&(leadRay._2));
        leadAngle(&(leadRay._3));

        coordTriangle->_1.x=leadRay._1._1;
        coordTriangle->_1.y=leadRay._1._2;

        coordTriangle->_2.x=leadRay._2._1;
        coordTriangle->_2.y=leadRay._2._2;

        coordTriangle->_3.x=leadRay._3._1;
        coordTriangle->_3.y=leadRay._3._2;
        // coordTriangle->_1.x=(cos(coordTriangle->_1.x/(float)180*M_PI)+1)/2*WINDOW;
        // coordTriangle->_1.y=(cos(coordTriangle->_1.y/(float)180*M_PI)+1)/2*WINDOW;
        // coordTriangle->_2.x=(cos(coordTriangle->_2.x/(float)180*M_PI)+1)/2*WINDOW;
        // coordTriangle->_2.y=(cos(coordTriangle->_2.y/(float)180*M_PI)+1)/2*WINDOW;
        // coordTriangle->_3.x=(cos(coordTriangle->_3.x/(float)180*M_PI)+1)/2*WINDOW;
        // coordTriangle->_3.y=(cos(coordTriangle->_3.y/(float)180*M_PI)+1)/2*WINDOW;
        float c1x=TAN(coordTriangle->_1.x);
        float c1y=TAN(coordTriangle->_1.y);
        float c2x=TAN(coordTriangle->_2.x);
        float c2y=TAN(coordTriangle->_2.y);
        float c3x=TAN(coordTriangle->_3.x);
        float c3y=TAN(coordTriangle->_3.y);
        if(c1x>1||
           c1y>1||
           c2x>1||
           c2y>1||
           c3x>1||
           c3y>1){
                std::cout<<coordTriangle->_1.x<<std::endl;
                std::cout<<coordTriangle->_1.y<<std::endl;
                std::cout<<coordTriangle->_2.x<<std::endl;
                std::cout<<coordTriangle->_2.y<<std::endl;
                std::cout<<coordTriangle->_3.x<<std::endl;
                std::cout<<coordTriangle->_3.y<<std::endl;
           }
        c1x++;
        c1y++;
        c2x++;
        c2y++;
        c3x++;
        c3y++;

        coordTriangle->_1.x=c1x/2*WINDOW;
        coordTriangle->_1.y=c1y/2*WINDOW;
        coordTriangle->_2.x=c2x/2*WINDOW;
        coordTriangle->_2.y=c2y/2*WINDOW;
        coordTriangle->_3.x=c3x/2*WINDOW;
        coordTriangle->_3.y=c3y/2*WINDOW;
        return coordTriangle;
    }
    return nullptr;
}