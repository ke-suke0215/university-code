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
    int m = 1, k = 2;

    FILE *output_1x;
    output_1x = fopen("output_1x.txt", "w");
    FILE *output_2x;
    output_2x = fopen("output_2x.txt", "w");

    FILE *output_1v;
    output_1v = fopen("output_1v.txt", "w");
    FILE *output_2v;
    output_2v = fopen("output_2v.txt", "w");

    //時間の幅を入力
    dt = 0.01;

    //初期条件
    x1 = 0.0;
    v1 = 0.0;

    x2 = 1.0;
    v2 = 0.0;

    //計算で使用する定数定義
    double omga = sqrt(k / m);
    double omgb = sqrt(3 * k / m);

    for (t = 0; t <= 5; t = t + dt)
    {
        //値を出力
        fprintf(output_1x, "%f\t%f\n", t, (cos(omga * t) - cos(omgb * t)) / 2);
        fprintf(output_1v, "%f\t%f\n", t, -(omga * sin(omga * t) - omgb * sin(omgb * t)) / 2);

        fprintf(output_2x, "%f\t%f\n", t, (cos(omga * t) + cos(omgb * t)) / 2);
        fprintf(output_2v, "%f\t%f\n", t, -(omga * sin(omga * t) + omgb * sin(omgb * t)) / 2);
    }
    fclose(output_1x);
    fclose(output_2x);
    fclose(output_1v);
    fclose(output_2v);

    return 0;
}