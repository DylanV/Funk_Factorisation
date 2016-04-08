//
// Created by Dylan on 06/04/2016.
//

#ifndef HELLOWORLD_RATINGS_H
#define HELLOWORLD_RATINGS_H

#include <vector>
#include <string>
#include <map>
#include <set>

struct rating {
    int ratingId;
    int userId;
    int itemId;
    double value;

    bool operator==(const rating& comp){
        return ratingId == comp.ratingId;
    }
};

class Ratings {

    private:
        // Variables
        static std::vector<std::string> split(const std::string &str, char delim);

        // Functions
        void RemapRatingIds(void);
        void CalculateStats(void);


    public:
        //Constructor
        Ratings(std::string fileName, char delim);

        // Variables
        bool ratingsLoaded = false;

        std::vector<rating> ratings;
        std::vector<rating> testRatings;

        std::set<int> userIds;
        std::set<int> itemIds;

        int numUsers;
        int numItems;
        double globalMean;

        std::vector<double> itemMeans;
        std::vector<double> userOffsets;

        // Functions
        void LoadRatings(std::string file_path, char delim);
        void AdjustRatings(void);
        void SplitTestTrain(double ratio);
};


#endif //HELLOWORLD_RATINGS_H
