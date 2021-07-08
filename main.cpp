#include <iostream>
#include <random>
#include "HMM_urn_ball.h"

const int urn_num = 2;
const int ball_num = 2;
HMM_urn_ball hmm(urn_num, ball_num);

int main()
{
    //std::vector<int> pattern(urn_num + 4);
    std::vector<int> pattern = {0, 1, 1, 1};
    std::random_device rnd;

    for (int i = 0; i < pattern.size(); i++)
    {
        //pattern[i] = rnd() % ball_num;
        std::cout << pattern[i] << ", ";
    }
    std::cout << std::endl;

    std::cout << "p=" << hmm.HMMMeasure(pattern) << std::endl;
    
    if (hmm.HMMLearning(pattern,2))
        std::cout << std::endl
                  << "prob is " << hmm.HMMMeasure(pattern) << std::endl;
    else
        std::cout << "error" << std::endl;
    
    std::vector<std::vector<double>> ball = hmm.GetBallMatrix();
    for (int i = 0; i < hmm.GetUrnNum(); i++)
    {
        for (int j = 0; j < hmm.GetBallNum(); j++)
        {
            std::cout << ball[i][j] << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}