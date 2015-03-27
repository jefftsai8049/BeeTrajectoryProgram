#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <QDir>
#include <ctime>
#include <algorithm>
#include <string>
#include "opencv.hpp"
#include "videothread.h"
#include "dataanalysis.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
       void Recslot();
       void Vidslot();
    
private slots:

    void on_loadVideo_clicked();

    void on_runVideo_clicked();

    void on_update_img(QImage qimg);

    void on_update_img1(QImage qimg);

    void on_update_img2(QImage qimg);

    void on_pushButton_hcChange_clicked();    

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_actionCap1_triggered();

    void on_actionCap2_triggered();    

    void on_RawDataRecord_clicked();

    void on_RawDataRecordStop_clicked();

    void on_AnalyzeData_clicked();

    void show_file_finish(QString filename);

    void on_CombineData_clicked();

    void on_Data_Divide_clicked();

    void on_individual_bee_info_clicked();


private:
    Ui::MainWindow *ui;
    VideoThread *multiVi;
    VideoThread *vidThread;
    cv::VideoWriter raw_writer;
    QTimer recTime, vidTime;
    QTime ftime;
    cv::Mat ori_img0, ori_img1, big_img;
    cv::VideoCapture cap0, cap1,cap2;
    cv::VideoWriter writer0, writer1;
    QStringList videofiles;
    fstream file_time;
    QString timeData;
    bool vidFlag;
    int lastTime;
    int nowTime;
    bool videofile_flag;


};

#endif // MAINWINDOW_H
