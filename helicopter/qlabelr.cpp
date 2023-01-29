#include "qlabelr.h"
#include <QPainter>

QLabelR::QLabelR(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{

}

void QLabelR::setInclination(int degree)
{
    const QPixmap image = permPic_;
    QPixmap rotate(140,140);
    QPainter p(&rotate);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotate.size().width() / 2, rotate.size().height() / 2);
    p.rotate(degree);
    p.translate(-rotate.size().width() / 2, -rotate.size().height()/2 );

    p.setBackgroundMode(Qt::TransparentMode);
    QRectF target(-24,-26, 200, 200);
    QRectF source(0, 0, 200, 200);

    //p.drawPixmap(-10,40,image);
    p.drawPixmap(target,image, source);
    p.end();
    rotate = rotate.scaled(100, 100);

    setPixmap(rotate);
}

void QLabelR::setPermPic(const QPixmap& image)
{
    permPic_ = image;
}
