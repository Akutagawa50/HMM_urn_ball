#ifndef HMM_URN_BALL
#define HMM_URN_BALL

#include <algorithm>
#include "Urn.h"

class HMM_urn_ball
{
public:
    HMM_urn_ball(int urn_n, int ball_n);
    ~HMM_urn_ball();
    std::vector<std::vector<double>> GetBallMatrix();
    int GetUrnNum();                                                     //ツボの数を返す
    int GetBallNum();                                                    //ボールの種類を返す
    bool SetLearnPattern(std::vector<int> pattern, int cl_max, int err); //学習の設定
    bool StartLearning();                                                //学習を開始する
    float GetProbability(std::vector<int>);                              //評価を行う関数

private:
    std::vector<Urn> urn;
    std::vector<int> learn_pattern;
    int urn_num;  //ツボの数
    int ball_num; //ボールの種類
    int calc_max; //演算回数
    double error; //誤差
};

#endif
