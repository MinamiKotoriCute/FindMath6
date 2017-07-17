#include "mathline.h"

#include <QDebug>

MathLine::MathLine(int Layer) : _layer(Layer)
{

}

MathLine::~MathLine(){
    qDeleteAll(_elements);
    _elements.clear();
    qDeleteAll(_submathlines);
    _submathlines.clear();
}

bool MathLine::canDraw(){
    //y=
    if(_elements.size()>=2){
        if(_elements[0]->type()==2 && (_elements[0]->know()==89 || _elements[0]->know()==121) &&
                _elements[1]->type()==3 && _elements[1]->know()==61){
            elementsRemoveAt(0);
            elementsRemoveAt(0);
        }
    }

    //f(x)=
    if(_elements.size()>=5){
        if(_elements[0]->type() == 2 && (_elements[0]->know() == 70 ||_elements[0]->know() == 102) &&
                _elements[1]->type() == 3 && _elements[1]->know() == 40 &&
                _elements[2]->type() == 2 && (_elements[2]->know() == 88 || _elements[2]->know() == 120) &&
                _elements[3]->type() == 3 && _elements[3]->know() == 41 &&
                _elements[4]->type() == 3 && _elements[4]->know() == 61 ){
            elementsRemoveAt(0);
            elementsRemoveAt(0);
            elementsRemoveAt(0);
            elementsRemoveAt(0);
            elementsRemoveAt(0);
        }
    }

    //有除了x以外的英文，就不能畫
    for(int i=0;i<_elements.size();i++)
        if(_elements[i]->type()==2)
            if(_elements[i]->know()!=88 && _elements[i]->know()!=120)
                return false;

    //刪除等於和它左邊的字
    for(int i=0;i<_elements.size();i++)
        if(_elements[i]->type() == 3 && _elements[i]->know() == 61){
            for(int j=i;j>=0;j--)
                elementsRemoveAt(j);
            break;
        }

    return true;
}

bool MathLine::analysis(){
    findError = false;

    if(_layer==0){
        //排序 (由小到大)
        sequence();
        if(GOODDEBUG)
            qDebug() << "Success MathLine::analysis(), sequence()";

        //刪除第1個元素
        if(_elements.size()>=1)
            _elements.removeFirst();
        else{
            qDebug() << "error in MathLine::analysis(), elements is empty.";
            return 1;
        }

        //找字 (初步判斷元素的資料)
        judge();
        if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), judge()";

        //刪除內圈的未知元素
        removeInner();
        if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), removeInner()";
    }

    //尋找分數
    fraction();
    if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), fraction()";
    if(findError) return 1;

    //尋找根號
    radical();
    if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), radical()";
    if(findError) return 1;

    //數字相連
    numberConnected();
    if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), numberConnected()";
    if(findError) return 1;

    //次方
    functionSplitPower();
    if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), functionSplitPower()";
    if(findError) return 1;

    //補乘號
    multiply();
    if(GOODDEBUG) qDebug() << "Success MathLine::analysis(), multiply()";
    if(findError) return 1;

    for(int i=0;i<_submathlines.size();i++){
        if(GOODDEBUG) qDebug() << "Layer:" << _layer+1;
        if(_submathlines[i]->analysis()) return 1;
    }

    return 0;
}

void MathLine::removeElement(){
    //刪除未知元素中沒有已知元素的元素
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()!=-1)
            continue;
        bool deleteElement = true;
        for(int j=i+1;j<_elements.size();j++){
            if(_elements[i]->rect().x() <= _elements[j]->rect().x() &&
                    _elements[i]->rect().right() >= _elements[j]->rect().right() &&
                    _elements[i]->rect().y() <= _elements[j]->rect().y() &&
                    _elements[i]->rect().bottom() >= _elements[j]->rect().bottom()){
                deleteElement = false;
            }
            else if(_elements[i]->rect().right() < _elements[j]->rect().x())
                break;
        }
        if(deleteElement){
            _elements.removeAt(i);
            i--;
        }
    }
}

