#include <iostream>
#include <random>
#include <iomanip>
#include "HMM_urn_ball.h"

const int urn_num = 3;    //壺の数
const int ball_num = 4;   //ボールの数
const int learning = 100; //最大学習回数

HMM_urn_ball hmm(urn_num, ball_num);

int main()
{
    std::vector<int> pattern(urn_num + 4);
    //std::vector<int> pattern = {0, 1, 1, 1, 0, 1, 0, 1};
    std::random_device rnd;

    //データを表示
    std::cout << "pattern: ";
    for (int i = 0; i < pattern.size(); i++)
    {
        pattern[i] = rnd() % ball_num;
        std::cout << pattern[i] << ((i == pattern.size() - 1) ? "\n" : ", ");
    }

    //データの出現確率を表示
    std::cout << "p=" << hmm.HMMMeasure(pattern) << std::endl;

    //学習開始
    if (hmm.HMMLearning(pattern, learning))
    { //結果を出力
        std::cout << std::endl
                  << "After learning, prob is " << hmm.HMMMeasure(pattern) << std::endl;
    }
    else //解析失敗
        std::cout << "error" << std::endl;

    //各壺のボールが出る確率と遷移確率を出力
    std::vector<std::vector<double>> ball = hmm.GetBallMatrix();
    std::vector<std::vector<double>> trans = hmm.GetTransMatrix();
    for (int i = 0; i < hmm.GetUrnNum(); i++)
    {
        std::cout << "壺" << i << ": ";
        for (int j = 0; j < hmm.GetBallNum(); j++)
        {
            std::cout << "ボール" << std::fixed << std::setprecision(3) << j << ": " << ball[i][j] << ", ";
        }
        std::cout << "自己ループ確率: " << trans[i][0] << ", ";
        std::cout << "遷移確率: " << trans[i][1] << std::endl;
    }

    return 0;
}