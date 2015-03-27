#include "dataanalysis.h"

void dataanalysis::analysePath(vector<vector<int> > allBeeId, vector<vector<cv::Point> > allBeeLoc,
                               vector<timeHMS> allTime, QFileInfo fileInfo, const float pathparameters[5], vector<int> inter_NUM)
{
    srand(time(NULL));
    path_flag = true;
    path_flag1 = true;

    TAG_NUM = 26;
    overlap_count = 0;

    //set path analyse value
    path_value = pathparameters[0];
    miss_threshold = pathparameters[1];
    pathDist = pathparameters[2];
    overlap_value = pathparameters[3];
    pathtype_ratio = pathparameters[4];


    //set file direction
    filepath = fileInfo.absolutePath().toStdString()+"/"+ fileInfo.baseName().toStdString();


    for(int i = 0; i < allBeeId.size(); i++)
    {

        for(int j = 0; j < allBeeId[i].size(); j++)
        {
            orderPath(allBeeId[i][j], allBeeLoc[i][j], i, allTime[i]); //排序軌跡
        }

        if(whole_path.size()>0)
            storePath(); // 儲存軌跡
    }


    for(int i = 0; i < whole_path.size(); i++)
        storage_path.push_back(whole_path[i]);

    whole_path.clear();

    namePath(storage_path, fileInfo.absolutePath().toStdString()); //尋找每條軌跡的名稱

    findGoodPath(storage_path); //利用軌跡距離濾除雜訊

    findPathPH(reduce_path); //計算各時段軌跡次數、移動距離、長型團型軌跡比例

    sortPath(reduce_path, allBeeId.size(), inter_NUM); //用時間排序軌跡

    findIndividualData(reduce_path,inter_NUM); //計算單隻蜜蜂資訊

    drawSingleNumPath(reduce_path); //畫出個別蜜蜂所有軌跡

    /*findDAdistribution(reduce_path); //計算DA WH 分布

    findSpeedDistribution(reduce_path); //計算Speed 分布*/
}

void dataanalysis::orderPath(const int num, cv::Point pt,const int f_count, timeHMS timeData) //利用標籤和位置排序軌跡
{
    vector<struct bee_path> inner_path;
    struct bee_path num_p;

    if(num!=-2)
    {
        if(num!=-1)
        {
            bool number_flag = true;
            if(path_flag) //初始化用
            {
                num_p.color[0]=(rand()%200)+50;
                num_p.color[1]=(rand()%200)+50;
                num_p.color[2]=(rand()%200)+50;
                num_p.number = num;
                num_p.dist = 0;
                num_p.position = pt;
                num_p.frameNum = f_count;
                num_p.T_flag = false;
                num_p.timedata = timeData;
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
                    cv::Point tempD = whole_path[j].back().position-pt;
                    double dist = norm(tempD);

                    if(dist < minDistance)
                    {
                        minDistance = dist;
                        minP = j;
                    }

                    if(num == whole_path[j].back().number && dist < path_value ) //號碼相同且距離小於閥值
                    {
                        number_flag = false;
                        num_p.color[0] = whole_path[j][0].color[0];
                        num_p.color[1] = whole_path[j][0].color[1];
                        num_p.color[2] = whole_path[j][0].color[2];
                        num_p.number = num;
                        num_p.position = pt;
                        num_p.frameNum = f_count;
                        num_p.timedata = timeData;
                        whole_path[j].push_back(num_p);
                    }

                }
                if(number_flag)
                {
                    if(minDistance < path_value )  //號碼不同但距離小於閥值
                    {
                        num_p.color[0] = whole_path[minP][0].color[0];
                        num_p.color[1] = whole_path[minP][0].color[1];
                        num_p.color[2] = whole_path[minP][0].color[2];
                        num_p.number = num;
                        num_p.position = pt;
                        num_p.frameNum = f_count;
                        num_p.timedata = timeData;
                        whole_path[minP].push_back(num_p);
                    }
                    else //產生新的軌跡矩陣
                    {
                        num_p.color[0]=(rand()%200)+50;
                        num_p.color[1]=(rand()%200)+50;
                        num_p.color[2]=(rand()%200)+50;
                        num_p.number = num;
                        num_p.dist = 0;
                        num_p.position = pt;
                        num_p.frameNum = f_count;
                        num_p.T_flag = false;
                        num_p.timedata = timeData;
                        inner_path.push_back(num_p);
                        whole_path.push_back(inner_path);
                        path_counter.push_back(0);
                        inner_path.clear();
                    }
                }
            }
        }
        else //利用最小距離追蹤軌跡
        {
            int minP;
            double minDistance = 100000000000;
            for(int j=0; j<whole_path.size(); j++)
            {
                cv::Point tempD = whole_path[j].back().position-pt;
                double dist = norm(tempD);
                if(dist < minDistance)
                {
                    minDistance = dist;
                    minP = j;
                }
            }
            if(minDistance < path_value) //距離小於閥值
            {
                num_p.color[0] = whole_path[minP][0].color[0];
                num_p.color[1] = whole_path[minP][0].color[1];
                num_p.color[2] = whole_path[minP][0].color[2];
                num_p.number = whole_path[minP].back().number;
                num_p.position = pt;
                num_p.frameNum = f_count;
                num_p.timedata = timeData;
                whole_path[minP].push_back(num_p);
            }
            else //距離大於閥值，故產生新的軌跡陣列
            {
                num_p.color[0]=(rand()%200)+50;
                num_p.color[1]=(rand()%200)+50;
                num_p.color[2]=(rand()%200)+50;
                num_p.number = -1;
                num_p.dist = 0;
                num_p.position = pt;
                num_p.frameNum = f_count;
                num_p.T_flag = false;
                num_p.timedata = timeData;
                inner_path.push_back(num_p);
                whole_path.push_back(inner_path);
                path_counter.push_back(0);
                inner_path.clear();
            }
        }
    }

}

