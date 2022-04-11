#ifndef COORDSTRCTURES_H
#define COORDSTRCTURES_H
struct COORD_3_POINT{ //координаты в 3-x мерном пространстве
    int x;
    int y;
    int z;
    COORD_3_POINT(int X, int Y, int Z){
        x=X;
        y=Y;
        z=Z;
    }
    void operator=(COORD_3_POINT a){
        x=a.x;
        y=a.y;
        z=a.z;
    }
    COORD_3_POINT operator-(COORD_3_POINT in){
        COORD_3_POINT toReturn;
        toReturn.x=this->x-in.x;
        toReturn.y=this->y-in.y;
        toReturn.z=this->z-in.z;
        return toReturn;
    }
    bool translateToUnsigned(){
        bool translate=false;
        if(x<0)x=0;translate=true;
        if(y<0)y=0;translate=true;
        if(z<0)z=0;translate=true;
        return translate;
    }
    COORD_3_POINT(){}
};
struct COORD_TRIANGLE{
    COORD_3_POINT _1;
    COORD_3_POINT _2;
    COORD_3_POINT _3;
};
struct COORD_3_POINT_FLOAT{ //координаты в 3-x мерном пространстве
    float x;
    float y;
    float z;
    COORD_3_POINT_FLOAT(){}
    COORD_3_POINT_FLOAT(float X, float Y, float Z){
        x=X;
        y=Y;
        z=Z;
    }

    COORD_3_POINT_FLOAT operator-(COORD_3_POINT_FLOAT in){
        COORD_3_POINT_FLOAT toReturn=*this;
        toReturn.x-=in.x;
        toReturn.y-=in.y;
        toReturn.z-=in.z;
        return toReturn;
    }
};
struct COORD_3_FLOAT{// 3, 3-x мерных координаты(полигон)
    COORD_3_POINT_FLOAT p;
    COORD_3_POINT_FLOAT p0;
    COORD_3_POINT_FLOAT p1;
    COORD_3_POINT_FLOAT p2;
    COORD_3_FLOAT(COORD_3_POINT_FLOAT P0, COORD_3_POINT_FLOAT P1, COORD_3_POINT_FLOAT P2){
        p0=P0;
        p1=P1;
        p2=P2;
    }
    COORD_3_FLOAT(){}
};

#endif