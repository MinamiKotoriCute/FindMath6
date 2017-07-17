#ifndef MATHLINE_H
#define MATHLINE_H

#include "element.h"

#define GOODDEBUG 0 //(0:不顯示 1:顯示)

class MathLine
{
public:
    MathLine(int Layer = 0);
    ~MathLine();

    bool analysis();    //分析步驟 (回傳 0:分析成功 1:分析失敗)
    QString show(); //回傳數列內容
    void addBrackets(); //補括號
    MathLine *allInOne();   //把數列化成1個數列(沒有子數列)
    bool canDraw(); //檢查是否可以畫圖
    void debug_ShowAll();   //輸出完整數列資料
    void debug_ShowAll_more();  //輸出完整數列資料(更多)

    void elementsAdd(Element *newelement, int moveTo = -1);
    void elementsRemoveAt(int removeAt, bool isDelete = false);
    void elementsMove(MathLine *from, int moveFrom, MathLine *to, int moveTo = -1);
    void submathlinesAdd(MathLine *newsubmathline, int moveTo = -1);
    void submathlinesRemoveAt(int removeAt, bool isDelete = false);
    QRect rect();
    void setlayer(int newlayer);

    inline const QList<Element *>& elements() const;
    inline const QList<MathLine *>& submathlines() const;

private:
    void sequence();    //排序 (由小到大)
    void judge();   //找字 (初步判斷元素的資料)
    void removeInner(); //刪除內圈的未知元素
    void fraction();    //尋找分數
    void radical(); //尋找根號
    void numberConnected(); //數字相連
    void functionSplitPower();  //次方
    void multiply();    //補乘號

    void removeElement();   //測試ing

    void elementsMerger(int n1, int n2);

    bool findError;

    int _layer;
    QList<Element *> _elements;
    QList<MathLine *> _submathlines;
};

inline const QList<Element *>& MathLine::elements() const{
    return _elements;
}

inline const QList<MathLine *>& MathLine::submathlines() const{
    return _submathlines;
}

#endif // MATHLINE_H