void dataanalysis::storePath() //當蜜蜂超出視野一定時間後，將軌跡儲存下來
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

            if(path_counter[i] > miss_threshold)
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

void dataanalysis::namePath(vector<vector<bee_path> > path, string fpath)//利用投票法來找每條軌跡的名稱
{
    fstream all_file;
    all_file.open(filepath+"_allPath.txt",ios::out);

    for(int i = 0; i < path.size(); i++)
    {
        int tagNum[677]={0};
        int massNum = -1;
        int massVal = 0;

        for(int j = 0; j < path[i].size(); j++)
        {
            if(path[i][j].number >= 0)
            {
                int index = path[i][j].number;
                tagNum[index]++;
            }
            else
                tagNum[676]++;
        }

        for(int k = 0; k < 677; k++)
        {
            if(tagNum[k] > massVal)
            {
                massVal = tagNum[k];
                massNum = k;
            }
        }

        if(massNum < 676 && massNum >= 0)
            path[i][0].number = massNum;
    }

    // reload the paths after rename
    storage_path.clear();

    for(int i = 0; i < path.size(); i++)
        storage_path.push_back(path[i]);

    // 按照軌跡名稱排序
    for(int i = 0; i < storage_path.size()-1; i++)
    {
        for(int j = i+1; j < storage_path.size(); j++)
        {
            if(storage_path[i][0].number > storage_path[j][0].number)
                swap(storage_path[i],storage_path[j]);
        }
    }

    // 軌跡內的位置點名稱重新命名
    for(int i = 0; i < storage_path.size();i++)
    {
        for(int j = 1; j < storage_path[i].size(); j++)
        {
            storage_path[i][j].number = storage_path[i][0].number;
        }
    }

    // 儲存過濾前的軌跡
    for(int i = 0; i < storage_path.size();i++)
    {
        char c_num[3];
        NumberTranslate(storage_path[i][0].number, c_num);

        // record paths before eliminating noise
        if(storage_path[i][0].number > 0)
            all_file << c_num << endl;
        else
            all_file << "-1" << endl;
    }    
    all_file.close();
}

void dataanalysis::sortPath(vector<vector<bee_path> > path, const int fNum, vector<int> inter_num) //用時間排序軌跡
{

    vector<int> ids,ids2;
    vector<cv::Point> locs, locs2;

    fstream sortfile;

    sortfile.open(filepath+"_sortFile.txt",ios::out);

    int delta_T12 = 0;

    cv::Mat inter_matrix12;
    inter_matrix12 = cv::Mat::zeros(676,676,CV_64FC1);

    for(int i = 0; i < fNum; i++)
    {
        delta_T12++;
         for(int j = 0; j < path.size(); j++)
         {
            if(path[j][0].number > 0)
            {
                if(path[j][0].frameNum == i)
                {
                    if(path[j][0].number<0)
                        cout<< path[j].size() <<" faild "<<endl;

                    ids2.push_back(path[j][0].number);
                    locs2.push_back(path[j][0].position);


                    for(int i = 0; i < inter_num.size(); i++)
                    {
                        if(path[j][0].number == inter_num[i])
                        {
                            ids.push_back(path[j][0].number);
                            locs.push_back(path[j][0].position);
                            break;
                        }
                    }

                    if(path[j].size()>1)
                        path[j].erase(path[j].begin());

                }
            }
        }

        if(ids2.size()>0)
        {
            for(int i = 0; i < ids2.size(); i++)
                sortfile <<" "<< ids2[i];

            sortfile << endl;

            for(int i = 0; i < locs2.size(); i++)
                sortfile <<" "<< locs2[i].x <<" "<< locs2[i].y;

            sortfile << endl;
        }
        else
        {
            sortfile << " " << -2 << endl;
            sortfile << " " << -2 << " " << -2 << endl;
        }


        if(delta_T12 == 12)
        {
            delta_T12 = 0;

            if(ids.size() > 0)
                findOverlap(ids, locs, inter_matrix12); //尋找每個frame蜜蜂位置重疊次數和隻數
        }

        ids.clear();
        locs.clear( );
        ids2.clear();
        locs2.clear( );
    }


    vector<vector<int> > order_interaction;
    vector<int> order_temp;

    fstream inter_file12;
    inter_file12.open(filepath+"_interation12.txt",ios::out);

    // 12
    for(int i = 0; i < inter_num.size(); i++)
    {
        for(int k = 0; k < i+1; k++)
        {
            order_temp.push_back(0);
            inter_file12 << 0 << " ";
        }

        for(int j = i+1; j < inter_num.size(); j++)
        {
            order_temp.push_back(inter_matrix12.ptr<int>(inter_num[i])[inter_num[j]]);
            inter_file12 << inter_matrix12.ptr<int>(inter_num[i])[inter_num[j]] << " ";
        }
        order_interaction.push_back(order_temp);
        order_temp.clear();
        inter_file12 << endl;
    }

    orderInteraction(order_interaction,inter_num);

    inter_matrix12.release();
}

