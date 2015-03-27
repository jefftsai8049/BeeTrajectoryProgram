#ifndef IMGP_BF1_H
#define IMGP_BF1_H
#include <vector>
#include <String>
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include "opencv.hpp"
#include "svm.h"
#include <Windows.h>

using namespace std;

struct blob_node
{
   blob_node *linker;
   int row,column;
   int data;
   int dir;
};
struct blob
{
    int height;
    int width;
    byte **data;
};
enum {Inside,Outside};
struct bee_state
{
    int state;
    int flag;
    int timer;

    CvPoint first;
    CvPoint last;
};
struct bee_info
{
    int number;
    int state;
    std::string time;
};
struct timeHMS
{
    int hour;
    int min;
    int sec;
};
struct bee_path
{    
    int number;
    int color[3];
    int dist;
    bool T_flag;
    int frameNum;
    //bool pathflag;
    cv::Point position;
    timeHMS timedata;

};
void vexGetImage(IplImage*src,IplImage*dst,CvRect img_rect,int flag);
void vexTagAngle(IplImage*img_in,double &angle,CvPoint &point_1,CvPoint &point_2);
void vexGoodImage(IplImage*src,IplImage*dst1,IplImage*dst2,double angle,CvPoint p1,CvPoint p2,int num_size);
void vexReleaseImage(byte **src,int height,int width);
void maxblob2(byte **src,byte **dst,int *max,unsigned int background,int height,int width);
void cvimageToimage(IplImage *src,byte **dst,int height,int width);
IplImage* vexMakeSquareImage(IplImage*src, byte fill_color,int size);
byte** vexCreateImage(int height,int width);
double vexPredict(IplImage*src, std::vector<svm_model*> model);




#endif // IMGP_BF1_H
