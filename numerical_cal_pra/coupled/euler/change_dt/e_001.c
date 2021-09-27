#include <stdio.h>
#include <math.h>

int main(void)
{
    double t;                //時間
    double dt;               //時間の刻み
    double x1, x2;           //現在のx
    double x1_next, x2_next; //次のx
    double v1, v2;           //現在のv
    double v1_next, v2_next; //次のv
    double m = 1.0, k = 2.0;
    double omg = k / m;

    FILE *output_e_001x;
    output_e_001x = fopen("output_e_001x.txt", "w");

    //時間の幅を入力
    dt = 0.01;

    //初期条件
    x1 = 0.0;
    v1 = 0.0;

    x2 = 1.0;
    v2 = 0.0;

    for (t = 0; t <= 5; t = t + dt)
    {
        //値を出力
        fprintf(output_e_001x, "%f\t%f\n", t, x1);

        //次のx,vを計算
        x1_next = v1 * dt + x1;
        v1_next = v1 - omg * (2 * x1 - x2) * dt;

        x2_next = v2 * dt + x2;
        v2_next = v2 - omg * (2 * x2 - x1) * dt;

        //次のx,vをそれぞれ代入
        x1 = x1_next;
        v1 = v1_next;

        x2 = x2_next;
        v2 = v2_next;
    }
    fclose(output_e_001x);

    return 0;
}