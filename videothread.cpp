#include "videothread.h"
#include <QDebug>
#include <algorithm>


void cvtCvImg2QImg(const cv::Mat &img, QImage &qimg)
{
    if(qimg.width() != img.cols || qimg.height() != img.rows)
    {
        qimg = QImage(img.cols, img.rows, QImage::Format_RGB888);
    }
    cv::Mat img2;
    if(img.channels() == 3)
        img2 = img;
    else
        cv::cvtColor(img, img2, CV_GRAY2BGR);

    for(int y = 0; y < qimg.height(); ++y)
    {
        memcpy(qimg.scanLine(y), img2.data + img2.cols*y*3, img2.cols*3);
    }
}

VideoThread::VideoThread(const QStringList video_names, QObject *parent) : // video source
    QThread(parent)
{
    initial();
    vid_names = video_names;
    if(video_names.size() > 0)
    {
        isReady = true;
    }
}

VideoThread::VideoThread(int cam_num, QObject *parent) : // camera source
    QThread(parent)
{
    initial();
    cap.open(cam_num);
    isReady = cap.isOpened();
    if (isReady)
    {
        qDebug()<<"Cam Num:" << cam_num;
    }
}

VideoThread::~VideoThread()
{
    storeAllPATH(whole_path,1200,3200, frame_counter);
    //Destroy svm models
    for(int i=0;i<TAG_NUM;i++)
    {
        svm_destroy_model(G_model_left[i]);
        svm_destroy_model(G_model_right[i]);
    }
    stopped = true;
    wait();
}

void VideoThread::setHoughCircleParam(const double dp, const double misdist, const double param1, const double
                                      param2, const double minR, const double maxR)
{
    hc_dp = dp;
    hc_misDist = misdist;
    hc_param1 = param1;
    hc_param2 = param2;
    hc_minRadius = minR;
    hc_maxRadius = maxR;
}

void VideoThread::setThreshold(const int threshold)
{
    td_threshold = threshold;
}

void VideoThread::setROI(const int ROI_upper, const int ROI_lower)
{
    boundary_u = ROI_upper;
    boundary_l = ROI_lower;
}

void VideoThread::setOffsetForDetect(const int Doffset, const int Eoffset)
{
    deOffset = Doffset;
    exOffset = Eoffset;
}

void VideoThread::initial()
{
    stopped = false;
    isReady = false;
    isDisplay = false;
    isSaveImage = false;
    path_flag = true;
    path_flag1 = true;

    TAG_NUM = 26;

    // hough circle parameters
    hc_dp = 4;
    hc_misDist = 20;
    hc_param1 = 80;
    hc_param2 = 100;
    hc_minRadius = 10;
    hc_maxRadius = 25;

    td_threshold = 50;

    // FOV boundary
    boundary_u = 0;
    boundary_l = 1200;

    // load svmModel
    std::string dirL = "modelAZ_left/";
    std::string dirR = "modelAZ_right/";
    for(int i=0;i<TAG_NUM;i++)
    {
        QString TagN = QString::number(i);
        struct svm_model *model_l,*model_r;

        model_l = svm_load_model( ( dirL+TagN.toStdString()+".model" ).c_str() );
        model_r = svm_load_model( ( dirR+TagN.toStdString()+".model" ).c_str() );
        G_model_left.push_back(model_l);
        G_model_right.push_back(model_r);
    }

    if(G_model_left.size() != 26 || G_model_right.size() != 26)
    {
        QMessageBox::information(0,0, "fail to load svm models!");
    }
}

void VideoThread::getHoughCircle(const cv::Mat &img, vector<Circle> &circle)
{
    cv::Mat img_h;
    if(img.channels() != 1)
    {
        cv::cvtColor(img, img_h, CV_BGR2GRAY);
    }
    else
    {
        img_h = img;
    }

    cv::GaussianBlur(img_h.clone(), img_h, cv::Size(5, 5),0);

    cv::imwrite("GaussianBlur.png", img_h);

    vector<cv::Vec3f> _cir;
    cv::HoughCircles(img_h, _cir, 3, hc_dp, hc_misDist, hc_param1, hc_param2, hc_minRadius, hc_maxRadius);
    circle.clear();
    circle.reserve(_cir.size());

    for(size_t i = 0; i < _cir.size(); ++i)
    {
        cv::Vec3i c = _cir[i];
        if(c[0]-c[2] < 0 || c[0]+c[2] >= img.cols || c[1]-c[2] < 0 || c[1]+c[2] >= img.rows)
        {
            continue;
        }

        circle.push_back(Circle(c[2], cv::Point(c[0], c[1])));
    }
}

