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
    bool toReturn=false;
    std::cout<<"ray angle_1="<<ray->_1<<std::endl; 
    std::cout<<"ray angle_2="<<ray->_2<<std::endl; 
    if(ray->_1<=180+ANGLE && ray->_1>=180-ANGLE){
        std::cout<<"TRUE"<<std::endl;
        if(ray->_2<=180+ANGLE && ray->_2>=180-ANGLE){
            std::cout<<"TRUE_2"<<std::endl;
            toReturn=true;
        }
    }
    return toReturn;
}
angle Render::updateCumAngle(angle newAngle){// изменяем угол наклона камеры
    cumRay+=newAngle;
    leadAngle(&cumRay);
    angle toReturn={cumRay._1-180,cumRay._2-180};
    leadAngle(&toReturn);
    return toReturn;
}
void Render::leadAngle(angle* ang){// приводим угол к диапозону от 0 до 360
    if(ang->_1<0)
        ang->_1=360+ang->_1;
    if(ang->_2<0)
        ang->_2=360+ang->_2;
    if(ang->_1>360)
        ang->_1=ang->_1-360;
    if(ang->_2>360)
        ang->_2=ang->_2-360;    
}
Render::drawCall** Render::viewTriangle(angleTriangle** rays,int polygonNum){
    drawCall** triangles=new drawCall*[polygonNum];
    for(int i=0;i<polygonNum;i++){
        *(triangles+i)=viewTriangle(**(rays+i));
    }

    return triangles;
}
Render::drawCall::~drawCall(){
    delete[] _points;
}
POINT* Render::drawCall::getPoint(){
    _pointsNum--;
    if(_pointsNum<0){
        this->~drawCall();
        return nullptr;
    }
    return (_points+_pointsNum);
}
POINT** Render::getNeighborVertex(POINT* vertex,POINT* mainVertex){
    if(mainVertex-vertex==0){
        POINT** toReturn=new POINT*[2];
        toReturn[0]=(vertex+1);
        toReturn[1]=(vertex+2);
        return toReturn;
    }
    if(mainVertex-vertex==1){
        POINT** toReturn=new POINT*[2];
        toReturn[0]=(vertex);
        toReturn[1]=(vertex+2);
        return toReturn;
    }
    if(mainVertex-vertex==2){
        POINT** toReturn=new POINT*[2];
        toReturn[0]=(vertex);
        toReturn[1]=(vertex+1);
        return toReturn;
    }
    return nullptr;
}
int Render::polygonCut(POINT* vertex){
    std::vector<POINT> cutPolygon;
    for(auto point=vertex;point-vertex<3;point++){// приводим углы к диапозону от 0 до 90, чтобы потом обрезать заходящие за края экрана части(те части угол которых <0 или >90)
        point->x=point->x-(180-ANGLE);
        point->y=point->y-(180-ANGLE);
    }
    int vertexNum=4;
    for(auto point=vertex;point-vertex<3;point++){
        cutPolygon.push_back(*point);
        if(point->x<0){
            //   (y0-y)*|x0 - x_0|
            //y1=-----------
            //      |x-x0|

            // *(cutPolygon.end())={0,( ( ((cutPolygon.end())->y/*y0*/ - getNeighborVertex(vertex,point)[0]->y)/*y*/ * abs((cutPolygon.end())->x-0)/*|x0|*/ ) /
            // abs(getNeighborVertex(vertex,point)[0]->x/*x*/ - (cutPolygon.end())->x/*x0*/ ))};
            // cutPolygon.push_back({0,( ( ((cutPolygon.end())->y/*y0*/ - getNeighborVertex(vertex,point)[0]->y)/*y*/ * abs((cutPolygon.end())->x)/*|x0|*/ ) /
            // abs(getNeighborVertex(vertex,point)[0]->x/*x*/ - (cutPolygon.end())->x/*x0*/ ))});
            // vertexNum++;
        }

    }
    // if(vertexNum!=4){
    //     delete[] vertex;
    //     vertex=new POINT[(vertexNum)];
    //     for(auto i=vertex;i-vertex<vertexNum;i++){
    //         *i=cutPolygon[i-vertex];
    //     }
    //     vertex[(vertexNum-1)]=cutPolygon[0];
    // }
    return vertexNum;
}
Render::drawCall* Render::viewTriangle(angleTriangle ray){
    angleTriangle leadRay=ray-cumRay;
    leadAngle(&(leadRay._1));
    leadAngle(&(leadRay._2));
    leadAngle(&(leadRay._3));
    bool inV_1=inView(&(leadRay._1));
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
        vertexNum=polygonCut(coordTriangle);

        (*coordTriangle).x=(double)(*coordTriangle).x/(double)(ANGLE*2)*WINDOW;
        (*coordTriangle).y=WINDOW-(double)(*coordTriangle).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+1)).x=(double)(*(coordTriangle+1)).x/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+1)).y=WINDOW-(double)(*(coordTriangle+1)).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+2)).x=(double)(*(coordTriangle+2)).x/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+2)).y=WINDOW-(double)(*(coordTriangle+2)).y/(double)(ANGLE*2)*WINDOW;
        (*(coordTriangle+3)).x=(*coordTriangle).x;
        (*(coordTriangle+3)).y=(*coordTriangle).y;

        drawCall* call=new drawCall(coordTriangle,vertexNum);
        return call;
    }
    return nullptr;
}