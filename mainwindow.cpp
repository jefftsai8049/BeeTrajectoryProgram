#include "mainwindow.h"
#include "ui_mainwindow.h"

using std::vector;
using std::string;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vidThread = 0; 
    videofile_flag = 0;
    vidFlag = true;
    connect(&this->recTime,SIGNAL(timeout()),this,SLOT(Recslot()));
    connect(&this->vidTime,SIGNAL(timeout()),this,SLOT(Vidslot()));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(vidThread)
    {
        vidThread->setStopped();
        vidThread->wait();
        delete vidThread;
    }
}

void MainWindow::on_loadVideo_clicked()
{
    videofiles = QFileDialog::getOpenFileNames(this, tr("Open .id File"), "D:/", tr("avi File (*.avi)"));

    if (videofiles.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }
}


void MainWindow::on_runVideo_clicked()
{
     if(vidThread == 0)
    {
        ui->loadVideo->setEnabled(false);
        ui->runVideo->setText("Stop");        

        vidThread = new VideoThread(videofiles);
        vidThread->setRecord(ui->checkBox_Record->isChecked());
        vidThread->root = QDir::currentPath();

        ui->pushButton_hcChange->click();
        connect(vidThread, SIGNAL(sendNewImage(QImage)), this, SLOT(on_update_img(QImage)));
        connect(vidThread, SIGNAL(sendMes(QString)), ui->statusBar, SLOT(showMessage(QString)));
        connect(vidThread, SIGNAL(sendFinish(QString)), this, SLOT(show_file_finish(QString)));
        ui->pushButton_2->click();
        vidThread->setOffsetForDetect(ui->spinBox_offset_for_detect->value(),ui->spinBox_offset_for_extract->value());
        vidThread->setDisplay(ui->checkBox_Display->checkState());
        vidThread->start();
    }
    else
    {
        ui->loadVideo->setEnabled(true);
        ui->runVideo->setText("Run");
        disconnect(vidThread, SIGNAL(sendNewImage(QImage)), this, SLOT(on_update_img(QImage)));
        disconnect(vidThread, SIGNAL(sendMes(QString)), ui->statusBar, SLOT(showMessage(QString)));
        disconnect(vidThread, SIGNAL(sendFinish(QString)), this, SLOT(show_file_finish(QString)));
        vidThread->setStopped();
        vidThread->wait();
        delete vidThread;
        vidThread = 0;
    }
}

void MainWindow::on_update_img(QImage qimg)
{
    ui->label->setPixmap(QPixmap::fromImage(qimg.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio)));
}
void MainWindow::on_update_img1(QImage qimg)
{
    ui->label2->setPixmap(QPixmap::fromImage(qimg.scaled(ui->label2->width(), ui->label2->height(), Qt::KeepAspectRatio)));
}
void MainWindow::on_update_img2(QImage qimg)
{
    ui->label3->setPixmap(QPixmap::fromImage(qimg.scaled(ui->label3->width(), ui->label3->height(), Qt::KeepAspectRatio)));
}