void dataanalysis::findOverlap(vector<int> ids, vector<cv::Point> locs, cv::Mat &inter_m) //統計蜜蜂重疊次數
{

    for(int i = 0; i < locs.size();i++)
    {
        for(int j = i+1; j < locs.size();j++)
        {
            cv::Point tempD = locs[i] - locs[j];

            double dist = sqrt(norm(tempD));

            if(dist < overlap_value)
            {
                int id_t1 = ids[i];
                int id_t2 = ids[j];

                if(id_t1 > id_t2)
                    swap(id_t1,id_t2);

                inter_m.ptr<int>(id_t1)[id_t2] = inter_m.ptr<int>(id_t1)[id_t2]+1;
                //cout <<  inter_m.ptr<int>(id_t1)[id_t2] << endl;

            }
        }
    }
}

void dataanalysis::caculateSD(vector<cv::Point> allpoint, float boundary[])
{
    int length = allpoint.size();
    float min_x,min_y,max_x,max_y;
    float sum_x = 0;
    float sum_y = 0;
    vector<int> allpoint_x, allpoint_y;


    for(int i = 0; i < allpoint.size(); i++)
    {
        allpoint_x.push_back(allpoint[i].x);
        allpoint_y.push_back(allpoint[i].y);
        sum_x = sum_x + allpoint[i].x;
        sum_y = sum_y + allpoint[i].y;
    }

    float minX = 3500;
    float MaxX = 0;
    float minY = 3500;
    float MaxY = 0;

    for(int i = 0; i < allpoint_x.size(); i++)
    {
        if(allpoint_x[i] < minX)
            minX = allpoint_x[i];

        if(allpoint_y[i] < minY)
            minY = allpoint_y[i];


        if(allpoint_x[i] > MaxX)
            MaxX = allpoint_x[i];

        if(allpoint_y[i] > MaxY)
            MaxY = allpoint_y[i];
    }

    min_x = minX;
    min_y = minY;
    max_x = MaxX;
    max_y = MaxY;

    boundary[0] = min_x;
    boundary[1] = min_y;
    boundary[2] = max_x;
    boundary[3] = max_y;

    float lx = abs(boundary[2]-boundary[0]);
    float ly = abs(boundary[3]-boundary[1]);

    //cout << "range " << lx*ly/92.3/92.3 << endl;

    double mean_x = sum_x/length;
    double mean_y = sum_y/length;

    sum_x = 0;
    sum_y = 0;

    for(int i = 0; i < allpoint.size(); i++)
    {
        double px = allpoint[i].x - mean_x;
        double py = allpoint[i].y - mean_y;
        sum_x = sum_x + pow(px,2);
        sum_y = sum_y + pow(py,2);
    }

    double SD_x = sqrt(sum_x/length);
    double SD_y = sqrt(sum_y/length);
    //cout << mean_x/100 <<" "<<mean_y/100<<endl;
    //cout << SD_x/100<<" "<<SD_y/100<<endl;
}

void dataanalysis::region_distribution(vector<vector<bee_path> > path) // x_max = 3199; y_max = 1199
{
    fstream outfile;
    string filename = filepath + "_1regionD.txt";
    outfile.open(filename,ios::out);

    int region_count[8] = {0};

    for(int i = 0; i < path.size(); i++)
    {
        for(int j = 0; j < path[i].size(); j++)
        {
            int region_x = path[i][j].position.x/800;
            int region_y = path[i][j].position.y/600;

            if(region_x == 0 && region_y == 0) //(0,0)
                region_count[0]++;

            if(region_x == 1 && region_y == 0) //(1,0)
                region_count[1]++;

            if(region_x == 2 && region_y == 0) //(2,0)
                region_count[2]++;

            if(region_x == 3 && region_y == 0) //(3,0)
                region_count[3]++;

            if(region_x == 0 && region_y == 1) //(0,1)
                region_count[4]++;

            if(region_x == 1 && region_y == 1) //(1,1)
                region_count[5]++;

            if(region_x == 2 && region_y == 1) //(2,1)
                region_count[6]++;

            if(region_x == 3 && region_y == 1) //(3,1)
                region_count[7]++;
        }
    }

    for(int i = 0; i < 8; i++)
        outfile << region_count[i] << endl;

    outfile.close();
}

