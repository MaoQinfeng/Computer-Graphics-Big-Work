#include "showwidget.h"
#define PROJECTION   1      //是否投影
ShowWidget::ShowWidget(QWidget *parent): QWidget(parent)
{
    reset();
    image.fill(QColor(255,255,255));
    depth=QVector3D(0,0,-1000);
    QFile out("D:\\Code\\image\\feng.txt");
    if(!out.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "Cannot Open The File!" << std::endl;
    }

    else{
        QTextStream Tout(&out);
        while(!Tout.atEnd()){
            count=0;
            for(int i =0;i<150&&(!Tout.atEnd());i++){
                 for(int j =0;j<4&&(!Tout.atEnd());j++){
                    int x,y,z;
                    Tout>>x>>y>>z;
                    if(x==y&&y==z&&z==0){}
                    else{
                        point[i][j].setX(x);
                        point[i][j].setY(y);
                        point[i][j].setZ(z);
                        point[i][j].setW(1);
                        pointA[i][j].setX(x);
                        pointA[i][j].setY(y);
                        pointA[i][j].setZ(z);
                        pointA[i][j].setW(1);
                        count++;
                    }

                }

            }

        }

    }
    count = count/4;
    mul(count);
    out.close();
    //笔刷的颜色，给笔刷的颜色附随机的RGB的值
    for(int i = 0;i<count+1;i++){
        if(i==0){
            col[i] = (QColor(255,255,255));
        }
        else  {
            col[i] = (QColor(rand()%255,rand()%255,rand()%255));
        }
    }

   //开始画
    draw();
    update();

}
void ShowWidget::mul(int n){
    sum_x =0;
    sum_y=0;
    for(int i=0;i<n;i++){
       for(int j=0;j<4;j++){
           point[i][j].setX(point[i][j].x()*A[0][0]+point[i][j].y()*A[0][1]+point[i][j].z()*A[0][2]+point[i][j].w()*A[0][3]);
           point[i][j].setY(point[i][j].x()*A[1][0]+point[i][j].y()*A[1][1]+point[i][j].z()*A[1][2]+point[i][j].w()*A[1][3]);
           point[i][j].setZ(point[i][j].x()*A[2][0]+point[i][j].y()*A[2][1]+point[i][j].z()*A[2][2]+point[i][j].w()*A[2][3]);
       }
    }
    //投影
     for(int i=0;i<n;i++){
        for(int j=0;j<4;j++){
            //储存投影后的坐标点的信息
            if(PROJECTION){
                point3d[i][j].setX((point[i][j].x()*depth.z()-point[i][j].z()*depth.x())/(depth.z()-point[i][j].z()));
                point3d[i][j].setY((point[i][j].y()*depth.z()-point[i][j].z()*depth.y())/(depth.z()-point[i][j].z()));
            }
            else
            {
                point3d[i][j].setX(point[i][j].x());
                point3d[i][j].setY(point[i][j].y());
            }
            point3d[i][j].setZ(point[i][j].z());

            //画多边形面片
            point2d[i][j].setX(point3d[i][j].x());
            point2d[i][j].setY(point3d[i][j].y());
        }
     }
     sum_x = sum_x/(count*4);
     sum_y = sum_y/(count*4);

}
void ShowWidget::wheelEvent(QWheelEvent *event){


    image.fill(QColor(255,255,255));
    float i=0.1;
    if(event->delta()<0)i=-0.1;
    A[0][0]=1+i;
    A[1][1]=1+i;
    A[2][2]=1+i;
    mul(count);
    draw();
    update();

}