void MathLine::multiply(){
    for(int i=0;i<_elements.size()-1;i++){
        if(_elements[i]->type()==3 || _elements[i+1]->type()==3)
            continue;
        elementsAdd(new Element(3, 42), i+1);
        i++;
    }
}

void MathLine::functionSplitPower(){
    //根據研究發現    n2的最下面比n1高 => 次方
    //寫一些容許值    就n1高度*0.05吧
    //容許值2  如果n2的類型是3
    const double heightError = 0.05;
    const double heightError2 = 0.4;
    for(int i=0;i<_elements.size()-1;i++){
        //qDebug() << "c1" << i << _elements.size()-1 << _elements[i]->type() << _elements[i]->know();
        if(_elements[i]->type()==3 || _elements[i+1]->type()==3)
            continue;

        MathLine *newMathLine = NULL;
        for(int j=i+1;j<_elements.size();j++){
            //qDebug() << "c2" << j << _elements.size() << _submathlines.size() << _elements[j]->type() << _elements[j]->know();
            if(_elements[j]->know()>=_submathlines.size() && _elements[j]->type()==4){
                qDebug() << "warning in MathLine::functionSplitPower().";
                findError = true;
                return;
            }

            QRect rect1, rect2;
            if(_elements[i]->type()==4)
                rect1 = _submathlines[(int)_elements[i]->know()]->rect();
            else
                rect1 = _elements[i]->rect();
            if(_elements[j]->type()==4)
                rect2 = _submathlines[(int)_elements[j]->know()]->rect();
            else
                rect2 = _elements[j]->rect();

            if((rect1.bottom() - rect1.height() * heightError > rect2.bottom() && _elements[j]->type()!=3) ||
                    (((rect1.bottom() - rect1.height() * heightError2 > rect2.bottom() && _elements[j]->know()!=45) ||
                      (rect1.bottom() - rect1.height() * 0.6 > rect2.bottom() && _elements[j]->know()==45)) && _elements[j]->type()==3)){
                //if(_elements[j]->know()==45 )
                if(newMathLine == NULL)
                    newMathLine = new MathLine(_layer+1);
                elementsMove(this, j, newMathLine);
                j--;
            }
            else
                break;
        }
        if(newMathLine != NULL){
            submathlinesAdd(newMathLine, i+1);
            elementsAdd(new Element(3, 94), i+1);
        }
    }
}

void MathLine::numberConnected(){
    for(int i=0;i<_elements.size()-1;i++){
        if(_elements[i]->type() == 1 && _elements[i+1]->type() == 1){
            const double heightError = 0.9;
            if(_elements[i]->rect().height() > _elements[i+1]->rect().height() * heightError &&
                    _elements[i]->rect().height() * heightError < _elements[i+1]->rect().height()){
                double total = _elements[i]->know() * 10 + _elements[i+1]->know();
                //合併
                elementsMerger(i, i+1);
                _elements[i]->set(1, total);
                i--;
            }
        }
    }
}

void MathLine::radical(){
    for(int i=0;i<_elements.size()-1;i++){
        if(!(_elements[i]->rect().x() < _elements[i+1]->rect().center().x() &&
                _elements[i]->rect().right() > _elements[i+1]->rect().center().x() &&
                _elements[i]->rect().y() < _elements[i+1]->rect().center().y() &&
                _elements[i]->rect().bottom() > _elements[i+1]->rect().center().y()))
            continue;

        MathLine *newMathLine = new MathLine(_layer+1);
        for(int j=i+1;j<_elements.size();j++){
            if(_elements[i]->rect().x() < _elements[i+1]->rect().center().x() &&
                    _elements[i]->rect().right() > _elements[i+1]->rect().center().x() &&
                    _elements[i]->rect().y() < _elements[i+1]->rect().center().y() &&
                    _elements[i]->rect().bottom() > _elements[i+1]->rect().center().y()){
                newMathLine->elementsAdd(_elements[j]);
                _elements.removeAt(j);
                j--;
            }
            else
                break;
        }
        _elements.removeAt(i);
        MathLine *newMathLine2 = new MathLine(_layer+1);
        newMathLine2->elementsAdd(new Element(1, 1));
        newMathLine2->elementsAdd(new Element(3, 47));
        newMathLine2->elementsAdd(new Element(1, 2));
        submathlinesAdd(newMathLine);
        elementsAdd(new Element(3, 94));
        submathlinesAdd(newMathLine2);

        i+=2;
    }
}

