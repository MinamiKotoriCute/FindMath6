#ifndef CALCULATE_H
#define CALCULATE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mathline.h>

class Calculate
{
public:
    Calculate(MathLine *mathline);
    double ask(double x);

private:
    void inToPostfix(int*, int*); // 中序轉後序
    int priority(char); // 運算子優先
    double eval(int*);
    double cal(char, double, double);

    MathLine *Lines;
    int number;
};

#endif // CALCULATE_H
