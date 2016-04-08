//
// Created by Dylan Verrezen on 2016/04/08.
//

#ifndef FUNK_FACTORISATION_FACTORISER_H
#define FUNK_FACTORISATION_FACTORISER_H

#include <vector>
#include "ratings.h"

struct residual{
    double curErr = 0;
    double prevErr = 0;
    double ratingValue = 0;
};

class factoriser {
    private:
        // Constants as recommended by Simon Funk
        const int numFeatures = 50;
        const int maxEpochs = 100;
        const int minEpoch = 0;
        const double learningRate = 0.001;
        const double regulariser = 0.02;
        const double featureInit = 0.1;

        void train(const Ratings& ratings);

    public:

        // Constructor
        factoriser(const Ratings& ratings);

        // Variables
        std::vector<std::vector<double>> userFeatures;
        std::vector<std::vector<double>> itemFeatures;

        // Functions
        double predict(int userId, int itemId);
        std::vector<double> predictAll(std::vector<rating>& ratings);
        double clip(double value, double floor, double ceiling);

};


#endif //FUNK_FACTORISATION_FACTORISER_H
