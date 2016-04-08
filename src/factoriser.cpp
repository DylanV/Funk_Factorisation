//
// Created by Dylan Verrezen on 2016/04/08.
//

#include <iostream>
#include "factoriser.h"
#include <map>
#include <math.h>

using namespace std;

factoriser::factoriser(const Ratings& ratings)
{
    train(ratings);
}

void factoriser::train(const Ratings& ratings)
{
    int num_users = ratings.numUsers;
    int num_items = ratings.numItems;

    vector<vector<double>> user_features (num_users, vector<double>(numFeatures, featureInit));
    vector<vector<double>> item_features (num_items, vector<double>(numFeatures, featureInit));

    map<int, residual> residuals;

    for(const rating& rat: ratings.ratings){
        residual res;
        res.ratingValue = rat.value;
        residuals[rat.ratingId] = res;
    }


    for(int feature_rank=0; feature_rank<numFeatures; feature_rank++){
        cout << "rank: " << feature_rank;

        vector<double> error_hist(3);
        error_hist[1] = MAXFLOAT;
        error_hist[2] = MAXFLOAT;

        for(int epoch=0; epoch<maxEpochs; epoch++){
            for(const rating& rating : ratings.ratings){
                int user_id = rating.userId;
                int item_id = rating.itemId;

                double user_feat = user_features[user_id][feature_rank];
                double item_feat = item_features[item_id][feature_rank];

                residual& res = residuals[rating.ratingId];
                res.curErr = res.ratingValue - user_feat*item_feat;

                error_hist[0] += fabs(res.curErr);

                user_features[user_id][feature_rank] += learningRate * (res.curErr*item_feat - regulariser*user_feat);
                item_features[item_id][feature_rank] += learningRate * (res.curErr*user_feat - regulariser*item_feat);
            }

            if(epoch > minEpoch && error_hist[0] < error_hist[1] && error_hist[1] > error_hist[2]){
                cout << " | " << epoch << " | " << error_hist[0] / ratings.ratings.size();
                break;
            }
            error_hist[2] = error_hist[1];
            error_hist[1] = error_hist[0];
            error_hist[0] = 0;

        }
        for (auto& res: residuals){
            res.second.ratingValue = res.second.curErr;
        }
        cout << endl;
    }

    userFeatures = user_features;
    itemFeatures = item_features;

}

double factoriser::predict(int userId, int itemId) {
    double prediction = 0;
    vector<double> user_feat = userFeatures[userId];
    vector<double> item_feat = itemFeatures[itemId];

    for(int index=0; index<numFeatures; index++){
        prediction += user_feat[index]*item_feat[index];
    }

    return prediction;
}

vector<double> factoriser::predictAll(vector<rating>& ratings) {
    vector<double> predicted(ratings.size());
    int count = 0;
    for(const rating& test: ratings){
        predicted[count] = predict(test.userId, test.itemId);
        count++;
    }
    cout <<  count << " predictions made." << endl;
    return predicted;
}

double factoriser::clip(double value, double floor=0, double ceiling=5) {
    if(value < floor){
        return floor;
    }else if(value > ceiling){
        return ceiling;
    }else {
        return value;
    }
}



