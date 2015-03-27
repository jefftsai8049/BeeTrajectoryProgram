#include "imgP_bf1.h"

//typedef unsigned char char;

//===========================================

void vexGetImage(IplImage*src,IplImage*dst,CvRect img_rect,int flag)
{

    //IplImage*dst=cvCreateImage( cvSize(img_rect.width,img_rect.height) ,8,1);
    for(int i=0;i<dst->height;i++)
    {
        byte *line1=(byte*)(src->imageData+ ( img_rect.y+i )*src->widthStep);
        byte *line2=(byte*)(dst->imageData + i*dst->widthStep);
        for(int j=0;j<dst->width;j++)
        {
            if(flag==0)
                line2[j]=line1[img_rect.x+j];
            else
            {
                if( sqrt( pow((double)(i-img_rect.width/2),2) + pow((double)(j-img_rect.width/2),2) )<flag )
                    line2[j]=line1[img_rect.x+j];
                else
                    line2[j]=255;
            }
        }
    }
}

void vexTagAngle(IplImage*img_in,double &angle,CvPoint &point_1,CvPoint &point_2)
{
    IplImage*src=cvCloneImage(img_in);
    //cvDilate(src,src);
    //cvErode(src,src);

    CvPoint2D32f p[3];
    p[0]=cvPoint2D32f(0,0);
    p[1]=cvPoint2D32f(0,0);
    p[2]=cvPoint2D32f(0,0);

    //int points;
    byte**src_img=vexCreateImage(src->height,src->width);
    byte**dst_img=vexCreateImage(src->height,src->width);
    cvimageToimage(src,src_img,src->height,src->width);

    int max;

    //float angle;


    // calculate the center of each part
    for(int t=0;t<3;t++)
    {
        //points=0;
        int a_l=src->width,a_r=0,a_t=src->height,a_b=0;
        maxblob2(src_img,dst_img,&max,0,src->height,src->width);
        for(int i=0;i<src->height;i++)
        {
            for(int j=0;j<src->width;j++)
            {
                if(dst_img[i][j]==255)
                {
                    src_img[i][j]=0;
                    //p[t].x+=j;
                    //p[t].y+=i;
                    //points++;

                    if(j<a_l)
                        a_l=j;
                    if(j>a_r)
                        a_r=j;

                    if(i<a_t)
                        a_t=i;
                    if(i>a_b)
                        a_b=i;

                }
            }
        }

        p[t].x=(a_l+a_r)/2;
        p[t].y=(a_t+a_b)/2;

    }

    CvPoint mid=cvPoint( (p[0].x+p[1].x)/2,(p[0].y+p[1].y)/2 );


    point_1.x=p[0].x;
    point_1.y=p[0].y;
    point_2.x=p[1].x;
    point_2.y=p[1].y;

    //point_1.x=mid.x-

    //CvPoint mid=cvPoint(img_in->width/2,img_in->height/2);

    if(mid.x==p[2].x)
        p[2].x+=0.00001;


    if(p[2].x>mid.x)
        angle=-atan(1.0*(mid.y-p[2].y)/(p[2].x-mid.x))+3.14159*0.5;
    else
        angle=-atan(1.0*(mid.y-p[2].y)/(p[2].x-mid.x))-3.14159*0.5;

    vexReleaseImage(src_img,src->height,src->width);
    vexReleaseImage(dst_img,src->height,src->width);
    cvReleaseImage(&src);

}
void vexGoodImage(IplImage*src,IplImage*dst1,IplImage*dst2,double angle,CvPoint p1,CvPoint p2,int num_size)
{
    //int num_size=6;
    float factor=1.3;
    int l_offset_y=0;
    int l_offset_x=0;
    int r_offset_y=0;
    int r_offset_x=1;

    float m[6];

    m[0]=(float)cos(-angle);
    m[4]=m[0];
    m[1]=(float)sin(-angle);
    m[3]=-m[1];
    m[2]=src->width*0.5;
    m[5]=src->height*0.5;

    CvMat M=cvMat(2,3,CV_32F,m);

    IplImage*dst=cvCreateImage(cvSize(src->width,src->height),8,1);

    cvGetQuadrangleSubPix(src,dst,&M);
    CvPoint pt1,pt2;
    //x'=x-(width(dst)-1)*0.5, y'=y-(height(dst)-1)*0.5
    //dst(x, y)= src( A11x'+A12y'+b1, A21x'+A22y'+b2)
    int xbar=p1.x-(dst->width )*0.5;
    int ybar=(dst->height )*0.5-p1.y;
    pt1.x=m[0]*xbar+m[1]*ybar+m[2];
    pt1.y=m[5]-m[3]*xbar-m[4]*ybar;

    int xbar2=p2.x-(dst->width )*0.5;
    int ybar2=(dst->height )*0.5-p2.y;
    pt2.x=m[0]*xbar2+m[1]*ybar2+m[2];
    pt2.y=m[5]-m[3]*xbar2-m[4]*ybar2;

    CvRect num_rect1,num_rect2;

    if(pt1.x<pt2.x){
        num_rect1.x=pt1.x-num_size+l_offset_x;
        num_rect1.y=pt1.y-factor*num_size+l_offset_y;
        num_rect2.x=pt2.x-num_size+r_offset_x;
        num_rect2.y=pt2.y-factor*num_size+r_offset_y;
    }
    else{
        num_rect2.x=pt1.x-num_size+r_offset_x;
        num_rect2.y=pt1.y-factor*num_size+r_offset_y;
        num_rect1.x=pt2.x-num_size+l_offset_x;
        num_rect1.y=pt2.y-factor*num_size+l_offset_y;
    }
    num_rect1.width=num_size*2;
    num_rect1.height=num_size*factor*2;
    num_rect2.width=num_size*2;
    num_rect2.height=num_size*factor*2;

    if(num_rect1.x<0)
        num_rect1.x=0;
    if(num_rect1.y<0)
        num_rect1.y=0;
    if(num_rect2.x<0)
        num_rect2.x=0;
    if(num_rect2.y<0)
        num_rect2.y=0;

    if(num_rect1.x+num_rect1.width>src->width-1)
        num_rect1.width=src->width-1-num_rect1.x;
    if(num_rect1.y+num_rect1.height>src->height-1)
        num_rect1.height=src->height-1-num_rect1.y;
    if(num_rect2.x+num_rect2.width>src->width-1)
        num_rect2.width=src->width-1-num_rect2.x;
    if(num_rect2.y+num_rect2.height>src->height-1)
        num_rect2.height=src->height-1-num_rect2.y;


    IplImage*num1=cvCreateImage(cvSize(num_rect1.width,num_rect1.height),8,1);
    IplImage*num2=cvCreateImage(cvSize(num_rect1.width,num_rect1.height),8,1);

    //cvNamedWindow("num1_o",1);
    //cvNamedWindow("num2_o",1);

    cvSetImageROI(dst,num_rect1);
    cvResize(dst,num1);
    //cvShowImage("num1_o",num1);
    //#ifdef make_hough
    //cvSaveImage("num1_o.jpg",num1);
    //#endif
    cvResetImageROI(dst);
    cvSetImageROI(dst,num_rect2);
    cvResize(dst,num2);
    //cvShowImage("num2_o",num2);
    //#ifdef make_hough
    //cvSaveImage("num2_o.jpg",num2);
    //#endif

    cvEqualizeHist(num1,num1);
    cvEqualizeHist(num2,num2);

    IplImage *g_num1=vexMakeSquareImage(num1,255,20);
    IplImage *g_num2=vexMakeSquareImage(num2,255,20);

    if(dst1!=NULL){
        for(int i=0;i<dst1->height;i++){
            byte*line1=(byte*)dst1->imageData+i*dst1->widthStep;
            byte*line2=(byte*)g_num1->imageData+i*g_num1->widthStep;
            for(int j=0;j<dst1->width;j++){
                line1[j]=line2[j];

            }
        }
    }

    if(dst2!=NULL){
        for(int i=0;i<dst2->height;i++){
            byte*line1=(byte*)dst2->imageData+i*dst2->widthStep;
            byte*line2=(byte*)g_num2->imageData+i*g_num2->widthStep;
            for(int j=0;j<dst2->width;j++){
                line1[j]=line2[j];

            }
        }

    }

    //if(flag==1){
    //cvNamedWindow("hi",1);

    /*
        cvRectangle(dst,cvPoint(num_rect1.x,num_rect1.y),
                        cvPoint(num_rect1.x+num_rect1.width,num_rect1.y+num_rect1.height),
                    CV_RGB(255,255,255),1);
        cvRectangle(dst,cvPoint(num_rect2.x,num_rect2.y),
                        cvPoint(num_rect2.x+num_rect2.width,num_rect2.y+num_rect2.height),
                    CV_RGB(255,255,255),1);
        */
    //cvShowImage("hi",g_num1);
    //}
    cvReleaseImage(&g_num1);
    cvReleaseImage(&g_num2);

    cvReleaseImage(&num1);
    cvReleaseImage(&num2);
    cvReleaseImage(&dst);
}
void vexReleaseImage(byte **src,int height,int width)
{
    for(int i=0;i<height;i++)
        delete []src[i];
    delete []src;
}
void maxblob2(byte **src,byte **dst,int *max,unsigned int background,int height,int width)
{
    int direction[8][2]={{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
    int blobcolor;

    if(background==0)
        blobcolor=255;
    if(background==255)
        blobcolor=0;


    int **arr=new int*[height];
    for(int i=0;i<height;i++)
    {
        arr[i]=new int [width];

        for(int j=0;j<width;j++)
        {
            arr[i][j]=0;
            dst[i][j]=background;
        }
    }

    double cal=0;
    blob_node *first,* temp_ptr;// = new blob_node;
    blob_node * next;
    blob_node * cancel;
    bool end_flag = false, status_flag = false;
    int x=0,y=0,recoder=0;

    int vexx,vexy;
    while (end_flag == false){
        //ShowMessage("hi");
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(src[i][j]==blobcolor && arr[i][j]==0)
                {
                    arr[i][j] = 1;
                    first=new blob_node;
                    first->linker=NULL;
                    next = new blob_node;
                    next->linker = first;
                    next->row = i;
                    next->column = j;
                    next->dir = -1;
                    temp_ptr = next;
                    cal++;
                    while(status_flag == false)
                    {
                        temp_ptr->dir++ ;
                        if(temp_ptr->dir < 8&& temp_ptr->dir>-1){
                            vexx=(temp_ptr->row)+(direction[temp_ptr->dir][0]);
                            vexy=temp_ptr->column+( direction[temp_ptr->dir][1]);

                            if(vexx >= 0 && vexx < height && vexy >= 0 && vexy < width )
                            {
                                if( arr[vexx][vexy]==0&&src[vexx][vexy]==blobcolor){
                                    arr[ vexx][vexy]=1;
                                    cal++;
                                    next = new blob_node;
                                    next->linker = temp_ptr;
                                    next->row = vexx;
                                    next->column = vexy;
                                    next->dir = -1;
                                    temp_ptr = next;
                                }
                            }

                        }


                        else
                        {// if(temp_ptr->dir >= 8 ){
                            cancel = temp_ptr;
                            temp_ptr = temp_ptr->linker;
                            delete cancel;
                            if(temp_ptr == NULL){
                                if(cal>recoder){
                                    recoder=cal;
                                    x=i;
                                    y=j;
                                }
                                delete temp_ptr;
                                status_flag = true;
                                cal=0;
                            }
                        }
                    }//end while
                    status_flag = false;
                }
                if(i == height-1 && j == width-1 )
                {
                    end_flag = true;
                }
            }
        }
    }//end while
    *max=recoder;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            arr[i][j]=0;

    next = new blob_node;
    next->linker = NULL;
    next->row = x;
    next->column = y;
    next->dir = -1;
    temp_ptr = next;
    cal++;
    dst[x][y]=blobcolor;


    while(status_flag == false){
        temp_ptr->dir++ ;
        if(temp_ptr->dir < 8&& temp_ptr->dir>-1)
        {
            vexx=(temp_ptr->row)+(direction[temp_ptr->dir][0]);
            vexy=temp_ptr->column+( direction[temp_ptr->dir][1]);
            if(vexx >= 0 && vexx < height && vexy >= 0 && vexy < width)
            {
                if( arr[vexx][vexy]==0&& src[vexx][vexy]==blobcolor )
                {
                    arr[vexx][vexy]=1;
                    dst[vexx][vexy]=blobcolor;
                    cal++;
                    next = new blob_node;
                    next->linker = temp_ptr;
                    next->row = vexx;
                    next->column = vexy;
                    next->dir = -1;
                    temp_ptr = next;
                }
            }
        }

        else
        {// if(temp_ptr->dir >= 8 ){
            cancel = temp_ptr;
            temp_ptr = temp_ptr->linker;
            delete cancel;
            if(temp_ptr == NULL){
                status_flag = true;
                end_flag = true;
                delete temp_ptr;
                cal=0;
            }
        }
    }


    for(int i=0;i<height;i++)
    {
        delete []arr[i];
    }
    delete []arr;
    arr=NULL;
}
void cvimageToimage(IplImage *src,byte **dst,int height,int width)
{
    for(int i=0;i<height;i++){
        byte *line =  (byte*) (src->imageData+i*src->widthStep);
        for(int j=0; j< width;j++)
            dst[i][j]=line[j];
    }
}
IplImage* vexMakeSquareImage(IplImage*src,byte fill_color,int size)
{
    int temp_size;
    if(src->height>src->width)
        temp_size=src->height;
    else
        temp_size=src->width;

    IplImage*dst=cvCreateImage(cvSize(size,size),8,1);
    IplImage*temp_image=cvCreateImage(cvSize(temp_size,temp_size),8,1);
    if( src->height>src->width ){
        int left,right;
        left=(src->height - src->width)/2;
        right=left+src->width;

        for(int i=0;i<temp_image->height;i++){
            byte*line=(byte*)temp_image->imageData+i*temp_image->widthStep;
            byte*line2=(byte*)src->imageData+i*src->widthStep;
            for(int j=0;j<temp_image->width;j++){
                if(j>=left&&j<right){
                    line[j]=line2[j-left];
                }
                else
                    line[j]=fill_color;
            }
        }

    }
    else{
        int top,down;
        top=(src->width - src->height)/2;
        down=top+src->height;

        for(int i=0;i<temp_image->height;i++){
            byte*line=(byte*)temp_image->imageData+i*temp_image->widthStep;

            for(int j=0;j<temp_image->width;j++){
                if(i>=top&&i<down){
                    byte*line2=(byte*)src->imageData+(i-top)*src->widthStep;
                    line[j]=line2[j];
                }
                else
                    line[j]=fill_color;
            }
        }

    }


    cvResize(temp_image,dst);

    cvReleaseImage(&temp_image);
    return dst;
}

byte** vexCreateImage(int height,int width)
{
    byte **img=new byte*[height];
    for(int i=0;i<height;i++)
    {
        img[i]=new byte[width];
        for(int j=0;j<width;j++)
        {
            img[i][j]=0;
        }
    }

    return img;
}
//==================================================================
double vexPredict(IplImage*src, std::vector<svm_model*> model)
{
    //cvEqualizeHist(src,src);
    struct svm_node image_data[401];
    int num=0;
    for(int i=0;i<src->height;i++)
    {
        byte*line=(byte*)src->imageData+i*src->widthStep;
        for(int j=0;j<src->width;j++)
        {
            image_data[num].index=num+1;
            image_data[num].value=line[j];
            num++;
        }
    }
    image_data[400].index=-1;

    double guess;

    guess=svm_predict(model[17],image_data);
    guess=svm_predict(model[ (int)(guess) ],image_data);
    guess--;

    if(guess == -1) // transform z = 0 to z = 25
        guess = 25;

    if(guess == -2)
        guess = -1;

    return guess;
}