void VideoThread::orderPath(Circle pt, const int numL, const int numR, const int frameNumber) //order honeybees trajectory
{
    struct bee_path num_p;
    double tagX = pt.center.x;
    double tagY = pt.center.y;

    if(numL!=-1&&numR!=-1)
    {
        bool number_flag = true;
        int num=numL*TAG_NUM+numR;
        if(path_flag)
        {
            num_p.color[0]=(rand()%200)+50;
            num_p.color[1]=(rand()%200)+50;
            num_p.color[2]=(rand()%200)+50;
            num_p.number = num;
            num_p.frameNum = frameNumber;
            num_p.dist = 0;
            num_p.position = pt.center;
            inner_path.push_back(num_p);
            whole_path.push_back(inner_path);
            path_counter.push_back(0);
            inner_path.clear();
            path_flag = false;
        }
        else
        {
            int minP;
            double minDistance = 1000000000000;
            for(int j = 0; j < whole_path.size(); j++)
            {
                double distance = sqrt( pow((double)(whole_path[j].back().position.x-tagX),2)+pow((double)(whole_path[j].back().position.y-tagY),2) );

                if(distance < minDistance)
                {
                    minDistance = distance;
                    minP = j;
                }

                if(num == whole_path[j].back().number && distance < 50 ) //號碼相同且距離小於閥值
                {
                    number_flag = false;
                    num_p.color[0] = whole_path[j][0].color[0];
                    num_p.color[1] = whole_path[j][0].color[1];
                    num_p.color[2] = whole_path[j][0].color[2];
                    num_p.number = num;
                    num_p.frameNum = frameNumber;
                    num_p.position = pt.center;
                    whole_path[j].push_back(num_p);
                }

            }
            if(number_flag)
            {
                if(minDistance < 50 )  //號碼不同但距離小於閥值
                {
                    num_p.color[0] = whole_path[minP][0].color[0];
                    num_p.color[1] = whole_path[minP][0].color[1];
                    num_p.color[2] = whole_path[minP][0].color[2];
                    num_p.number = num;
                    num_p.frameNum = frameNumber;
                    num_p.position = pt.center;
                    whole_path[minP].push_back(num_p);
                }
                else// 產生新的軌跡矩陣
                {
                    num_p.color[0]=(rand()%200)+50;
                    num_p.color[1]=(rand()%200)+50;
                    num_p.color[2]=(rand()%200)+50;
                    num_p.number = num;
                    num_p.frameNum = frameNumber;
                    num_p.dist = 0;
                    num_p.position = pt.center;
                    inner_path.push_back(num_p);
                    whole_path.push_back(inner_path);
                    path_counter.push_back(0);
                    inner_path.clear();
                }
            }
        }
    }
    else // 使用最小距離追蹤軌跡
    {
        int minP;
        double minDistance = 100000000000;
        for(int j=0; j<whole_path.size(); j++)
        {
            double distance = sqrt(pow((double)(whole_path[j].back().position.x-tagX),2) +pow((double)(whole_path[j].back().position.y-tagY),2));
            if(distance < minDistance)
            {
                minDistance = distance;
                minP = j;
            }
        }
        if(minDistance < 50) //距離小於閥值
        {
            //qDebug() << "failButMinDist";
            num_p.color[0] = whole_path[minP][0].color[0];
            num_p.color[1] = whole_path[minP][0].color[1];
            num_p.color[2] = whole_path[minP][0].color[2];
            num_p.number = whole_path[minP].back().number;
            num_p.frameNum = frameNumber;
            num_p.position = pt.center;
            whole_path[minP].push_back(num_p);
        }
        else //距離大於閥值，故產生新的軌跡陣列
        {
            //qDebug() << "fail";
            num_p.color[0]=(rand()%200)+50;
            num_p.color[1]=(rand()%200)+50;
            num_p.color[2]=(rand()%200)+50;
            num_p.number = -1;
            num_p.frameNum = frameNumber;
            num_p.dist = 0;
            num_p.position = pt.center;
            inner_path.push_back(num_p);
            whole_path.push_back(inner_path);
            path_counter.push_back(0);
            inner_path.clear();
        }
    }

}

