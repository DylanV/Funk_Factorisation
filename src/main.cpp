#include <iostream>
#include "ratings.h"

using namespace std;

int main() {
    cout << "Loading movielens data:" << endl;
//    Ratings ratings("D:\\Dev\\cpp\\datasets\\ratings.dat", ':');
    Ratings ratings("D:\\Dev\\cpp\\datasets\\ml20m\\ratings.dat", ',');
    cout << "Adjusting the ratings" << endl;
    ratings.AdjustRatings();
    cout << "Splitting out 10% of ratings into test set" << endl;
    ratings.SplitTestTrain(0.1);
    return 0;
}