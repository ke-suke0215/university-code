#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// グローバル変数を定義
int N = 200; // ブロック数
double param_a_general, param_a_slow, param_b, param_c, plate_v, special_v, d, l_general, l_slow, time, t_start, t_max, dt, zero, random_num;

// main外に記述する関数を定義
double f(double v);
double determine_friction_direction(double others, double friction, double v);
double g_left(double t, double x_i0, double x_i, double x_ip1, double v, double theta, double param_a, double l);
double g_inside(double t, double x_i0, double x_im1, double x_i, double x_ip1, double v, double theta, double param_a, double l);
double g_right(double t, double x_i0, double x_im1, double x_i, double v, double theta, double param_a, double l);
double h(double v, double theta);

int main()
{
  // cal_count:ループの回数を格納する変数、s:for文で使用する変数
  int cal_count, s;

  //ブロックの位置、速度、状態変数の長さNの配列
  double x_initial[N], x[N], v[N], theta[N];

  // ルンゲクッタ法で使用するk,l,mの値を格納する N×4 の配列
  double rk_k[N][4], rk_l[N][4], rk_m[N][4];

  // 通常の地震とゆっくり地震を判断するためのパラメータaとばね定数lを格納する配列
  double param_a[N], l[N];

  ///////////////////////////
  /// 定義した変数に値を代入 ///
  ///////////////////////////

  param_a_general = pow(10.0, -5.0); // 通常の地震の摩擦パラメータa
  param_a_slow = pow(10.0, -1.0);    // ゆっくり地震の摩擦パラメータa
  param_b = pow(10.0, -3.0);         // 摩擦パラメータb
  param_c = pow(10.0, -3.0);         // 摩擦パラメータc

  plate_v = pow(10.0, -2.0);   // プレートの速度
  special_v = pow(10.0, -1.0); // 特徴的な速度
  d = pow(10.0, -6.0);         //バネの自然長

  l_general = 2000.0; // 通常の地震での無次元化されたばね定数
  l_slow = 0.2;       // ゆっくり地震での無次元化されたばね定数

  time = 0.0;              // 時間計測用変数
  t_start = 100.0;         // 計測開始時間
  t_max = 250.0;           // 計測終了時間
  dt = pow(10.0, -6.0);    // 時間の刻み幅
  cal_count = 0;           // 計算回数のカウント
  zero = pow(10.0, -10.0); // プレートが逆に滑るのを防ぐための値

  // 通常の地震とゆっくり地震が起こるパラメータを代入
  for (s = 0; s < N; s++)
  {
    if (s < N / 2) // 通常の地震
    {
      param_a[s] = param_a_general;
      l[s] = l_general;
    }
    else if (s >= N / 2) // ゆっくり地震
    {
      param_a[s] = param_a_slow;
      l[s] = l_slow;
    }
    else
    {
      printf("不適切な値で計算が行われています。");
      return 0;
    }
  }

  // x_initial, x に初期位置を代入する
  x_initial[0] = 0.0;
  x[0] = 0.0;
  // N=200の場合、ブロック全体の長さは 100±5 程度に収まる
  for (s = 1; s < N; s++)
  {
    // 0~1の乱数を random_num に代入
    random_num = (double)rand() / (double)RAND_MAX;
    x_initial[s] = x_initial[s - 1] + random_num;
    x[s] = x[s - 1] + random_num;
  }

  // 速さと状態変数の初期値を代入
  for (s = 0; s < N; s++)
  {
    v[s] = 0.0;
    theta[s] = 0.0;
  }
  ////////// 値の代入終了 ////////////

  //////////////////////////////////
  ///// ブロックを動かすループ開始 /////
  //////////////////////////////////
  for (time = t_start; time < t_max; time += dt)
  {
    cal_count++; // ループの回数を記録

    ////////////////////////////////////////////////////////////////////
    ////// 各ブロックのルンゲクッタ法の計算で使用する k,l,m の1~4に値を代入 /////
    ////////////////////////////////////////////////////////////////////

    // k -> dx, l -> dv, m -> dθ にそれぞれ対応している
  }

  // データを記述するファイルの作成

  // FILE *OUTPUTFILE1;
  // FILE *OUTPUTFILE2;
  // FILE *OUTPUTFILE3;
  // FILE *OUTPUTFILE4;

  // OUTPUTFILE1 = fopen("region410x.txt", "w");
  // OUTPUTFILE2 = fopen("region410y.txt", "w");
  // OUTPUTFILE3 = fopen("region410f.txt", "w");
  // OUTPUTFILE4 = fopen("region410t.txt", "w");

  double g_left_num = g_left(100.0, 2.0, 1.0, 3.0, 1.0, 1000.0, 0.1, 0.2);
  printf("g_left：%.10f\n", g_left_num);

  // ファイルへの記述を終了
  // fclose(OUTPUTFILE1);
  // fclose(OUTPUTFILE2);
  // fclose(OUTPUTFILE3);
  // fclose(OUTPUTFILE4);
}

//////////////////////////////////////////////
/// ルンゲクッタ法で使用する方程式を返す関数を記述 ///
//////////////////////////////////////////////

// dxの方程式を返す関数
double
f(double v)
{
  return v;
}

// 3つのdvの方程式の関数で使用する、摩擦の進行方向に対して摩擦の有無と向きを決める関数
// 引数は運動方程式の摩擦以外の部分（others）、運動方程式の摩擦部分（friction）、ブロックの速度(v)
double determine_friction_direction(double others, double friction, double v)
{
  if (v == 0)
  {
    if (others - friction > 0)
    {
      return (others - friction);
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return others - friction * v / fabs(v);
  }
}

// dv(左のブロック)の方程式を返す関数
double g_left(double t, double x_i0, double x_i, double x_ip1, double v, double theta, double param_a, double l)
{
  // 運動方程式の摩擦以外の部分を定義 （vによって摩擦の値が変更される場合があるため）
  double other_than_friction = plate_v * t - (x_i - x_i0) + l * (x_ip1 - x_i - d);
  // 運動方程式の摩擦力の部分を定義
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);

  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(左右以外のブロック)の方程式を返す関数
double g_inside(double t, double x_i0, double x_im1, double x_i, double x_ip1, double v, double theta, double param_a, double l)
{
  double other_than_friction = plate_v * t - (x_i - x_i0) + l * (x_ip1 - 2.0 * x_i - x_im1);
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);

  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(右のブロック)の方程式を返す関数
double g_right(double t, double x_i0, double x_im1, double x_i, double v, double theta, double param_a, double l)
{
  double other_than_friction = plate_v * t - (x_i - x_i0) + l * (x_im1 - x_i + d);
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);

  return determine_friction_direction(other_than_friction, friction, v);
}

// dθの方程式を返す関数
double h(double v, double theta)
{
  return 1.0 - (v * theta);
}