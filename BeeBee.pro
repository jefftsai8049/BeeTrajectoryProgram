#-------------------------------------------------
#
# Project created by QtCreator 2012-04-17T20:26:32
#
#-------------------------------------------------

QT       += core gui\
            widgets

TARGET = BeeBee
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dataanalysis.cpp \
    svm.cpp \
    imgP_bf1.cpp \
    videothread.cpp \
    helper.cpp \
    colormap.cpp

HEADERS  += mainwindow.h \
    dataanalysis.h \
    svm.h \
    imgP_bf1.h \
    videothread.h \
    helper.hpp \
    colormap.hpp


FORMS    += mainwindow.ui \
    mainwindow.ui

TBB_Lib = C:/OpenCV240/lib
OpenCV_Libd = C:/OpenCV240/lib

OpenCV_Lib = D:/libraries/opencv249/lib

INCLUDEPATH += D:/libraries/opencv249/include\
               D:/libraries/opencv249/include/opencv2\


Release: LIBS += $$OpenCV_Lib/opencv_calib3d249.lib\
$$OpenCV_Lib/opencv_contrib249.lib\
$$OpenCV_Lib/opencv_core249.lib\
$$OpenCV_Lib/opencv_features2d249.lib\
$$OpenCV_Lib/opencv_flann249.lib\
$$OpenCV_Lib/opencv_gpu249.lib\
$$OpenCV_Lib/opencv_highgui249.lib\
$$OpenCV_Lib/opencv_imgproc249.lib\
$$OpenCV_Lib/opencv_legacy249.lib\
$$OpenCV_Lib/opencv_ml249.lib\
$$OpenCV_Lib/opencv_nonfree249.lib\
$$OpenCV_Lib/opencv_objdetect249.lib\
$$OpenCV_Lib/opencv_ocl249.lib\
$$OpenCV_Lib/opencv_photo249.lib\
$$OpenCV_Lib/opencv_stitching249.lib\
$$OpenCV_Lib/opencv_superres249.lib\
$$OpenCV_Lib/opencv_ts249.lib\
$$OpenCV_Lib/opencv_video249.lib\
$$OpenCV_Lib/opencv_videostab249.lib\
$$OpenCV_Lib/tbb.lib
