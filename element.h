#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <QRect>

#include <QDebug>

class Element
{
public:
    Element();
    Element(QRect rect);
    Element(QRect rect, QString data);
    Element(int type, double know);

    void set(int type, double know);

    inline QString data() const;
    inline double know() const;
    inline int type() const;
    inline QRect rect() const;
    inline int stat() const;

private:
    QString _data;	//從tesseract裡獲得的資料
    double _know;	//經由我們鑑定後的資料
    int _type;	//know的資料型態
    QRect _rect;    //位置資料
};

inline int Element::stat() const{
    if(!_data.isNull() && _know==-1)
        return 0;
    return -1;
}

inline QString Element::data() const{
    return _data;
}

inline double Element::know() const{
    return _know;
}

inline int Element::type() const{
    return _type;
}

inline QRect Element::rect() const{
    return _rect;
}

#endif // ELEMENT_H
