#ifndef HMM_URN_BALL
#define HMM_URN_BALL

//--------debug----------//
#define _DEBUG_

#ifdef _DEBUG_

#include <iostream>
#define DEBUG std::cout << "DEBUG " <<
#define ENDL << std::endl

#endif //DEBUG
//--------debug----------//

#include <algorithm>
#include <vector>
#include <numeric>
#include <random>

class Urn
{
public:
    Urn();
    ~Urn();

    bool SetBallKind(int kind); //ボールの種類を設定する関数

    std::vector<double> ball; //各ボールが出る確率
    double loop;              //自己ループの確率
    double trans;             //次の壺に遷移する確率

private:
    int ball_kind;
};

class HMM_urn_ball
{
public:
    HMM_urn_ball(int urn_n, int ball_n);
    ~HMM_urn_ball();
    std::vector<std::vector<double>> GetBallMatrix();                                   //ボールの出現確率を配列化して返す
    int GetUrnNum();                                                                    //壺の数を返す
    int GetBallNum();                                                                   //ボールの種類を返す
    bool HMMLearning(std::vector<int> data, int cl_max = 100, double err = 0.00000001); //学習する関数 (data：学習するデータ(int型vector), cl_max：最大計算回数(int型)，error：収束判定の誤差(double型))
    double HMMMeasure(std::vector<int> data);                                           //データの評価を行う関数(data：評価するデータ(int型vector))

private:
    int factorial(int x);                                   //xの階乗を返す関数
    std::vector<Urn> urn;                                   //壺とボール
    std::vector<std::vector<std::vector<int>>> ball_count;  //出てきたボールの種類を壺ごとに分けて格納
    std::vector<std::vector<std::vector<int>>> trans_count; //遷移回数を保存
    std::vector<int> trans_buff;                            //遷移方向を格納
    std::vector<double> prob_buff;                          //各パスの確率を格納する
    std::vector<int> learn_data;                            //学習するデータを格納

    int urn_num;  //壺の数
    int ball_num; //ボールの種類
    int path_num; //パスの総数
    double prob_sum;
};

#endif