void dataanalysis::NumberTranslate(const int number, char c_num[]) // translate number to character
{
    int numL = number /TAG_NUM;
    int numR = number %TAG_NUM;

    if(numL==0)
        c_num[0]=(char)(65);
    else
        c_num[0]=(char)(65+numL);

    if(numR==0)
        c_num[1]=(char)(65);
    else
        c_num[1]=(char)(65+numR);

    c_num[2]='\0';
}

void dataanalysis::drawSingleNumPath(vector<vector<bee_path> > path)
{
    cv::Mat pathImg;
    pathImg.create(1200,3200,CV_8UC3);

    int pathid = path[0][0].number;
    vector<cv::Point> allpoint;

    for(int i = 0; i < path.size(); i++)
    {
        char c_num[3];
        NumberTranslate(path[i][0].number, c_num);

        int R = path[i][0].color[0];
        int G = path[i][0].color[1];
        int B = path[i][0].color[2];

        if(path[i][0].number == pathid)
        {
            //cv::putText(pathImg,c_num,cv::Point(path[i][0].position.x,path[i][0].position.y),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][0].timedata.hour).toStdString(),cv::Point(path[i][0].position.x+20,path[i][0].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][0].timedata.min).toStdString(),cv::Point(path[i][0].position.x+60,path[i][0].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][0].timedata.sec).toStdString(),cv::Point(path[i][0].position.x+100,path[i][0].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);

            for(int j = 0; j < path[i].size()-1; j++)
            {
                allpoint.push_back(path[i][j].position);
                cv::line(pathImg,path[i][j].position,path[i][j+1].position,CV_RGB(B,G,R),1);
            }
            int end = path[i].size()-1;
            //cv::putText(pathImg,c_num,cv::Point(path[i][end].position.x,path[i][end].position.y),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][end].timedata.hour).toStdString(),cv::Point(path[i][end].position.x+20,path[i][end].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][end].timedata.min).toStdString(),cv::Point(path[i][end].position.x+60,path[i][end].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);
            //cv::putText(pathImg,QString::number(path[i][end].timedata.sec).toStdString(),cv::Point(path[i][end].position.x+100,path[i][end].position.y+10),3,1,CV_RGB(B,G,R), 1, CV_AA);

            if(i == path.size()-1)
            {
                char c_num[3];
                NumberTranslate(pathid, c_num);                
                float boundary[4] = {0};
                caculateSD(allpoint,boundary);
                cv::rectangle(pathImg,cv::Point( boundary[0],boundary[1]),cv::Point(boundary[2],boundary[3]),CV_RGB(255,255,0),3,CV_AA);
                allpoint.clear();
                cv::imwrite(filepath+"_"+QString(c_num).toStdString()+".png",pathImg);
                pathImg.release();
            }

        }
        else
        {
            char c_num[3];
            NumberTranslate(pathid, c_num);


            float boundary[4] = {0};
            caculateSD(allpoint,boundary);
            cv::rectangle(pathImg,cv::Point( boundary[0],boundary[1]),cv::Point(boundary[2],boundary[3]),CV_RGB(255,255,0),3,CV_AA);
            allpoint.clear();
            cv::Mat resizeImg;
            //resizeImg.create(800,300,CV_8UC3);

            cv::resize(pathImg,resizeImg,cv::Size(1600,600),cv::INTER_NEAREST );

            cv::imwrite(filepath+"_"+QString(c_num).toStdString()+".png",pathImg);
            pathid = path[i][0].number;

            i=i-1;

            pathImg.release();
            pathImg.create(1200,3200,CV_8UC3);
        }
    }

}

void dataanalysis::drawSinglePath(vector<bee_path> path,int p_n)
{
    cv::Mat pathImg;
    pathImg.create(1200,3200,CV_8UC3);

    int R = path[0].color[0];
    int G = path[0].color[1];
    int B = path[0].color[2];

    char c_num[3];
    NumberTranslate(path[0].number, c_num);

    for(int i = 0; i < path.size()-1; i++)
    {
        cv::line(pathImg,path[i].position,path[i+1].position,CV_RGB(B,G,R),1);
    }
    cv::putText(pathImg,c_num,cv::Point(path[0].position.x,path[0].position.y),3,1,CV_RGB(B,G,R), 1, CV_AA);
    cv::imwrite(filepath+"_"+QString::number(p_n).toStdString()+"_"+c_num+".png",pathImg);

}

