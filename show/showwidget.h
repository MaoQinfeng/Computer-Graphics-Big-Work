#include<QWidget>
#include<QPainter>
#include<QtWidgets>
#include<QMouseEvent>
#include<iostream>
#include<QIODevice>
#include<QtGui>
#include<QTextStream>
#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H
class ShowWidget : public QWidget
{
    Q_OBJECT

public:
    ShowWidget(QWidget *parent = 0);
    ~ShowWidget();
    QImage image =QImage(2000,1000,QImage::Format_RGB888);
    double A[4][4];
    void reset();
    void draw();
    void mul(int n);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseRelaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    QVector4D  point[100][4];
    QVector4D  pointA[100][4];
    QVector3D  point3d[100][4];
    QPointF  point2d[100][4];
    int count;
    QColor col[100];
    QPoint start_point;
    float sum_x;
    float sum_y;
    QVector3D depth;
};

#endif