void VideoThread::storePath() //store path
{
    if(path_flag1)
    {
        for(int i = 0; i < whole_path.size(); i++)
            temp_length.push_back(whole_path[i].size());

        path_flag1 = false;
    }
    else
    {
        for(int i = 0; i < whole_path.size(); i++)
        {
            if(temp_length[i] == whole_path[i].size())
            {
                path_counter[i]++;
            }
            else
                path_counter[i] = 0;

            if(path_counter[i] > 10)
            {
                storage_path.push_back(whole_path[i]);
                whole_path.erase(whole_path.begin()+i);
                path_counter.erase(path_counter.begin()+i);
            }
        }

        temp_length.clear();

        for(int i = 0; i < whole_path.size(); i++)
        {
            temp_length.push_back(whole_path[i].size());

        }
    }
}

void VideoThread::storeAllPATH(vector<vector<bee_path> > path, const int row, const int col, const int totalFrame) //store all path
{
    for(int i = 0; i < path.size(); i++)
        storage_path.push_back(path[i]);

    vector<vector<bee_path> > new_path;

    cv::Mat pathImg;
    pathImg.create(row,col,CV_8UC3);

    for(int i = 0; i < totalFrame; i++)
    {
        for(int j = 0; j < storage_path.size(); j++)
        {
            if( storage_path[j][0].frameNum == i)
                new_path.push_back(storage_path[j]);
        }
    }

    for(int i = 0; i < storage_path.size(); i++)
    {
        int R = storage_path[i][0].color[0];
        int G = storage_path[i][0].color[1];
        int B = storage_path[i][0].color[2];
        if(storage_path[i].size() > 2)
        {
            for(int j = 0; j < storage_path[i].size()-1; j++)
            {
                cv::line(pathImg,storage_path[i][j].position,storage_path[i][j+1].position,CV_RGB(B,G,R),2);
             }
        }
    }

    cv::resize(pathImg,pathImg,cv::Size(1600,600));
    cv::imwrite("ALL_PATH.png",pathImg);
    cv::imshow("ALL PATH",pathImg);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

//main function
void VideoThread::run()
{
    cv::Mat frame,img, img_original, img_thre;
    QImage qimg;
    frame_counter = 0;

    const int DELAY = 50; // ms
    double mask_r = deOffset;

    QTime cur;
    QDateTime reopen;
    cur.start();
    reopen = QDateTime::currentDateTime();

    for(int i = 0; i < vid_names.size(); i++)
    {
        cap.open(vid_names[i].toStdString());
        QFileInfo fileInfo(vid_names[i]);
        videoInfo = fileInfo;
        isReady = cap.isOpened();
        int framecout = cap.get(CV_CAP_PROP_FRAME_COUNT);
        cout <<"framecout = " <<framecout<< endl;

        // store position and ID information
        fstream path_file;
        QString modfileName = videoInfo.absolutePath()+"/"+videoInfo.baseName();
        path_file.open(modfileName.toStdString()+"_PI.txt",ios::out);
        writer.open(modfileName.toStdString()+"_result.avi",CV_FOURCC('X', 'V', 'I', 'D'), 12, cv::Size(3200,1200), 1);

        // caculate fps
        QTime fps;
        fps.start();

        while(!stopped)
        {
            if(cur.elapsed() > DELAY)
            {
                cur.restart();

                cv::waitKey();

                cap >> frame;

                if(!frame.data)
                    break;

                frame_counter++;
                frame.copyTo(img_original);

                img = img_original.rowRange(cv::Range(boundary_u,boundary_l));

                cv::Mat img_gray;
                cv::cvtColor(img,img_gray,CV_BGR2GRAY);

                double angle;
                CvPoint num_p1,num_p2;
                IplImage*tag=cvCreateImage(cvSize((mask_r)*2,(mask_r)*2),8,1);
                IplImage*G_numL=cvCreateImage(cvSize(20,20),8,1);
                IplImage*G_numR=cvCreateImage(cvSize(20,20),8,1);
                CvRect tag_rect; // ROI
                std::vector<Circle> b_c; // hough detection
                b_c.clear();

                getHoughCircle(img_gray.clone(), b_c);

                IplImage ipl_img_smooth= IplImage(img_gray);

                for(int i = 0; i < b_c.size(); ++i)
                {

                    Circle &c = b_c[i];
                    int cx = c.center.x;
                    int cy = c.center.y;
                    if(cx-mask_r >= 0 && cy-mask_r >= 0 && cx+mask_r < ipl_img_smooth.width && cy+mask_r < ipl_img_smooth.height)
                    {
                        tag_rect=cvRect(cx-mask_r,cy-mask_r,2*mask_r,2*mask_r);

                        cv::circle(img_original, cv::Point(cx,cy),c.r,CV_RGB(0,0,255),3,1);

                        if(mask_r > 5)
                            vexGetImage(&ipl_img_smooth,tag,tag_rect,mask_r-1); //r-1
                        else
                            vexGetImage(&ipl_img_smooth,tag,tag_rect,mask_r-2);


                        cvEqualizeHist(tag,tag);

                        cvSaveImage("0Equalize.jpg", tag);

                        cvThreshold(tag,tag,td_threshold,255,1);

                        cvSaveImage("1Threshold.jpg", tag);

                        vexTagAngle(tag,angle,num_p1,num_p2);

                        vexGetImage(&ipl_img_smooth,tag,tag_rect,0);

                        cvEqualizeHist(tag,tag);

                        cvSaveImage("2ScecondGet.jpg", tag);

                        vexGoodImage(tag,G_numL,G_numR,angle,num_p1,num_p2,exOffset);

                        cvSaveImage("G_numL.jpg",G_numL);

                        cvSaveImage("G_numR.jpg",G_numR);

                        // predict tag
                        int numL = vexPredict(G_numL,G_model_left);
                        int numR = vexPredict(G_numR,G_model_right);

                        // write  tag data
                        int bee_id = numL*TAG_NUM+numR;

                        if(numL == -1||numR == -1)
                            bee_id = -1;

                        path_file << " " << bee_id;

                        // number transformation;
                        if(numL != -1&&numR != -1)
                        {
                            char c_num[3];
                            if(numL==0)
                                c_num[0]=(char)(65);
                            else
                                c_num[0]=(char)(65+numL);

                            if(numR==0)
                                c_num[1]=(char)(65);
                            else
                                c_num[1]=(char)(65+numR);

                            c_num[2]='\0';

                            cv::putText(img_original,c_num,cv::Point(cx-c.r,cy+60),3,1,CV_RGB(0,255,0), 2, CV_AA);                            
                        }
                        orderPath(c, numL, numR, frame_counter); //輸入標籤位置、左編號、右編號、frame編號
                    }
                }

                if(b_c.size() > 0)
                {
                    path_file << "\n";
                    for(size_t i = 0; i < b_c.size(); ++i)
                    {
                        Circle &c = b_c[i];
                        int cx = c.center.x;
                        int cy = c.center.y;
                        if(cx-mask_r>=0 && cy-mask_r>=0 && cx+mask_r<ipl_img_smooth.width && cy+mask_r<ipl_img_smooth.height)
                            path_file << " " << c.center.x << " " << c.center.y;
                    }
                    path_file << "\n";
                }
                else
                {
                    path_file << " " << -2 << endl;
                    path_file << " " << -2 << " " << -2 << endl;
                }

                cv::line(img_original,cv::Point(0,boundary_u),cv::Point(img_original.cols,boundary_u),CV_RGB(0, 255, 0),5);
                cv::line(img_original,cv::Point(0,boundary_l),cv::Point(img_original.cols,boundary_l),CV_RGB(0, 255, 0),5);


                if (isDisplay)
                {
                    //store path
                    storePath();

                    //draw honeybees trajectory
                    for(int i = 0; i < whole_path.size();i++)
                    {
                        int R = whole_path[i][0].color[0];
                        int G = whole_path[i][0].color[1];
                        int B = whole_path[i][0].color[2];
                        if(whole_path[i].size() > 2)
                        {
                            for(int j = 0; j < whole_path[i].size()-1; j++)
                                cv::line(img_original,whole_path[i][j].position,whole_path[i][j+1].position,CV_RGB(R,G,B),2);
                        }
                    }

                    cv::imwrite("result_img.png",img_original);
                    writer << img_original;
                    cv::Mat src_temp;
                    cv::pyrDown(img_original.clone(),src_temp);

                    cvtCvImg2QImg(img_original, qimg);

                    emit sendNewImage(qimg);
                }

                int procedure_time = fps.elapsed();

                qDebug() << "fps = " << 1000.0/procedure_time;

                fps.restart();
                img_original.release();
            }
            else
                msleep(10);
        }

        frame_counter = 0;

        path_file.close();
        cap.release();
        emit sendFinish(videoInfo.baseName());
    }
    QString file_finish = "All file";
    emit sendFinish(file_finish);
}
