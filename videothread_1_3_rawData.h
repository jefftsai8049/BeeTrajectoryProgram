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
#include "beerecord.h"
#include "svm.h"
#include "imgP_bf1.h"
#include "videoInput.h"


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
    explicit VideoThread(const int cam_num, const int w, const int h, std::vector<int> _open_prior,
                         QObject *parent = 0);
    explicit VideoThread(QString video_name, QObject *parent = 0);
    explicit VideoThread(int cam_num, QObject *parent = 0);
    explicit VideoThread(const std::vector<std::string> &filename_vec, QObject *parent = 0);

    ~VideoThread();
    enum CAMMode{Video, Image, Test};
    CAMMode getCAMMode(){return cam_mode;}
    void setCAMMode(CAMMode mode){cam_mode = mode;}
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
    void setTimeLimit(const int time_num){ time_limit = time_num;}
    void setFrameLimit(const int frame_num){frame_limit = frame_num;}
    //
    void setOffsetForDetect(const int Doffset, const int Eoffset);
    //
    bool isCAMOpened(){return isReady;}
    void setLineOpen (int i){isLineOpen = i;}
    void setPointCoordinate(const int x, const int y);
    void getGoodCircleTagNum(const cv::Mat &img, const cv::Mat &img_mask, const cv::Mat &img_mask_circle, const Circle &c, int max_r, cv::Mat &img_out, int &tag_num);

    //multi_camera
    bool setCamPrior(const std::vector<int> &prior);
    static int getAllCameraNum();
    void setStopped(bool stop);
    int getCameraNum() const{return camera_num;}
    int getWidth() const {return width;}
    int getHeight() const {return height;}






    QDir root;

    int id;
protected:
    void run();
signals:
    void sendNewImage(QImage img);
    void sendNewImage1(QImage img);
    void sendNewImage2(QImage img);
    void sendTagMes (int tag_num, bool dir_in);
    void sendMes(QString msg);
public slots:
private:
    void getHoughCircle(const cv::Mat &img, std::vector<Circle> &circle);
    void TagDetection(const cv::Mat &src, cv::Mat &mask, std::vector<Circle> &circles);
    void TagThreshold(const cv::Mat &src,cv::Mat &dst, const cv::Mat &back);
    //void tdThreshold(const cv::Mat &src, cv::Mat &dst, const int td_threshold);
    void initial();
    volatile bool stopped;
    bool isPause;
    bool isReady;
    bool isRecord, isRecord_r, isRecord_save, isRecord_save_r;
    bool isDisplay, isSaveImage;
    bool path_flag;

    cv::VideoCapture cap;
    cv::VideoCapture cap0;
    cv::VideoCapture cap1;
    cv::VideoWriter writer0, writer1;
    cv::VideoWriter writer, writer_r, writer_bf;
    std::vector<svm_model*> G_model_left,G_model_right;
    //std::vector<std::vector<struct bee_path> > whole_path;
    //std::vector<struct bee_path> inner_path;

    float DDdistance;
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
    //

    // for threshold
    int td_threshold;
    //

    // for boundary
    int boundary_u, boundary_l;

    int x_coordinate, y_coordinate;

    std::ofstream outFile;

    WholeRecord record;

    int frame_limit, time_limit;

    CAMMode cam_mode;
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
    videoInput VI;


};

#endif // VIDEOTHREAD_H
