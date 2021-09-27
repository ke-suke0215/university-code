#include <stdio.h>

int main(void)
{
    double t;      //時間
    double dt;     //時間の刻み
    double x;      //現在のx
    double x_next; //次のx
    double v;      //現在のv
    double v_next; //次のv
    int m = 1, k = 2;

    //時間の幅を入力
    dt = 0.1;

    //初期条件
    x = 1.0;
    v = 0.0;

    for (t = 0; t <= 5; t = t + dt)
    {
        //値を出力
        printf("%f\t%f\n", t, x);

        //次のx,vを計算
        x_next = v * dt + x;
        v_next = v - k / m * x * dt;

        //次のx,vをそれぞれ代入
        x = x_next;
        v = v_next;
    }

    return 0;
}