void MathLine::fraction(){
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->rect().width() < _elements[i]->rect().height() * 5)
            continue;

        int isfraction = false;
        for(int j=i+1;j<_elements.size();j++){
            if(_elements[i]->rect().x() < _elements[j]->rect().center().x() &&
                    _elements[i]->rect().right() > _elements[j]->rect().center().x() &&
                    (_elements[i]->rect().y() < _elements[j]->rect().y() || _elements[i]->rect().y() > _elements[j]->rect().y())){
                isfraction = true;
                break;
            }
            if(_elements[i]->rect().right() < _elements[j]->rect().x())
                break;
        }
        if(!isfraction)
            continue;

        MathLine *upMathLine = new MathLine(_layer+1);
        for(int j=i+1;j<_elements.size();j++){
            if(_elements[i]->rect().x() < _elements[j]->rect().center().x() &&
                    _elements[i]->rect().right() > _elements[j]->rect().center().x() &&
                    _elements[i]->rect().y() > _elements[j]->rect().y()){
                elementsMove(this, j, upMathLine);
                //upMathLine->elementsAdd(_elements[j]);
                //_elements.removeAt(j);
                j--;
            }
            if(_elements[i]->rect().right() < _elements[j]->rect().x())
                break;
        }

        MathLine *downMathLine = new MathLine(_layer+1);
        for(int j=i+1;j<_elements.size();j++){
            if(_elements[i]->rect().x() < _elements[j]->rect().center().x() &&
                    _elements[i]->rect().right() > _elements[j]->rect().center().x() &&
                    _elements[i]->rect().y() < _elements[j]->rect().y()){
                elementsMove(this, j, downMathLine);
                //downMathLine->elementsAdd(_elements[j]);
                //_elements.removeAt(j);
                j--;
            }
            if(_elements[i]->rect().right() < _elements[j]->rect().x())
                break;
        }

        _elements.removeAt(i);
        submathlinesAdd(downMathLine, i);
        elementsAdd(new Element(3, 47), i);
        submathlinesAdd(upMathLine, i);
        i+=2;
    }
}

void MathLine::removeInner(){
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()==1 && _elements[i]->know()!=-1)
            for(int j=i+1;j<_elements.size();j++){
                if(_elements[i]->rect().x() < _elements[j]->rect().x() &&
                        _elements[i]->rect().right() > _elements[j]->rect().right() &&
                        _elements[i]->rect().y() < _elements[j]->rect().y() &&
                        _elements[i]->rect().bottom() > _elements[j]->rect().bottom()){
                    //移除
                    _elements.removeAt(j);
                    j--;
                }
            }
    }
}

