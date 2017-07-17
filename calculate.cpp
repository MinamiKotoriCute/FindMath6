#include "calculate.h"
#include <QDebug>

//來源：
//http://openhome.cc/Gossip/AlgorithmGossip/PostfixCal.htm

Calculate::Calculate(MathLine *mathline) : Lines(mathline)
{
    number = Lines->elements().size();
}

double Calculate::ask(double x){
    //取代X
    int *tem, *tem2, n=0;
    tem = new int[Lines->elements().size()];
    tem2 = new int[Lines->elements().size()];
    for(int i=0;i<Lines->elements().size();i++){
        if(Lines->elements()[i]->type()==2 && ((int)Lines->elements()[i]->know()==88 || (int)Lines->elements()[i]->know()==120)){
            tem[n]=i;
            tem2[n]=(int)Lines->elements()[i]->know();
            Lines->elementsRemoveAt(i, true);
            Lines->elementsAdd(new Element(1, x), i);
            n++;
        }
    }

    int *Sequence = new int[number+1];
    for(int i=0;i<number;i++)
        Sequence[i] = i;
    Sequence[number]=-1;
    double ans = eval(Sequence);

    //還原
    for(int i=0;i<n;i++){
        //qDebug() << tem[i] << tem2[i];
        Lines->elementsRemoveAt(tem[i], true);
        Lines->elementsAdd(new Element(2, tem2[i]), tem[i]);
    }

    delete [] tem;
    delete [] tem2;
    delete [] Sequence;

    return ans;
}

void Calculate::inToPostfix(int* infix, int* postfix) {
    int *stack = new int[number+1];
    stack[0] = 0;
    int i, j, top;
    for (i = 0, j = 0, top = 0; infix[i] != -1; i++){
        if(Lines->elements()[infix[i]]->type()==1){
            postfix[j++] = infix[i];
            continue;
        }
        switch ((int)Lines->elements()[infix[i]]->know()) {
        case '(':              // 運算子堆疊
            stack[++top] = infix[i];
            break;
        case '+': case '-': case '*': case '/': case '^':
            while (priority((int)Lines->elements()[stack[top]]->know()) >= priority((int)Lines->elements()[infix[i]]->know())) {
                postfix[j++] = stack[top--];
            }
            stack[++top] = infix[i]; // 存入堆疊
            break;
        case ')':
            while ((int)Lines->elements()[stack[top]]->know() != '(') { // 遇 ) 輸出至 (
                postfix[j++] = stack[top--];
            }
            top--;  // 不輸出 (
            break;
        //default:  // 運算元直接輸出
            //qDebug() << "GG 1";
        }
    }
    while (top > 0) {
        postfix[j++] = stack[top--];
    }
    delete [] stack;
}

double Calculate::eval(int* infix) {
    int *postfix = new int[number+1];
    for(int i=0;i<number+1;i++)
        postfix[i]=-1;
    double *stack = new double[number+1];

    inToPostfix(infix, postfix);

    int top, i;
    for (top = 0, i = 0; postfix[i] != -1; i++){
        if(Lines->elements()[postfix[i]]->type()!=3){
            stack[++top] = Lines->elements()[postfix[i]]->know();
            continue;
        }
        switch ((int)Lines->elements()[postfix[i]]->know()) {
        case '+': case '-': case '*': case '/': case '^':
            stack[top - 1] = cal((int)Lines->elements()[postfix[i]]->know(), stack[top - 1], stack[top]);
            top--;
            break;
        //default:
            //qDebug() << "GG 2";
        }
        //qDebug() << stack[top - 1+1] << stack[top+1] << (int)Lines->elements()[postfix[i]]->know() << stack[top];
    }

    double ans = stack[top];

    delete [] postfix;
    delete [] stack;

    return ans;
}

int Calculate::priority(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': return 3;
    default:            return 0;
    }
}

double Calculate::cal(char op, double p1, double p2) {
    switch (op) {
    case '+': return p1 + p2;
    case '-': return p1 - p2;
    case '*': return p1 * p2;
    case '/': return p1 / p2;
    case '^': return pow(p1, p2);
    }
}
