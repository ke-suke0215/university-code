#include <stdio.h>

double f1(double t, double x, double v);
double f2(double t, double x, double v);

int main()
{
    double t, x, v, dt, tmax;
    double k1[2], k2[2], k3[2], k4[2];

    x = 1.0;    //位置の初期値
    v = 0.0;    //速度の初期値
    dt = 0.01;  //刻み幅
    tmax = 5.0; //繰り返し最大回数

    FILE *output_x001;
    FILE *output_v001;
    output_x001 = fopen("output_x001.txt", "w");
    output_v001 = fopen("output_v001.txt", "w");

    for (t = 0; t < tmax; t += dt)
    {
        k1[0] = dt * f1(t, x, v);
        k1[1] = dt * f2(t, x, v);

        k2[0] = dt * f1(t + dt / 2.0, x + k1[0] / 2.0, v + k1[1] / 2.0);
        k2[1] = dt * f2(t + dt / 2.0, x + k1[0] / 2.0, v + k1[1] / 2.0);

        k3[0] = dt * f1(t + dt / 2.0, x + k2[0] / 2.0, v + k2[1] / 2.0);
        k3[1] = dt * f2(t + dt / 2.0, x + k2[0] / 2.0, v + k2[1] / 2.0);

        k4[0] = dt * f1(t + dt, x + k3[0], v + k3[1]);
        k4[1] = dt * f2(t + dt, x + k3[0], v + k3[1]);

        x = x + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
        v = v + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;

        fprintf(output_x001, "%f\t%f\n", t, x);
        fprintf(output_v001, "%f\t%f\n", t, v);
    }

    fclose(output_x001);
    fclose(output_v001);

    return 0;
}

double f1(double t, double x, double v)
{
    return v;
}

double f2(double t, double x, double v)
{
    double m = 1;
    double k = 2;

    return (-k / m * x);
}