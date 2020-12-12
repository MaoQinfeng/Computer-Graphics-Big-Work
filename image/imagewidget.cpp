#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
{
    QFile in("D:\\Code\\image\\feng.txt");
    if ( !in.open( QIODevice::WriteOnly ) ){
        std::cout<<"Cannot do like this";
    }
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::mousePressEvent(QMouseEvent *event){
     sp = event->pos();
}
void ImageWidget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    //if(!start.isEmpty())painter.drawLine(sp,start.back());
    for(int i=0;i<Sstart.size()-1;i++){
        if((i+1)%5!=0)
            painter.drawLine(Sstart[i],Sstart[i+1]);
    }
}
void ImageWidget::mouseReleaseEvent(QMouseEvent *event){


   if(event->button()==Qt::RightButton) {
         start.append(start[0]);
         Sstart.append(start[0]);
         QFile in("D:\\Code\\image\\feng.txt");
         if ( in.open( QIODevice::WriteOnly |QIODevice::Append	) ) {
               QTextStream stream( &in );
               for(int i=0;i<start.size()-1;i++){
                    stream<<start[i].x()<<" "<<start[i].y()<<" -50 ";
               }
               stream<<"\n";
               for(int i=0;i<start.size()-1;i++){
                   stream<<start[i].x()<<" "<<start[i].y()<<" 0 ";
                   stream<<start[i].x()<<" "<<start[i].y()<<" -50 ";
                   stream<<start[i+1].x()<<" "<<start[i+1].y()<<" -50 ";
                   stream<<start[i+1].x()<<" "<<start[i+1].y()<<" 0\n";
               }


               for(int i=0;i<start.size()-1;i++){
                    stream<<start[i].x()<<" "<<start[i].y()<<" 0 ";
               }
               std::cout<<"OK"<<std::endl;

               in.close();
           }
         start.clear();

    }
    else  if(event->button()!=Qt::RightButton){
       start.append(sp);
       Sstart.append(sp);
   }

    update();
}
