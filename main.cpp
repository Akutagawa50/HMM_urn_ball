#include <iostream>
#include "HMM_urn_ball.h"

HMM_urn_ball hmm(4,7);

int main()
{
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