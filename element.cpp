#include "element.h"

Element::Element() : _know(-1), _type(-1) {}

Element::Element(QRect rect) :
        _rect(rect), _know(-1), _type(-1) {}

Element::Element(QRect rect, QString data) :
        _rect(rect), _data(data), _know(-1), _type(-1) {}

Element::Element(int type, double know) :
        _type(type), _know(know){}

void Element::set(int type, double know){
    if(type<0 || type>4)
        return;
    _know=know;
    _type=type;
}