void dataanalysis::orderInteraction(vector<vector<int> > inter_data, vector<int> inter_num)
{
    int matrix_size = inter_data.size();
    vector<vector<int> > inter_order,inter_inverse;
    vector<int> temp,tempInverse,inter_index;

    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = 0; j < matrix_size; j++)
        {
            tempInverse.push_back(0);
            temp.push_back(inter_data[i][j]);
        }
        inter_inverse.push_back(tempInverse);
        inter_index.push_back(i);
        inter_order.push_back(temp);
        temp.clear();
        tempInverse.clear();
    }
    //cout << "ok1" << endl;

    // 產生對稱矩陣
    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = 0; j < matrix_size; j++)
        {
            if(i!=j)
            {
                int data = inter_order[i][j];

                if(inter_order[j][i] == 0 )
                {
                    inter_order[j][i] = data;
                }
            }
        }
    }

    //cout << "ok2" << endl;

    // 產生反矩陣
    //    for(int i = 0; i < matrix_size; i++)
    //    {
    //        for(int j = 0; j < matrix_size; j++)
    //        {
    //            int data = inter_data[i][j];

    //            inter_inverse[j][i] = data;
    //        }
    //    }

    // 計算列向量和
    vector<int> inter_count;
    for(int i = 0; i < matrix_size; i++)
    {
        int count = 0;
        for(int j = 0; j < matrix_size; j++)
        {
            count = count+inter_order[i][j];
        }
        inter_count.push_back(count);
        //cout << count <<endl;
    }

    //cout << "ok3" << endl;
    // 由大到小排序列向量
    for(int i = 0; i < matrix_size-1; i++)
    {
        for(int j = i+1; j < matrix_size; j++)
        {
            if(inter_count[i] < inter_count[j])
            {
                swap(inter_count[i],inter_count[j]);
                swap(inter_index[i],inter_index[j]);
                //swap( inter_inverse[i], inter_inverse[j]);
                swap(inter_num[i],inter_num[j]);
            }
        }
    }

    //cout << "ok4" << endl;
    float max_value = -1;
    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = i+1; j < matrix_size; j++)
        {
            int row = inter_index[i];
            int col = inter_index[j];
            inter_inverse[i][j] =  inter_order[row][col];

            if(inter_inverse[i][j] > max_value)
                max_value = inter_inverse[i][j];
        }
    }

    cv::Mat inter_img;

    int img_size = 30;

    inter_img = cv::Mat::zeros(img_size*matrix_size, img_size*matrix_size, CV_8UC1);

    for(int i = 0; i < matrix_size; i++)
    {
        for(int j = 0; j < matrix_size; j++)
        {
            double value = inter_inverse[i][j];

            value = (value/max_value)*255;

            if(value > 255)
                value = 255;

            if(value < 0)
                value = 0;

            for(int k = i*img_size; k < img_size*(i+1); k++)
            {
                for(int p = j*img_size; p < img_size*(j+1); p++)
                {
                    inter_img.ptr<uchar>(k)[p] = value;
                }
            }
        }
    }


    cv::Mat output_Img,output2;
    output_Img = cv::Mat::zeros(inter_img.rows+img_size,inter_img.cols+img_size,CV_8UC3);

    for(int i = 0; i < output_Img.rows; i++)
    {
        for(int j = 0; j < output_Img.cols; j++)
        {
            output_Img.ptr<uchar>(i)[j*3]=255;
            output_Img.ptr<uchar>(i)[j*3+1]=0;
            output_Img.ptr<uchar>(i)[j*3+2]=0;
        }
    }   

    for(int i = 0; i < output_Img.rows; i++)
    {
        for(int j = 0; j < output_Img.cols; j++)
        {
            if(i>=img_size&&j>=img_size)
            {
                int value = inter_img.ptr<uchar>(i-img_size)[j-img_size];
                output_Img.ptr<uchar>(i)[j*3]=value;
                output_Img.ptr<uchar>(i)[j*3+1]=value;
                output_Img.ptr<uchar>(i)[j*3+2]=value;
            }
        }
    }



    for(int i = 0; i < inter_num.size(); i++)
    {
        char c_num[3];
        NumberTranslate(inter_num[i], c_num);
        //        cv::putText(output_Img,c_num,cv::Point(10,img_size*(i+1.6)),1,2.2,CV_RGB(0,255,0),2,CV_AA);
        //        cv::putText(output_Img,c_num,cv::Point(img_size*(i+1.2),40),1,2.2,CV_RGB(0,255,0),2,CV_AA);


        //                cv::putText(output_Img,c_num,cv::Point(10,img_size*(i+1.6)),1,1.8,CV_RGB(0,255,0),2,CV_AA);
        //                cv::putText(output_Img,c_num,cv::Point(img_size*(i+1),40),1,1.8,CV_RGB(0,255,0),2,CV_AA);

        cv::putText(output_Img,c_num,cv::Point(5,img_size*(i+1.6)),1,1,CV_RGB(0,255,0),2,CV_AA);
        cv::putText(output_Img,c_num,cv::Point(img_size*(i+1),20),1,1,CV_RGB(0,255,0),2,CV_AA);
        //cv::putText(output_Img,c_num,cv::Point(img_size/5,img_size*(i+1.6)),3,2,CV_RGB(0,255,0),matrix_size/5,CV_AA);
        //cv::putText(output_Img,c_num,cv::Point(img_size*(i+1.1),img_size/1.5),3,2,CV_RGB(0,255,0),matrix_size/5,CV_AA);

    }


    cv::imwrite(filepath+"_"+QString::number(max_value).toStdString()+"_"+"interaction_img.jpg", output_Img,cv::colormap::Jet());



    //    for(int i = 0; i < matrix_size; i++)
    //    {
    //        char c_num[3];
    //        NumberTranslate(inter_num[i],c_num);
    //        cout << c_num << " ";
    //        for(int j = 0; j < matrix_size; j++)
    //        {

    //            cout << inter_inverse[i][j] << " ";
    //        }
    //        cout << endl;
    //    }

    cout << "max_value = " << max_value << endl;


    inter_data.clear();
    inter_order.clear();
    inter_count.clear();
    inter_num.clear();
}