void MathLine::judge(){
    //找=	(中心點差不多、寬度差不多、扁扁的)
    for(int i=0;i<_elements.size()-1;i++){
        const double centerError = 0.05; //中心點誤差
        const double widthError = 0.05;  //寬度誤差
        if(_elements[i]->rect().center().x() <= _elements[i+1]->rect().center().x() + _elements[i+1]->rect().width() * centerError &&
                _elements[i]->rect().width() <= _elements[i+1]->rect().width() * (1+widthError) &&
                _elements[i]->rect().width() >= _elements[i+1]->rect().width() * (1-widthError) &&
                _elements[i]->rect().width() > _elements[i]->rect().height() * 4){
            if(_elements[i]->stat()!=0 || _elements[i+1]->stat()!=0)
                continue;
            //合併
            elementsMerger(i, i+1);
            _elements[i]->set(3, 61);
            break;
        }
    }

    //找-    (扁扁的)
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->rect().height() * 4 < _elements[i]->rect().width() &&
                _elements[i]->know()==-1)
            _elements[i]->set(3, 45);
    }

    //利用ascii尋找
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->stat()!=0)
            continue;

        //數字
        if(48 <= _elements[i]->data()[0] && _elements[i]->data()[0] < 48+10){
            _elements[i]->set(1, _elements[i]->data()[0].toLatin1() - 48);
            continue;
        }

        //大寫英文
        else if(65 <= _elements[i]->data()[0] && _elements[i]->data()[0].toLatin1() < 65+26){
            _elements[i]->set(2, _elements[i]->data()[0].toLatin1());
            continue;
        }

        //小寫英文
        else if(97 <= _elements[i]->data()[0] && _elements[i]->data()[0] < 97+26){
            _elements[i]->set(2, _elements[i]->data()[0].toLatin1());
            continue;
        }

        //+
        else if(_elements[i]->data()[0] == 43){
            _elements[i]->set(3, 43);
            continue;
        }

        //(
        else if(_elements[i]->data()[0] == 40){
            _elements[i]->set(3, 40);
            continue;
        }

        //)
        else if(_elements[i]->data()[0] == 41){
            _elements[i]->set(3, 41);
            continue;
        }

        //例外
        else{
            QString a;
            a.sprintf("\ncan't find:%d %c %d", i, _elements[i]->data()[0], _elements[i]->data()[0]);
            //qDebug() << a.toLatin1().data();
        }
    }
}

void MathLine::sequence(){
    //排序    (依照左上角x值，由小到大)
    for(int i=0;i<_elements.size();i++)
        for(int j=0;j<_elements.size()-1;j++)
            if(_elements[j]->rect().x() > _elements[j+1]->rect().x())
                _elements.move(j,j+1);
}

void MathLine::addBrackets(){
    for(int i=0;i<_submathlines.size();i++){
        int previous=-1;
        for(int j=0;j<_elements.size();j++)
            if((int)_elements[j]->know() == i && _elements[j]->type()==4){
                previous = j;
                break;
            }
        for(int j=0;j<_submathlines[i]->elements().size();j++){
            //分數補括號
            if(_submathlines[i]->elements()[j]->type()==3 &&
                    (_submathlines[i]->elements()[j]->know()==45 ||
                     _submathlines[i]->elements()[j]->know()==43 ||
                    (i%2==0 && (_submathlines[i]->elements()[j]->know()==42 ||
                    _submathlines[i]->elements()[j]->know()==47)))){
                _submathlines[i]->elementsAdd(new Element(3, 40), 0);
                _submathlines[i]->elementsAdd(new Element(3, 41));
                break;
            }
            //次方補括號
            if(previous==-1 || previous==0)
                continue;
            if(_elements[previous-1]->type()==3 && _elements[previous-1]->know()==94)
                if(_submathlines[i]->elements().size()>1){
                    _submathlines[i]->elementsAdd(new Element(3, 40), 0);
                    _submathlines[i]->elementsAdd(new Element(3, 41));
                    break;
                }
        }
    }
    for(int i=0;i<_submathlines.size();i++)
        _submathlines[i]->addBrackets();
}

void MathLine::elementsMove(MathLine *from, int moveFrom, MathLine *to, int moveTo){
    if(from->elements()[moveFrom]->type()==4){
        to->submathlinesAdd(from->submathlines()[(int)from->elements()[moveFrom]->know()]);
        from->submathlinesRemoveAt(moveFrom);
        //from->elementsRemoveAt(moveFrom);
    }
    else{
        to->elementsAdd(from->elements()[moveFrom], moveTo);
        from->elementsRemoveAt(moveFrom);
    }
}

