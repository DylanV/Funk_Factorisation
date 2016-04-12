#include <iostream>
#include "ratings.h"
#include "factoriser.h"
#include <math.h>

#include <windows.h>

using namespace std;

int main() {

    cout << "Loading movielens data:" << endl;
    //osx
//    Ratings ratings("/Users/dylan/Dev/masters/Funk_Factorisation/datasets/ml1m/ratings.dat", ':');

    //Windows
//    Ratings ratings("../datasets/ml20m/ratings.dat", ',');
    Ratings ratings("../datasets/ml1m/ratings.dat", ':');


    cout << "Adjusting the ratings" << endl;
    ratings.AdjustRatings();
    cout << "Splitting out 10% of ratings into test set" << endl;
    ratings.SplitTestTrain(0.1);
    factoriser funk(ratings);
    cout << "Predicting ratings for test set" << endl;
    vector<double> predicted = funk.predictAll(ratings.testRatings);
    double total = 0;
    double total2 = 0;

    for(int i=0; i<ratings.testRatings.size(); i++){
        int uid = ratings.testRatings[i].userId;
        int iid = ratings.testRatings[i].itemId;

        double base = ratings.userOffsets[uid] + ratings.itemMeans[iid];
        double pred = funk.clip(predicted[i]+base,1,5);
        double actual = ratings.testRatings[i].value + base;

        total += fabs(pred - actual)*fabs(pred - actual);
        total2 += fabs(base - actual)*fabs(pred - actual);

        if(i%1000 == 0){
            cout << pred << " | " << actual << " | " << base << endl;
        }
    }
    cout << "predicted rmse: " << sqrt(total/ratings.testRatings.size()) << endl;
    cout << "baseline rmse: " << sqrt(total2/ratings.testRatings.size()) << endl;

    return 0;
}