void ShowWidget::draw(){
    image.fill(QColor(255,255,255));
    QPainter painter(&image);
     int SIDE[100];
    //求面 SIDE的总值
     for(int i=count-1;i>=0;i--){
        SIDE[i]=point3d[i][0].z()+point3d[i][1].z()+point3d[i][2].z()+point3d[i][3].z();
     }
     int tem=30000;
     int temp;
     for(int i=0;i<count;i++){
         for(int j=0;j<count;j++){
            if(tem>SIDE[j]){
                tem=SIDE[j];
                temp=j;
            }

         }
         SIDE[temp]=30000;
         tem=30000;
         QPolygonF polygon;
         QBrush brush(col[temp+1]);
         polygon << point2d[temp][0]<< point2d[temp][1]<< point2d[temp][2]<< point2d[temp][3];
         brush.setStyle(Qt::SolidPattern);
         painter.setBrush(brush);
         painter.setPen(col[temp+1]);
         painter.drawPolygon(polygon, Qt::WindingFill);

     }
}
ShowWidget::~ShowWidget()
{

}
void ShowWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(0,0,image);

}
void ShowWidget::reset(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            A[i][j]=(i==j);
        }
    }

}
void equal(QVector4D A[][4],QVector4D B[][4],int n){
     for(int i=0;i<n;i++){
         for(int j=0;j<4;j++){
             A[i][j]=B[i][j];
         }
     }
}

void ShowWidget::mousePressEvent(QMouseEvent *event){
    start_point = event->pos();
    equal(pointA,point,count);
}

void ShowWidget::mouseRelaseEvent(QMouseEvent *event){
    if(event->buttons()==Qt::RightButton)
    equal(pointA,point,count);
}
void ShowWidget::mouseMoveEvent(QMouseEvent *event){
    QPoint end_point=event->pos();
    reset();
    if(event->buttons()!=Qt::RightButton){//移动
        A[0][3]=end_point.x()-start_point.x();
        A[1][3]=end_point.y()-start_point.y();
        start_point = end_point;
        mul(count);
        draw();
        update();
    }
    else if(event->buttons()==Qt::RightButton){//旋转
        equal(point,pointA,count);
        end_point=event->pos();//按几何中心，若按原点则 先A[0][3]=200;A[1][3]=200;
        A[0][3]=-sum_x;
        A[1][3]=-sum_y;
        mul(count);
        reset();
        float cos,sin;
        if(event->modifiers()&Qt::ControlModifier){
            cos=((start_point.x()-sum_x)*(end_point.x()-sum_x)+(start_point.y()-sum_y)*(end_point.y()-sum_y))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));
            sin =((start_point.x()-sum_x)*(end_point.y()-sum_y)-(start_point.y()-sum_y)*(end_point.x()-sum_x))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));
            A[0][0]=cos;
            A[0][1]=-sin;
            A[1][0]=sin;
            A[1][1]=cos;
        }
        else if(event->modifiers()&Qt::ShiftModifier){
            cos=((start_point.x()-sum_x)*(end_point.x()-sum_x)+(start_point.y()-sum_y)*(end_point.y()-sum_y))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));
            sin =((start_point.x()-sum_x)*(end_point.y()-sum_y)-(start_point.y()-sum_y)*(end_point.x()-sum_x))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));
            A[0][0]=cos;
            A[0][2]=-sin;
            A[2][0]=sin;
            A[2][2]=cos;
        }
        else{
            cos=((start_point.x()-sum_x)*(end_point.x()-sum_x)+(start_point.y()-sum_y)*(end_point.y()-sum_y))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));
            sin =((start_point.x()-sum_x)*(end_point.y()-sum_y)-(start_point.y()-sum_y)*(end_point.x()-sum_x))/sqrt(((start_point.x()-sum_x)*(start_point.x()-sum_x)+(start_point.y()-sum_y)*(start_point.y()-sum_y))*((end_point.x()-sum_x)*(end_point.x()-sum_x)+(end_point.y()-sum_y)*(end_point.y()-sum_y)));

            A[1][1]=cos;
            A[1][2]=-sin;
            A[2][1]=sin;
            A[2][2]=cos;
        }
        mul(count);
        reset();
        image.fill(QColor(255,255,255));
        A[0][3]=sum_x;
        A[1][3]=sum_y;
        mul(count);
        reset();
        draw();
        mul(count);
        update();

    }
}
