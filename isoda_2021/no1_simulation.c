#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// グローバル変数を定義
int N = 200; // ブロック数
double param_a_general, param_a_slow, param_b, param_c, plate_v, special_v, d, l_general, l_slow, time_measure, t_start, t_max, dt, zero;

// main外に記述する関数を定義
double determine_friction_direction(double others, double friction, double v);
double g_left(double t, double x_i, double x_i0, double x_ip1, double v, double theta, double param_a, double l);
double g_inside(double t, double x_i, double x_i0, double x_im1, double x_ip1, double v, double theta, double param_a, double l);
double g_right(double t, double x_i, double x_i0, double x_im1, double v, double theta, double param_a, double l);
double h(double v, double theta);

int main()
{
  int cal_count, s; // sはfor文で使用する変数

  ///////////////////////////////////
  /// 定義したグローバル変数に値を代入 ///
  ///////////////////////////////////

  param_a_general = pow(10.0, -5.0); // 通常の地震の摩擦パラメータa
  param_a_slow = pow(10.0, -1.0);    // ゆっくり地震の摩擦パラメータa
  param_b = pow(10.0, -3.0);         // 摩擦パラメータb
  param_c = pow(10.0, -3.0);         // 摩擦パラメータc

  plate_v = pow(10.0, -2.0);   // プレートの速度
  special_v = pow(10.0, -1.0); // 特徴的な速度
  d = pow(10.0, -6.0);         //バネの自然長

  l_general = 2000.0; // 通常の地震での無次元化されたばね定数
  l_slow = 0.2;       // ゆっくり地震での無次元化されたばね定数

  time_measure = 0.0;      // 時間計測用変数
  t_start = 100.0;         // 計測開始時間
  t_max = 250.0;           // 計測終了時間
  dt = pow(10.0, -6.0);    // 時間の刻み幅
  cal_count = 0;           // 計算回数のカウント
  zero = pow(10.0, -10.0); // プレートが逆に滑るのを防ぐための値

  ////////// 値の代入終了 ////////////
  double g_left_num = g_left(100.0, 2.0, 1.0, 3.0, 1.0, 1000.0, 0.1, 0.2);
  printf("g_left：%.10f\n", g_left_num);
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
double g_left(double t, double x_i, double x_i0, double x_ip1, double v, double theta, double param_a, double l)
{
  // 運動方程式の摩擦以外の部分を定義 （vによって摩擦の値が変更される場合があるため）
  double other_than_friction = plate_v * t - (x_i - x_i0) + l * (x_ip1 - x_i - d);
  // 運動方程式の摩擦力の部分を定義
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);

  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(左右以外のブロック)の方程式を返す関数
double g_inside(double t, double x_i, double x_i0, double x_im1, double x_ip1, double v, double theta, double param_a, double l)
{
  double other_than_friction = plate_v * t - (x_i - x_i0) + l * (x_ip1 - 2.0 * x_i - x_im1);
  double friction = param_c + param_a * log(1 + v / special_v) + param_b * log(theta);

  return determine_friction_direction(other_than_friction, friction, v);
}

// dv(右のブロック)の方程式を返す関数
double g_right(double t, double x_i, double x_i0, double x_im1, double v, double theta, double param_a, double l)
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