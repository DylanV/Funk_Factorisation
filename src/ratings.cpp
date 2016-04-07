//
// Created by Dylan on 06/04/2016.
//

#include "ratings.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <unordered_map>
#include <random>

using namespace std;

Ratings::Ratings(string fileName, char delim){
    LoadRatings(fileName, delim);
}

vector<string> Ratings::split(const string &str, char delim)
{
    vector<string> items;
    stringstream ss(str);
    string item;

    while (std::getline(ss, item, delim)) {
        if(!item.empty()){ items.push_back(item); }
    }

    return items;
}

void Ratings::RemapRatingIds()
{
    unordered_map<int, int> user_ids_map;
    unordered_map<int, int> item_ids_map;

    int newId = 0;
    for(set<int>::iterator iter = userIds.begin(); iter != userIds.end(); ++iter){
        int userId = *iter;
        user_ids_map[userId] = newId;
        newId++;
    }

    newId = 0;
    for(set<int>::iterator iter = itemIds.begin(); iter != itemIds.end(); ++iter){
        int itemId = *iter;
        item_ids_map[itemId] = newId;
        newId++;
    }

    for(rating& rating : ratings){
        int new_user_id = user_ids_map[rating.userId];
        int new_item_id = item_ids_map[rating.itemId];
        rating.userId = new_user_id;
        rating.itemId = new_item_id;
    }

}

void Ratings::CalculateUserStats()
{
    // Get user total rating sum and rating counts
    vector<double> user_totals(userIds.size(), 0);
    vector<int> user_counts(userIds.size(), 0);

    for (const rating& rating : ratings){
        int userId = rating.userId;
        double value = rating.value;
        user_totals[userId] += value;
        user_counts[userId]++;
    }

    // Calculate user means
    vector<double> means(userIds.size(), 0);
    for(int i=0; i<userIds.size(); i++){
        means[i] = user_totals[i]/user_counts[i];
    }
    userMeans = means; // set the class member

    // Get sum of the distances from the user means for each user rating
    vector<double> dists(userIds.size(), 0);
    for (const rating& rating : ratings){
        int userId = rating.userId;
        double value = rating.value;
        dists[userId] += fabs(value-means[userId]);
    }

    // Calculate ethe user standard deviations
    vector<double> std_devs(userIds.size(), 0);
    for(int i=0; i<userIds.size(); i++){
        std_devs[i] = dists[i]/user_counts[i];
    }
    userStdDevs = std_devs; // set the class member
}

void Ratings::LoadRatings(string file_path, char delim = ':')
{
    // Open the ratings file
    ifstream fs(file_path);
    if(fs.is_open()){
        string line;
        int count = 0;

        while(!fs.eof()){

            getline(fs, line);
            vector<string> items = split(line, delim);

            if(items.size() >= 3){

                int userId = stoi(items[0]);
                int itemId = stoi(items[1]);
                double value = stod(items[2]);

                userIds.insert(userId);
                itemIds.insert(itemId);

                rating cur_rating;
                cur_rating.ratingId = count;
                cur_rating.userId = userId;
                cur_rating.itemId = itemId;
                cur_rating.value = value;

                ratings.push_back(cur_rating);

                count++;
            }
        }

        RemapRatingIds(); // Remap the user and item ids to a continuous space
        CalculateUserStats(); // Calculate the user means and standard deviations

        cout << count << " ratings loaded from file: " << file_path << endl;

        ratingsLoaded = true;

    } else {
        cout << "Failed to open file." << endl;
    }

    fs.close();
}

void Ratings::SplitTestTrain(double ratio)
{
    double test_set_size = floor(ratings.size() * ratio);
    set<int> test_set_rating_ids;

    default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,ratings.size());

    while(test_set_rating_ids.size() <= test_set_size){
        test_set_rating_ids.insert(distribution(generator));
    }

    vector<rating> new_ratings;
    for(const rating& rating : ratings){
        if(test_set_rating_ids.find(rating.ratingId) != test_set_rating_ids.end()){
            testRatings.push_back(rating);
        }else {
            new_ratings.push_back(rating);
        }
    }
    ratings = new_ratings;

    cout << testRatings.size() << " test ratings have been removed from the ratings and "
        << ratings.size() << " ratings remain" << endl;
}

void Ratings::AdjustRatings()
{
    for(rating& rating: ratings){
        int user_id = rating.userId;
        double user_mean = userMeans[user_id];
        double user_std_dev = userStdDevs[user_id];

        rating.value = (rating.value - user_mean)/user_std_dev;
    }

    cout << "Ratings have been normalised with user means and standard deviations" << endl;
}
