#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H
#include <QThread>
#include <QTime>
#include <QImage>
#include <vector>
#include <fstream>
#include <iostream>
#include <QMutex>
#include <QDir>
#include <string>
#include <QMessageBox>
#include "opencv.hpp"
#include "svm.h"
#include "imgP_bf1.h"


using namespace std;


struct Circle{
    Circle(double _r, cv::Point _c):r(_r), center(_c){}
    double r;
    cv::Point center;
};

void cvtCvImg2QImg(const cv::Mat &img, QImage &qimg);

class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(const QStringList video_names, QObject *parent = 0);
    explicit VideoThread(int cam_num, QObject *parent = 0);
    ~VideoThread();  
    void setRecord(bool r){isRecord = r;}
    void setRecord_r (bool r) {isRecord_r = r;}
    void setStopped(){stopped = true;}
    void setPause(bool p){isPause = p;}
    void setDisplay(bool d){isDisplay = d;}
    void setSaveImage(bool s){isSaveImage = s;}
    void setHoughCircleParam(const double dp, const double misdist, const double param1,
                             const double param2, const double minR, const double maxR);
    void setThreshold(const int threshold);
    void setROI(const int ROI_upper, const int ROI_lower);
    //
    void setOffsetForDetect(const int Doffset, const int Eoffset);
    void storeAllPATH(vector<vector<bee_path> > path, const int row, const int col, const int toatlFrame);
    //
    bool isCAMOpened(){return isReady;}


    QDir root;


protected:
    void run();
signals:
    void sendNewImage(QImage img);
    void sendNewImage1(QImage img);
    void sendNewImage2(QImage img);
    void sendTagMes (int tag_num, bool dir_in);
    void sendMes(QString msg);
    void sendFinish(QString filename);
public slots:
private:
    void initial();
    void getHoughCircle(const cv::Mat &img, std::vector<Circle> &circle);
    void orderPath(Circle pt,const int numL, const int numR, const int frameNumber);
    void storePath();    
    volatile bool stopped;
    bool isPause;
    bool isReady;
    bool isRecord, isRecord_r, isRecord_save, isRecord_save_r;
    bool isDisplay, isSaveImage;
    bool path_flag;
    bool path_flag1;

    cv::VideoCapture cap;
    cv::VideoWriter writer, writer_r;
    std::vector<svm_model*> G_model_left,G_model_right;

    int TAG_NUM;
    int isLineOpen;

    // for detect offset
    int deOffset;
    // for tag extract offset
    int exOffset;

    // for hough circle
    double hc_dp;
    double hc_misDist;
    double hc_param1;
    double hc_param2;
    int hc_minRadius;
    int hc_maxRadius;

    // for threshold
    int td_threshold;

    //record path
    vector<vector<struct bee_path> > whole_path;
    vector<vector<struct bee_path> > storage_path;
    vector<struct bee_path> inner_path;
    vector<int> path_counter;
    vector<int> temp_length;

    //record video file    
    QFileInfo videoInfo;
    QStringList vid_names;

    // for boundary
    int boundary_u, boundary_l;

    int x_coordinate, y_coordinate;

    std::ofstream outFile;

    std::vector<std::string> img_files;
    int img_index;

    //count fps
    int frame_counter;
    double fperiod;
    QTime frame_time;

    //multi_camera
    QMutex mutex;
    //volatile bool stopped;
    int camera_num;
    int width, height;
    std::vector<int> cam_prior;
    std::vector<cv::Mat> img_vec;
    std::vector<QImage> qimg_vec;


};

#endif // VIDEOTHREAD_H