void MathLine::submathlinesRemoveAt(int removeAt, bool isDelete){
    if(removeAt<0 || removeAt>=_elements.size())
        return;
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()==4 && _elements[i]->know() > _elements[removeAt]->know()){
            elementsAdd(new Element(4, _elements[i]->know()-1), i);
            elementsRemoveAt(i+1);
        }
    }
    if(isDelete)
        delete _submathlines[(int)_elements[removeAt]->know()];
    _submathlines.removeAt((int)_elements[removeAt]->know());
    delete _elements[removeAt];
    _elements.removeAt(removeAt);
}

QRect MathLine::rect(){
    if(_elements.size()==0)
        return QRect();
    int x = _elements[0]->rect().x();
    int y = _elements[0]->rect().y();
    int right = _elements[0]->rect().right();
    int bottom = _elements[0]->rect().bottom();
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()!=4){
            if(x>_elements[i]->rect().x())
                x = _elements[i]->rect().x();
            if(y>_elements[i]->rect().y())
                y = _elements[i]->rect().y();
            if(right<_elements[i]->rect().right())
                right=_elements[i]->rect().right();
            if(bottom<_elements[i]->rect().bottom())
                bottom=_elements[i]->rect().bottom();
        }
        else{
            int xx = _submathlines[(int)_elements[i]->know()]->rect().x();
            if(x > xx)
                x=xx;
            int yy = _submathlines[(int)_elements[i]->know()]->rect().y();
            if(y > yy)
                y=yy;
            int rright = _submathlines[(int)_elements[i]->know()]->rect().right();
            if(right < rright)
                right = rright;
            int bbottom = _submathlines[(int)_elements[i]->know()]->rect().bottom();
            if(bottom < bbottom)
                bottom = bbottom;
        }
    }
    return QRect(x, y, (right-x), (bottom-y));
}

void MathLine::debug_ShowAll(){
    if(_layer==0)
        qDebug() << "****************************************\n"
                    "*          Debug sholl all             *\n"
                    "****************************************\n";

    QString space;
    for(int i=0;i<_layer;i++)
        space+="   ";
    qDebug() << space.toLatin1().data() << "layer: " << _layer;
    qDebug() << space.toLatin1().data() << "elements size: " << _elements.size();
    qDebug() << space.toLatin1().data() << "submathline size: " << _submathlines.size() << "\n";
    qDebug() << space.toLatin1().data() << "[elements]";
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()==1)
            qDebug() << space.toLatin1().data() << i << " " << (int)_elements[i]->know();
        else if(_elements[i]->type()==2 || _elements[i]->type()==3){
            QString str;
            str.sprintf("%c", (int)_elements[i]->know());
            qDebug() << space.toLatin1().data() << i << " " << str.toLatin1().data();
        }
        else if(_elements[i]->type()==4){
            QString str;
            str.sprintf("[%d]", (int)_elements[i]->know());
            qDebug() << space.toLatin1().data() << i << " " << str.toLatin1().data();
        }
        else
            qDebug() << space.toLatin1().data() << i << " " << _elements[i]->data()[0];
    }

    for(int i=0;i<_submathlines.size();i++){
        QString tem;
        tem.sprintf("[submathline][%d]", i);
        qDebug() << space.toLatin1().data() << tem.toLatin1().data();
        _submathlines[i]->debug_ShowAll();
    }
}