void MainWindow::on_pushButton_hcChange_clicked()
{
    if(vidThread)
    {
        vidThread->setHoughCircleParam(ui->doubleSpinBox_dp->value(), ui->spinBox_minDist->value(),
                                       ui->spinBox_param1->value(), ui->spinBox_param2->value(),
                                       ui->spinBox_minR->value(), ui->spinBox_maxR->value());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(vidThread)
    {
        vidThread->setThreshold(ui->spinBox_TDthreshold->value());
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    ui->listWidget->clear();
}

void MainWindow::on_pushButton_7_clicked()
{
    if(vidThread)
    {
        vidThread->setROI(ui->spinBox_ROI_U->value(),ui->spinBox_ROI_L->value());
    }

}

void MainWindow::on_actionCap1_triggered()
{
    ui->dockWidget->show();
}

void MainWindow::on_actionCap2_triggered()
{
    ui->dockWidget_2->show();
}

void MainWindow::on_RawDataRecord_clicked()
{
    ui->RawDataRecord->setEnabled(false);
    ui->RawDataRecordStop->setEnabled(true);
    if(!cap0.isOpened())
    {
        cap0.open(0);
    }
    if(!cap1.isOpened())
    {
        cap1.open(1);
    }

    big_img.create(3200,1200,CV_8U);
    vidTime.start(1);
}

void MainWindow::Vidslot()
{
    if(vidFlag)
    {
        int sampleRate_vid = ui->spinBox_time_limit->value()*60*1000; // record time
        vidTime.start(sampleRate_vid);
        vidFlag = false;
    }

    recTime.stop();
    file_time.close();

    QDir data_dir = QDir::currentPath()+"\\Raw_video";
    QString date_dir =QDateTime::currentDateTime().toString("MM_dd");

    if (!data_dir.exists(date_dir))
    {
        data_dir.mkdir(date_dir);
    }

    data_dir.cd(date_dir);
    QString file_name = data_dir.absolutePath()+"\\"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    lastTime = QDateTime::currentDateTime().toString("dd").toInt();
    file_time.open(file_name.toStdString()+"_time.txt",ios::out);

    writer0.open(file_name.toStdString()+".avi", CV_FOURCC('X', 'V', 'I', 'D'), 12, cv::Size(3200,1200), 1);

    if(!writer0.isOpened())
    {
        qDebug() << "Can't open video writer";
        return;
    }
    recTime.start(1);
}

void MainWindow::Recslot()
{
    nowTime = QDateTime::currentDateTime().toString("dd").toInt();
    if(nowTime != lastTime)
    {
        file_time.close();

        QDir data_dir = QDir::currentPath()+"\\Raw_video";
        QString date_dir =QDateTime::currentDateTime().toString("MM_dd");

        if (!data_dir.exists(date_dir))
        {
            data_dir.mkdir(date_dir);
        }

        data_dir.cd(date_dir);
        QString file_name = data_dir.absolutePath()+"\\"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        file_time.open(file_name.toStdString()+"_time.txt",ios::out);

        writer0.open(file_name.toStdString()+".avi", CV_FOURCC('X', 'V', 'I', 'D'), 12, cv::Size(3200,1200), 1);

        if(!writer0.isOpened())
        {
            qDebug() << "Can Not Open";
            return;
        }
    }

    timeData = QDateTime::currentDateTime().toString("hh mm ss z");

    cap0 >> ori_img0;
    cap1 >> ori_img1;

    cv::hconcat(ori_img1, ori_img0, big_img);

    writer0 << big_img;

    file_time << timeData.toStdString() << endl;

    big_img.release();

    lastTime = nowTime;   
}


void MainWindow::on_RawDataRecordStop_clicked()
{    
    ui->RawDataRecord->setEnabled(true);
    ui->RawDataRecordStop->setEnabled(false);
    recTime.stop();
    vidTime.stop();
}

void MainWindow::on_AnalyzeData_clicked() //Analyze Data
{
    int minPathValue = ui->Dist_threshold->value();
    int missThreshold = ui->Miss_threshold->value();
    int pathDistanceThreshold = ui->Path_dist_threshold->value();
    int overlapThreshold = ui->Overlap_Threshold->value();
    float pathTypeRatio = ui->Path_type_ratio->value();

    vector<int> inter_num;

    for(int i = 0; i < ui->inter_num->text().size(); i=i+3)
    {
        int L_num = int(ui->inter_num->text().at(i).toLatin1())-65;
        int R_num = int(ui->inter_num->text().at(i+1).toLatin1())-65;
        int IterN = L_num*26+R_num;
        cout << IterN << endl;
        inter_num.push_back(IterN);
    }

    const float pathParameters[5] = {minPathValue, missThreshold, pathDistanceThreshold, overlapThreshold, pathTypeRatio};

    QStringList id_FilePaths = QFileDialog::getOpenFileNames(this, tr("Open .id File"),
                                                             "D:/Research/InBee_Experiement/indoor/firstRound/test",
                                                             tr("txt File (*.txt)"));


    if (id_FilePaths.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }

    foreach(QString id_FilePath, id_FilePaths)
    {
        dataanalysis BeeData;
        string id_infileName = id_FilePath.toStdString();
        QFileInfo fileInfo(id_FilePath);
        string data_line;
        fstream path_infile, time_infile;
        vector<vector<int> > allBeeId;
        vector<vector<cv::Point> > allBeeLoc;
        vector<timeHMS> allTime;
        vector<int> tempBeeId;
        vector<cv::Point> tempBeeLoc;

        int beeId;
        int beeloc_x;
        int beeloc_y;


        char tempName[11];
        for(int i = 0; i < 10; i++)
            tempName[i] = fileInfo.baseName().toStdString().at(i);

        tempName[10] = '\0';

        string time_infileName(tempName);
        time_infileName = fileInfo.absolutePath().toStdString()+"/"+time_infileName+"_time.txt";

        cout << time_infileName << endl;

        path_infile.open(id_infileName.c_str(),ios::in);
        time_infile.open(time_infileName.c_str(),ios::in);

        if(!path_infile)
        {
            QMessageBox::information(0, "Error!", "Open PI file failed");
            return;
        }

        if(!time_infile)
        {
            QMessageBox::information(0, "Error!", "Open time file failed");
            return;
        }


        while( getline(path_infile,data_line))
        {
            stringstream get_data(data_line);

            while(!get_data.eof())
            {
                get_data >> beeId;
                tempBeeId.push_back(beeId);
            }
            allBeeId.push_back(tempBeeId);
            tempBeeId.clear();

            getline(path_infile,data_line);
            stringstream get_data2(data_line);

            while(!get_data2.eof())
            {
                get_data2 >> beeloc_x;
                get_data2 >> beeloc_y;
                tempBeeLoc.push_back(cv::Point(beeloc_x,beeloc_y));
            }
            allBeeLoc.push_back(tempBeeLoc);
            tempBeeLoc.clear();
        }

        // load time information
        while( getline(time_infile,data_line))
        {
            stringstream get_data(data_line);

            int i = 0;
            int hour, min, sec;
            timeHMS timedata;
            while(!get_data.eof())
            {
                int data;
                get_data >> data;


                // time data need to be modified
                if(i == 0) //3
                    timedata.hour = data;

                if(i == 1) //4
                    timedata.min = data;

                if(i == 2) //5
                {
                    timedata.sec = data;
                    allTime.push_back(timedata);
                    //cout << allTime.back().hour<<" "<<allTime.back().min << " " << allTime.back().sec<<endl;
                }


                i++;
            }
        }

        cout <<" id size = " <<allBeeId.size() << "   time size = " << allBeeLoc.size()<< endl;



        if(allBeeId.size() == allTime.size() )
        {
            BeeData.analysePath(allBeeId, allBeeLoc, allTime, fileInfo, pathParameters, inter_num);
        }

    }
    QMessageBox::information(0, 0, "Analyze data successfully!");

}

void MainWindow::show_file_finish(QString filename)
{
    QListWidgetItem *item = new QListWidgetItem;

    QString name = filename+ " done!!!";

    item->setText(name);

    ui->listWidget->addItem(item);
}

void MainWindow::on_CombineData_clicked() //Combine Data
{
    QStringList id_FilePaths = QFileDialog::getOpenFileNames(this, tr("Open .id File"),
                                                             "D:/Research/InBee_Experiement/firstRound",
                                                             tr("txt File (*PI.txt)"));
    if (id_FilePaths.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }

    QFileInfo Info(id_FilePaths.at(0));
    string basename = Info.baseName().toStdString();
    char temp1[11];
    for(int i = 0; i < 10; i++)
    {
        temp1[i] = basename.at(i);
    }
    temp1[10] = '\0';

    string temp2(temp1);
    string filename = Info.absolutePath().toStdString()+"/"+temp2+"_PI.txt";
    cout << filename << endl;
    fstream outfile;
    outfile.open(filename, ios::out);

    int id = 0;

    foreach(QString id_FilePath, id_FilePaths)
    {
        string inFIleName = id_FilePath.toStdString();
        string data_line;
        fstream path_infile;

        path_infile.open(inFIleName.c_str(),ios::in);

        if(!path_infile)
        {
            QMessageBox::information(0, "Error!", "Open file failed");
            return;
        }


        while( getline(path_infile,data_line))
        {
            outfile << data_line;
            outfile << endl;
            id++;
        }
        //        outfile << " " << -2 << endl;
        //        outfile << " " << -2 << " " << -2 << endl;
        //        id=id+2;
    }

    cout << "id = " << id << endl;



    QStringList time_FilePaths = QFileDialog::getOpenFileNames(this, tr("Open .time File"),
                                                               "D:/Research/2013_12_8_inBeeHiveTest/11cm",
                                                               tr("txt File (*time.txt)"));
    if (time_FilePaths.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }

    QFileInfo time_Info(time_FilePaths.at(0));
    string time_basename = time_Info.baseName().toStdString();
    char time1[11];
    for(int i = 0; i < 10; i++)
    {
        time1[i] = time_basename.at(i);
    }
    time1[10] = '\0';

    string time2(time1);
    string timename = Info.absolutePath().toStdString()+"/"+time2+"_time.txt";

    fstream time_outfile;
    time_outfile.open(timename, ios::out);


    int t = 0;
    foreach(QString time_FilePath, time_FilePaths)
    {
        string inFIleName = time_FilePath.toStdString();
        string data_line;
        fstream path_infile;

        path_infile.open(inFIleName.c_str(),ios::in);

        if(!path_infile)
        {
            QMessageBox::information(0, "Error!", "Open file failed");
            return;
        }


        while( getline(path_infile,data_line))
        {
            time_outfile << data_line;
            time_outfile << endl;
            t++;
        }
    }
    cout << "time = " << t*2 << endl;
    QMessageBox::information(0, 0, "Combine data successfully!");
}

void MainWindow::on_Data_Divide_clicked()
{
    vector<int> control_number,treated_number;

    for(int i = 0; i < ui->control_num->text().size(); i++)
    {
        int num = int(ui->control_num->text().at(i).toLatin1())-65;
        num = num*26;
        cout << num << endl;
        control_number.push_back(num);
    }
    cout << endl;

    for(int i = 0; i < ui->treated_num->text().size(); i++)
    {
        int num = int(ui->treated_num->text().at(i).toLatin1())-65;
        num = num*26;
        cout << num << endl;
        treated_number.push_back(num);
    }

    QStringList id_FilePaths = QFileDialog::getOpenFileNames(this, tr("Open .id File"),
                                                             "D:/Research/InBee_Experiement/indoor/firstRound/test",
                                                             tr("txt File (*.txt)"));
    if (id_FilePaths.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }

    foreach(QString id_FilePath, id_FilePaths)
    {
        QFileInfo Info(id_FilePath);
        string basename = Info.baseName().toStdString();
        char temp1[11];
        for(int i = 0; i < 10; i++)
        {
            temp1[i] = basename.at(i);
        }
        temp1[10] = '\0';

        string temp2(temp1);
        string control_filename = Info.absolutePath().toStdString()+"/"+temp2+"control_PI.txt";
        string treated_filename = Info.absolutePath().toStdString()+"/"+temp2+"treated_PI.txt";

        fstream control_outfile, treated_outfile;
        control_outfile.open(control_filename, ios::out);
        treated_outfile.open(treated_filename, ios::out);

        string inFIleName = id_FilePath.toStdString();
        string data_line;
        fstream path_infile;

        path_infile.open(inFIleName.c_str(),ios::in);

        if(!path_infile)
        {
            QMessageBox::information(0, "Error!", "Open file failed");
            return;
        }


        int bee_id = 0;
        int bee_loc_x = 0;
        int bee_loc_y = 0;
        vector<int> ids,seleted_id;
        vector<cv::Point> locs, selected_loc;

        while( getline(path_infile,data_line))
        {
            stringstream get_data(data_line);

            while(!get_data.eof())
            {
                get_data >> bee_id;

                ids.push_back(bee_id);
            }

            getline(path_infile,data_line);
            stringstream get_data2(data_line);

            while(!get_data2.eof())
            {
                get_data2 >> bee_loc_x;
                get_data2 >> bee_loc_y;
                locs.push_back(cv::Point(bee_loc_x,bee_loc_y));
            }

            for(int i = 0; i < ids.size(); i++)
            {
                for(int j = 0; j < control_number.size(); j++)
                {
                    if(control_number[j] == 0)
                    {
                        if(ids[i] < 26 && ids[i] >=0)
                        {
                            seleted_id.push_back(ids[i]);
                            selected_loc.push_back(locs[i]);
                            break;
                        }
                    }
                    else
                    {
                        int num1 = ids[i]/control_number[j];
                        int num2 = ids[i]%control_number[j];

                        if(num1 == 1 && num2 < 26)
                        {
                            seleted_id.push_back(ids[i]);
                            selected_loc.push_back(locs[i]);
                            break;
                        }
                    }
                }
            }

            if(seleted_id.size()>0)
            {
                for(int i = 0; i < seleted_id.size(); i++)
                {
                    control_outfile << " " << seleted_id[i];
                }

                control_outfile << endl;

                for(int i = 0; i < selected_loc.size(); i++)
                {
                    control_outfile << " " << selected_loc[i].x << " " <<selected_loc[i].y ;
                }

                control_outfile << endl;
            }
            else
            {
                control_outfile << " " << -2 << endl;
                control_outfile << " " << -2 << " " << -2 << endl;
            }

            seleted_id.clear();
            selected_loc.clear();

            for(int i = 0; i < ids.size(); i++)
            {
                for(int j = 0; j < treated_number.size(); j++)
                {
                    if(treated_number[j] == 0)
                    {
                        if(ids[i] < 26 && ids[i] >=0)
                        {
                            seleted_id.push_back(ids[i]);
                            selected_loc.push_back(locs[i]);
                            break;
                        }

                    }
                    else
                    {
                        int num1 = ids[i]/treated_number[j];
                        int num2 = ids[i]%treated_number[j];

                        if(num1 == 1 && num2 < 26)
                        {
                            seleted_id.push_back(ids[i]);
                            selected_loc.push_back(locs[i]);
                            break;
                        }
                    }
                }
            }

            if(seleted_id.size()>0)
            {
                for(int i = 0; i < seleted_id.size(); i++)
                {
                    treated_outfile << " " << seleted_id[i];
                }

                treated_outfile << endl;

                for(int i = 0; i < selected_loc.size(); i++)
                {
                    treated_outfile << " " << selected_loc[i].x << " " <<selected_loc[i].y ;
                }

                treated_outfile << endl;
            }
            else
            {
                treated_outfile << " " << -2 << endl;
                treated_outfile << " " << -2 << " " << -2 << endl;
            }

            seleted_id.clear();
            selected_loc.clear();
            ids.clear();
            locs.clear();
        }
    }
    QMessageBox::information(0, 0, "Divide data successfully!");
}

void MainWindow::on_individual_bee_info_clicked()
{
    vector<int> inter_num;

    for(int i = 0; i < ui->inter_num->text().size(); i=i+3)
    {
        int L_num = int(ui->inter_num->text().at(i).toLatin1())-65;
        int R_num = int(ui->inter_num->text().at(i+1).toLatin1())-65;
        int IterN = L_num*26+R_num;
        cout << IterN << endl;
        inter_num.push_back(IterN);
    }


    vector<vector<double> > Info_box;
    vector<double> Info_temp;
    bool initial_flag = 1;

    QStringList id_FilePaths = QFileDialog::getOpenFileNames(this, tr("Open .id File"),
                                                             "D:/Research/InBee_Experiement/indoor/firstRound/test/bee_info",
                                                             tr("txt File (*txt)"));
    if (id_FilePaths.size() == 0)
    {
        QMessageBox::information(0, "Error!", "Need file name!");
        return;
    }

    //    QFileInfo Info(id_FilePaths.at(0));
    //    string basename = Info.baseName().toStdString();
    //    char temp1[11];
    //    for(int i = 0; i < 10; i++)
    //    {
    //        temp1[i] = basename.at(i);
    //    }
    //    temp1[10] = '\0';

    //    string temp2(temp1);
    //    string filename = Info.absolutePath().toStdString()+"/"+temp2+"_BeeInfo_0.txt";
    //    fstream outfile;
    //    outfile.open(filename, ios::out);

    foreach(QString id_FilePath, id_FilePaths)
    {
        string inFIleName = id_FilePath.toStdString();
        string data_line;
        fstream path_infile;

        path_infile.open(inFIleName.c_str(),ios::in);

        if(!path_infile)
        {
            QMessageBox::information(0, "Error!", "Open file failed");
            return;
        }

        int i = 0;
        float data = 0;
        while( getline(path_infile,data_line))
        {
            stringstream get_data(data_line);

            while(!get_data.eof())
            {
                get_data >> data;

                Info_temp.push_back(data);
            }

            if(initial_flag)
            {
                Info_box.push_back(Info_temp);
                Info_temp.clear();
            }
            else
            {
                for(int j = 0; j < Info_temp.size(); j++ )
                    Info_box[i].push_back(Info_temp[j]);

                i++;
                Info_temp.clear();
            }
        }
        initial_flag = 0;
    }

    cv::Mat out_img;
    int offset = 30;
    int gap = 100;
    int gap2 = gap*1.5;
    int img_rows = Info_box.size()*gap+offset*5;
    int img_cols = Info_box[0].size()/24*gap2+offset*5;

    out_img.create(img_rows, img_cols,CV_8UC3);

    for(int i = 0; i < out_img.rows; i++)
    {
        for(int j = 0; j < out_img.cols; j++)
        {
            out_img.ptr<uchar>(i)[j*3]=255;
            out_img.ptr<uchar>(i)[j*3+1]=255;
            out_img.ptr<uchar>(i)[j*3+2]=255;
        }
    }

    //橫座標 縱座標
    cv::line(out_img,cv::Point(offset*5,img_rows-offset*5),cv::Point(img_cols-offset,img_rows-offset*5),CV_RGB(0,0,0),2);
    cv::line(out_img,cv::Point(offset*5,img_rows-offset*5),cv::Point(offset*5,0),CV_RGB(0,0,0),2);

    for(int i = 0 ; i < Info_box.size(); i++)
    {
        int numL = inter_num[i] /26;
        int numR = inter_num[i] %26;

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

        // put text
        cv::putText(out_img,c_num,cv::Point(offset*0.5,(i+0.5)*gap),4,1,CV_RGB(0,0,0),2,CV_AA);
        // base line
        cv::line(out_img,cv::Point(offset*5,(i+1)*gap),cv::Point(img_cols,(i+1)*gap),CV_RGB(0,0,0),2);
        // midile line
        cv::line(out_img,cv::Point(offset*5,(i+0.5)*gap),cv::Point(img_cols-offset,(i+0.5)*gap),CV_RGB(100,100,100),1);
        cv::line(out_img,cv::Point(offset*5,(i+0.5)*gap),cv::Point(offset*5+5,(i+0.5)*gap),CV_RGB(0,0,0),2);
        cv::putText(out_img,QString::number(50).toStdString(),cv::Point(offset*3,(i+0.5)*gap),4,1,CV_RGB(0,0,0),2,CV_AA);
        cv::putText(out_img,"Date",cv::Point(offset*1.5+img_cols/2.0,img_rows-offset*2.2),4,1,CV_RGB(0,0,0),2,CV_AA);

        int t1 = 1;
        for(int j = 0; j < Info_box[i].size(); j++)
        {
            int value = Info_box[i][j];

            if(value > 0)
                cv::line(out_img,cv::Point(offset*5+(j+1)*gap2/24.0,(i+1)*gap),cv::Point(offset*5+(j+1)*gap2/24.0,(i+1)*gap-value),CV_RGB(0,0,255),2);

            int t2 = (j)/12;
            int t3 = (j)%12;
            if(t2%2 == 1 && t3 == 0)
            {
                cv::line(out_img,cv::Point(offset*5+(j+1)*gap2/24.0,(i+1)*gap),cv::Point(offset*5+(j+1)*gap2/24.0,(i+1)*gap-5),CV_RGB(0,0,0),2);
                cv::putText(out_img,QString::number(t1).toStdString(),cv::Point(offset*4.8+(j+1)*gap2/24.0,img_rows-offset*3.5),4,1,CV_RGB(0,0,0),1,CV_AA);
                t1++;
            }

        }

    }

    cv::imwrite("bee_info.png",out_img);
    cv::imshow("bee_info",out_img);
    cv::waitKey(0);
    cv::destroyAllWindows();

}


