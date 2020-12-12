#include <QWidget>
#include <QPainter>
#include<QtWidgets>
#include <QMouseEvent>
#include<iostream>
#include<QIODevice>
#include<QtGui>
#include<QTextStream>
#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(QWidget *parent = 0);
    ~ImageWidget();
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);


    QList <QPoint>start;
    QList <QPoint>Sstart;
    QPoint sp;
};

#endif
