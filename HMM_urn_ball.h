#ifndef HMM_URN_BALL
#define HMM_URN_BALL

//--------debug----------//
#define _DEBUG_

#ifdef _DEBUG_

#include <iostream>
#define DEBUG std::cout <<
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
    double trans;             //次のツボに遷移する確率

private:
    int ball_kind;
};

class HMM_urn_ball
{
public:
    HMM_urn_ball(int urn_n, int ball_n);
    ~HMM_urn_ball();
    std::vector<std::vector<double>> GetBallMatrix();
    int GetUrnNum();                                                                       //ツボの数を返す
    int GetBallNum();                                                                      //ボールの種類を返す
    bool HMMLearning(std::vector<int> pattern, int cl_max = 100, double err = 0.00000001); //学習する関数
    double HMMMeasure(std::vector<int>);                                                   //評価を行う関数

private:
    std::vector<Urn> urn;                       //ツボとボール
    std::vector<std::vector<int>> ball_count;   //出てきたボールの種類をツボごとに分けて格納
    std::vector<std::vector<int>> trans_count;  //遷移回数をカウント 遷移trans_count[j][0] 自己ループtrans_count[j][1]
    std::vector<int> learn_pattern;             //学習するパターンを格納
    std::vector<std::vector<double>> prob_buff; //トレリスの各マスの確率を格納
    std::vector<std::vector<bool>> trans_buff;  //遷移方向を格納
    //std::vector<std::vector<int>> times_event; //ボールの出た回数や遷移した回数を保存
    int urn_num;  //ツボの数
    int ball_num; //ボールの種類
};

#endif
