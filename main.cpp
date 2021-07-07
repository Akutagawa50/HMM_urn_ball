#include <iostream>
#include <random>
#include "HMM_urn_ball.h"

const int urn_num = 4;
const int ball_num = 8;
HMM_urn_ball hmm(urn_num, ball_num);

int main()
{
    /*
    std::vector<std::vector<double>> ball = hmm.GetBallMatrix();
    for (int i = 0; i < hmm.GetUrnNum(); i++)
    {
        for (int j = 0; j < hmm.GetBallNum(); j++)
        {
            std::cout << ball[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    */
    std::vector<int> pattern(urn_num + 3);
    std::random_device rnd;
    for (int i = 0; i < pattern.size(); i++)
    {
        pattern[i] = rnd() % ball_num;
    }

    for (int i = 0; i < pattern.size(); i++)
    {
        std::cout << pattern[i] << ", ";
    }
    std::cout << std::endl
              << "prob is " << hmm.GetProbability(pattern) << std::endl;

    return 0;
}