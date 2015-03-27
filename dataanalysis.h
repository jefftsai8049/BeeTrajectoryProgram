#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include <QTime>
#include <QFileDialog>
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <complex>
#include <algorithm>
#include "imgP_bf1.h"
#include "colormap.hpp"

using namespace std;

struct timeInfo
{
    int goodpath_c;
    double goodpath_dist;
    int long_type;
    double type_ratio;
};

class dataanalysis
{
public:

    void analysePath(vector<vector<int> > allBeeId, vector<vector<cv::Point> > allBeeLoc,
                     vector<timeHMS> allTime, QFileInfo fileInfo, const float pathparameters[], vector<int> inter_NUM);
    void orderPath(const int num,cv::Point pt, const int f_count, timeHMS timeData);
    void storePath();
    void namePath(vector<vector<bee_path> > path, string fpath);
    void sortPath(vector<vector<bee_path> > path, const int fNum, vector<int> inter_num);
    void findOverlap(vector<int> ids, vector<cv::Point> locs, cv::Mat &inter_m);
    void findGoodPath(vector<vector<bee_path> > path);    
    void caculateSD(vector<cv::Point> allpoint, float boundary[]);
    void region_distribution(vector<vector<bee_path> > path);
    void NumberTranslate(const int number, char c_num[]);    
    void findDAdistribution(vector<vector<bee_path> > path);
    void findPathPH(vector<vector<bee_path> > path);
    void findIndividualData(vector<vector<bee_path> > path, vector<int> inter_num);
    void drawSingleNumPath(vector<vector<bee_path> > path);
    void drawSinglePath(vector<bee_path> path, int p_n);
    void orderInteraction(vector<vector<int> > inter_data, vector<int> inter_num);
    void findSpeedDistribution (vector<vector<bee_path> > path);



private:

    // path
    vector<vector<bee_path> > whole_path;
    vector<vector<bee_path> > storage_path;
    vector<vector<bee_path> > reduce_path;
    vector<int> path_counter;
    vector<int> temp_length;
    vector<timeHMS> datatime;
    //interaction
    vector<int> inter_count;
    vector<cv::Point> inter_tag;

    //parameters
    int path_value, miss_threshold,overlap_value,pathLength,pathDist,TAG_NUM;
    float pathtype_ratio;

    int overlap_count;
    int time_count;
    bool path_flag;
    bool path_flag1;

    //store analysed data
    string filepath;

};

#endif // DATAANALYSIS_H