void dataanalysis::findDAdistribution(vector<vector<bee_path> > path)
{
    fstream outfile, outfile2;
    outfile.open(filepath+"_DAdistribution.txt",ios::out);
    outfile2.open(filepath+"_WHdistribution.txt",ios::out);
    int array_da[10000]={0};
    int array_wh[10000]={0};

    for(int i = 0; i < path.size(); i++)
    {
        cv::Mat pathImg;
        float distance = 0;
        pathImg.create(1200,3200,CV_8UC3);
        int R = path[i][0].color[0];
        int G = path[i][0].color[1];
        int B = path[i][0].color[2];

        // 計算單一軌跡長度
        for(int j = 0; j < path[i].size()-1; j++)
        {
            cv::line(pathImg,path[i][j].position,path[i][j+1].position,CV_RGB(B,G,R),1);
            float temp_distance = norm(path[i][j].position - path[i][j+1].position);
            distance = distance+temp_distance;
        }

        char c_num[3];
        NumberTranslate(path[i][0].number, c_num);

        cv::Mat src = pathImg.clone();
        cv::cvtColor(src.clone(),src,CV_BGR2GRAY);
        cv::Mat threshold_output;
        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;

        // Detect edges using Threshold
        cv::threshold( src, threshold_output, 0, 255, cv::THRESH_BINARY );

        // retrieves only the extreme outer contours
        cv::findContours( threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        cv::RotatedRect minRect;

        minRect = cv::minAreaRect( cv::Mat(contours[0]));

        cv::Point2f rect_points[4]; minRect.points( rect_points );

        //        for( int j = 0; j < 4; j++ )
        //            cv::line( pathImg, rect_points[j], rect_points[(j+1)%4], CV_RGB(255,0,0), 2, 8 );


        //        int quarter_size = path[i].size()*0.25;
        //        cv::Point q1_point(0,0), q3_point(0,0);

        //        for(int j = 0; j < quarter_size; j++)
        //        {
        //            q1_point.x = q1_point.x+path[i][j].position.x;
        //            q1_point.y = q1_point.y+path[i][j].position.y;
        //        }

        //        for(int j = path[i].size()-quarter_size; j < path[i].size(); j++)
        //        {
        //            q3_point.x = q3_point.x+path[i][j].position.x;
        //            q3_point.y = q3_point.y+path[i][j].position.y;
        //        }

        //        q1_point.x = q1_point.x/quarter_size;
        //        q1_point.y = q1_point.y/quarter_size;
        //        q3_point.x = q3_point.x/quarter_size;
        //        q3_point.y = q3_point.y/quarter_size;


        //        cv::line( pathImg, q1_point, q3_point, CV_RGB(255,255,255), 2, 8 );
        //        cv::line( pathImg, path[i][0].position, path[i][path[i].size()-1].position, CV_RGB(0,255,0), 2, 8 );
        //        cv::circle(pathImg,path[i][0].position,2,CV_RGB(0,255,0),2,1,0);
        //        cv::circle(pathImg,path[i][path[i].size()-1].position,2,CV_RGB(0,0,255),2,1,0);
        //        cv::circle(pathImg,q1_point,2,CV_RGB(0,255,0),2,1,0);
        //        cv::circle(pathImg,q3_point,2,CV_RGB(0,0,255),2,1,0);


        float b_width = minRect.size.width;
        float b_height = minRect.size.height;
        float b_area = minRect.size.area();

        if(b_width > 0 && b_height > 0)
        {
            float ratio = 0;
            if(b_width >= b_height)
            {
                ratio = b_width/b_height;
            }
            else
            {
                ratio = b_height/b_width;
            }

            float divisor = 1;
            for(int j = 0; j < 10000; j++)
            {
                int a = ratio/divisor;
                float b = ratio-divisor;

                if(ratio > 1001)
                {
                    array_wh[9999]++;
                    divisor = divisor+0.1;
                    break;
                }
                else if(a == 1 && b < 0.1 )
                {
                    array_wh[j]++;
                    divisor = divisor+0.1;
                    break;
                }
                divisor = divisor+0.1;
            }
        }

        if(b_area > 0)
        {
            float DA = distance/b_area;
            float divisor = 0.001;
            for(int j = 0; j < 10000; j++)
            {
                int a = DA/divisor;
                float b = DA-divisor;

                if(DA > 10)
                {
                    array_da[9999]++;
                    divisor = divisor+0.001;
                    break;
                }
                else if(DA < 0.001)
                {
                    array_da[0]++;
                    divisor = divisor+0.001;
                    break;
                }
                else if(a == 1 && b < 0.001 )
                {
                    array_da[j]++;
                    divisor = divisor+0.001;
                    break;
                }
                divisor = divisor+0.001;
            }
            cv::putText(pathImg,QString::number(DA).toStdString(),cv::Point(600,600),5,5,CV_RGB(B,G,R), 5, CV_AA);
        }
        //cv::imwrite(filepath+QString::number(i).toStdString()+".png",pathImg);
    }

    for(int i = 0; i < sizeof(array_da)/sizeof(array_da[0]); i++)
        outfile << array_da[i] << endl;

    for(int i = 0; i < sizeof(array_wh)/sizeof(array_wh[0]); i++)
        outfile2 << array_wh[i] << endl;


    outfile.close();
    outfile2.close();

}

void dataanalysis::findGoodPath(vector<vector<bee_path> > path)
{
    reduce_path.clear();

    for(int i = 0; i < path.size(); i++)
    {
        float total_distance = 0;
        for(int j = 0; j < path[i].size()-1; j++)
        {
            float distance = norm(path[i][j].position - path[i][j+1].position);
            total_distance = total_distance + distance;
        }

        if(path[i][0].number > 0 && total_distance > pathDist)
        {
            //drawSinglePath(path[i],i); // 畫出每一條軌跡
            reduce_path.push_back(path[i]);
        }
    }

}

void dataanalysis::findPathPH(vector<vector<bee_path> > path)
{
    fstream count_outfile, distance_outfile, longRatio_outfile;
    count_outfile.open(filepath+"_countPH.txt", ios::out);
    distance_outfile.open(filepath+"_distancePH.txt", ios::out);
    longRatio_outfile.open(filepath+"_longRatioPH.txt", ios::out);
    timeInfo time_info[24] = {0};

    for(int i = 0; i < path.size(); i++)
    {
        float distance = 0;
        cv::Mat pathImg;
        pathImg.create(1200,3200,CV_8UC1);

        // 計算單一軌跡長度
        for(int j = 0; j < path[i].size()-1; j++)
        {
            cv::line(pathImg,path[i][j].position,path[i][j+1].position, 255,1);
            float tempdistance = norm(path[i][j].position - path[i][j+1].position);
            distance = distance + tempdistance;
        }

        cv::Mat src = pathImg.clone();
        //cv::cvtColor(src.clone(),src,CV_BGR2GRAY);
        cv::Mat threshold_output;
        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;

        // Detect edges using Threshold
        cv::threshold( src, threshold_output, 0, 255, cv::THRESH_BINARY );

        // retrieves only the extreme outer contours
        cv::findContours( threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        cv::RotatedRect minRect;

        minRect = cv::minAreaRect( cv::Mat(contours[0]));

        //        cv::Point2f rect_points[4]; minRect.points( rect_points );

        //        for( int j = 0; j < 4; j++ )
        //            cv::line( pathImg, rect_points[j], rect_points[(j+1)%4], 255, 2, 8 );

        //        cv::imwrite(filepath+QString::number(i).toStdString()+".png",pathImg);

        float b_width = minRect.size.width;
        float b_height = minRect.size.height;

        int index = path[i][0].timedata.hour;



        //        if(b_width>=b_height)
        //            cout <<  b_width/b_height << endl;


        //        if(b_width < b_height)
        //            cout << b_height/b_width << endl;


        if(b_width/b_height>=pathtype_ratio || b_height/b_width>=pathtype_ratio)
        {
            time_info[index].long_type++;
        }

        time_info[index].goodpath_c++;

        time_info[index].goodpath_dist = time_info[index].goodpath_dist+distance;
    }

    for(int i = 0; i < 24; i++)
    {
        double typeRatio;

        if(time_info[i].goodpath_c > 0)
            typeRatio = time_info[i].long_type/(double)time_info[i].goodpath_c;
        else
            typeRatio = 0;

        count_outfile << time_info[i].goodpath_c << endl;
        distance_outfile << time_info[i].goodpath_dist/100.0 << endl; // cm
        longRatio_outfile << typeRatio << endl;
    }

    count_outfile.close();
    distance_outfile.close();
    longRatio_outfile.close();
}

void dataanalysis::findIndividualData(vector<vector<bee_path> > path, vector<int> inter_num)
{

    timeInfo timeBee_info[676][24] = {0};

    for(int i = 0; i < path.size(); i++)
    {
        float distance = 0;
        cv::Mat pathImg;
        pathImg.create(1200,3200,CV_8UC1);

        // 計算單一軌跡長度
        for(int j = 0; j < path[i].size()-1; j++)
        {
            cv::line(pathImg,path[i][j].position,path[i][j+1].position, 255,1);
            float tempdistance = norm(path[i][j].position - path[i][j+1].position);
            distance = distance + tempdistance;
        }

        cv::Mat src = pathImg.clone();
        //cv::cvtColor(src.clone(),src,CV_BGR2GRAY);
        cv::Mat threshold_output;
        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;

        // Detect edges using Threshold
        cv::threshold( src, threshold_output, 0, 255, cv::THRESH_BINARY );        

        // retrieves only the extreme outer contours
        cv::findContours( threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

        cv::RotatedRect minRect;

        minRect = cv::minAreaRect( cv::Mat(contours[0]));


        float b_width = minRect.size.width;
        float b_height = minRect.size.height;

        int time_index = path[i][0].timedata.hour;
        int number_index = path[i][0].number;


        if(b_width/b_height>=pathtype_ratio || b_height/b_width>=pathtype_ratio)
        {
            timeBee_info[number_index][time_index].long_type++;
        }

        timeBee_info[number_index][time_index].goodpath_c++;

        timeBee_info[number_index][time_index].goodpath_dist = timeBee_info[number_index][time_index].goodpath_dist+distance;
    }


    // draw picture

    //    QString name;
    //    cv::Mat out_img;

    //    for(int i = 0; i < 676; i++)
    //    {
    //        for(int j = 0; j < inter_num.size(); i++)
    //        {
    //            if(j == inter_num[i])
    //            {
    //                for(int z = 0; z < 24; z++)
    //                {



    //                    timeBee_info[i][z].goodpath_c;

    //                }

    //                break;
    //            }
    //        }

    //    }

    fstream beeInfo_file;

    for(int k = 0; k < 3; k++)
    {
        QString name;

        name = +"_BeeInfo_"+QString::number(k)+".txt";
        beeInfo_file.open(filepath+name.toStdString().c_str(),ios::out);

        for(int i = 0; i < 676; i++)
        {
            for(int z = 0; z < inter_num.size(); z++)
            {
                if( i == inter_num[z])
                {
                    //char c_num[3];
                    //NumberTranslate(i, c_num);
                    //beeInfo_file << c_num << " ";
                    for(int j = 0; j < 24; j++)
                    {
                        if(k==0)
                            beeInfo_file <<" "<<timeBee_info[i][j].goodpath_c;

                        else if(k==1)
                            beeInfo_file <<" " << timeBee_info[i][j].goodpath_dist/100.0;

                        else
                        {
                            int count = timeBee_info[i][j].goodpath_c;
                            double ratio;
                            if(count > 0)
                                ratio = timeBee_info[i][j].long_type/(double)count;
                            else
                                ratio = 0;

                            beeInfo_file << " " << ratio ;
                        }
                    }
                    beeInfo_file << endl;
                    break;
                }
            }
        }
        beeInfo_file.close();
    }
}

void dataanalysis::findSpeedDistribution(vector<vector<bee_path> > path)
{

    fstream outfile;
    outfile.open(filepath+"_"+"speedDistrubution.txt",ios::out);
    int array_speed[10000] = {0};

    for(int i = 0; i < path.size(); i++)
    {
        float total_distance = 0;
        for(int j = 0; j < path[i].size()-1; j++)
        {
            float distance = norm(path[i][j].position - path[i][j+1].position);
            total_distance = total_distance + distance;
        }

        int hour = path[i][path[i].size()-1].timedata.hour - path[i][0].timedata.hour;
        int min =  path[i][path[i].size()-1].timedata.min - path[i][0].timedata.min;
        int sec =  path[i][path[i].size()-1].timedata.sec - path[i][0].timedata.sec;

        float duration = hour*60*60+min*60+sec;

        float speed = total_distance/100.0/duration*60.0; // 每分鐘移動幾公分


        float divisor = 0.5;

        for(int j = 0; j < 10000; j++)
        {
            int a = speed/divisor;
            float b = speed-divisor;

            if(speed > 5000)
            {
                array_speed[9999]++;
                divisor = divisor+0.5;
                break;
            }
            else if(speed < 0.5)
            {
                array_speed[0]++;
                divisor = divisor+0.5;
                break;
            }
            else if(a == 1 && b < 0.5)
            {
                array_speed[j]++;
                divisor = divisor+0.5;
                break;
            }
            divisor = divisor+0.5;
        }
    }

    for(int i = 0; i < sizeof(array_speed)/sizeof(array_speed[0]); i++)
        outfile << array_speed[i] << endl;


    outfile.close();

}



