void MathLine::debug_ShowAll_more(){
    if(_layer==0)
        qDebug() << "****************************************\n"
                    "*          Debug sholl all             *\n"
                    "****************************************\n";

    QString space;
    for(int i=0;i<_layer;i++)
        space+="   ";
    qDebug() << space.toLatin1().data() << "layer: " << _layer;
    qDebug() << space.toLatin1().data() << "elements size: " << _elements.size();
    qDebug() << space.toLatin1().data() << "submathline size: " << _submathlines.size() << "\n";
    qDebug() << space.toLatin1().data() << "[elements]";
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()==1)
            qDebug() << space.toLatin1().data() << i << " " << (int)_elements[i]->know();
        else if(_elements[i]->type()==2 || _elements[i]->type()==3){
            QString str;
            str.sprintf("%c", (int)_elements[i]->know());
            qDebug() << space.toLatin1().data() << i << " " << str.toLatin1().data() << " " << _elements[i]->rect();
        }
        else if(_elements[i]->type()==4){
            QString str;
            str.sprintf("[%d]", (int)_elements[i]->know());
            qDebug() << space.toLatin1().data() << i << " " << str.toLatin1().data();
        }
        else
            qDebug() << space.toLatin1().data() << i << " " << _elements[i]->data()[0];
    }

    for(int i=0;i<_submathlines.size();i++){
        QString tem;
        tem.sprintf("[submathline][%d]", i);
        qDebug() << space.toLatin1().data() << tem.toLatin1().data();
        _submathlines[i]->debug_ShowAll();
    }
}

QString MathLine::show(){
    QString re;
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()==1)
            re += QString::number((int)_elements[i]->know()) + " ";
        else if(_elements[i]->type()==2 || _elements[i]->type()==3){
            QString str;
            str.sprintf("%c", (int)_elements[i]->know());
            re += str + " ";
        }
        else if(_elements[i]->type()==4){
            re += _submathlines[(int)_elements[i]->know()]->show();
        }
    }
    return re;
}

MathLine* MathLine::allInOne(){
    MathLine *lines = new MathLine;
    for(int i=0;i<_elements.size();i++){
        if(_elements[i]->type()!=4)
            lines->elementsAdd(_elements[i]);
        else{
            MathLine *sublines = _submathlines[(int)_elements[i]->know()]->allInOne();
            for(int j=0;j<sublines->elements().size();j++)
                lines->elementsAdd(sublines->elements()[j]);
        }
    }
    return lines;
}

void MathLine::setlayer(int newlayer){
    _layer = newlayer;
}

void MathLine::submathlinesAdd(MathLine *newsubmathline, int moveTo){
    newsubmathline->setlayer(_layer+1);
    _submathlines << newsubmathline;
    elementsAdd(new Element(4, _submathlines.size()-1), moveTo);
}

void MathLine::elementsRemoveAt(int removeAt, bool isDelete){
    if(removeAt>=_elements.size() || removeAt<0)
        return;
    if(_elements[removeAt]->type()==4)
        submathlinesRemoveAt(removeAt);
    else{
        if(isDelete)
            delete _elements[removeAt];
        _elements.removeAt(removeAt);
    }
}

void MathLine::elementsAdd(Element *newelement, int moveTo){
    _elements << newelement;
    if(moveTo>=0 && moveTo<_elements.size()-1)
        _elements.move(_elements.size()-1, moveTo);
}

void MathLine::elementsMerger(int n1, int n2){
    if(n1==n2 || n1>=_elements.size() || n1<0 ||
            n2>=_elements.size() || n2<0)
        return;

    QRect newrect;
    newrect.setX(qMin(_elements[n1]->rect().x(), _elements[n2]->rect().x()));
    newrect.setY(qMin(_elements[n1]->rect().y(), _elements[n2]->rect().y()));
    newrect.setRight(qMax(_elements[n1]->rect().right(), _elements[n2]->rect().right()));
    newrect.setBottom(qMax(_elements[n1]->rect().bottom(), _elements[n2]->rect().bottom()));

    _elements.removeAt(qMax(n1, n2));
    _elements.removeAt(qMin(n1, n2));
    elementsAdd(new Element(newrect), qMin(n1, n2));
}
