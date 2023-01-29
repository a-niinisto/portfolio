#ifndef QLABELR_H
#define QLABELR_H

#include <QLabel>

class QLabelR : public QLabel
{
public:
    QLabelR(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setInclination(int degree);

    void setPermPic(const QPixmap& image);

private:
    QPixmap permPic_;
};

#endif // QLABELR_H
