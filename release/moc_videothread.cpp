/****************************************************************************
** Meta object code from reading C++ file 'videothread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../videothread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videothread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VideoThread_t {
    QByteArrayData data[13];
    char stringdata[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoThread_t qt_meta_stringdata_VideoThread = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 12),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 3),
QT_MOC_LITERAL(4, 30, 13),
QT_MOC_LITERAL(5, 44, 13),
QT_MOC_LITERAL(6, 58, 10),
QT_MOC_LITERAL(7, 69, 7),
QT_MOC_LITERAL(8, 77, 6),
QT_MOC_LITERAL(9, 84, 7),
QT_MOC_LITERAL(10, 92, 3),
QT_MOC_LITERAL(11, 96, 10),
QT_MOC_LITERAL(12, 107, 8)
    },
    "VideoThread\0sendNewImage\0\0img\0"
    "sendNewImage1\0sendNewImage2\0sendTagMes\0"
    "tag_num\0dir_in\0sendMes\0msg\0sendFinish\0"
    "filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       5,    1,   50,    2, 0x06 /* Public */,
       6,    2,   53,    2, 0x06 /* Public */,
       9,    1,   58,    2, 0x06 /* Public */,
      11,    1,   61,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void VideoThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoThread *_t = static_cast<VideoThread *>(_o);
        switch (_id) {
        case 0: _t->sendNewImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->sendNewImage1((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->sendNewImage2((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 3: _t->sendTagMes((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->sendMes((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sendFinish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (VideoThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendNewImage)) {
                *result = 0;
            }
        }
        {
            typedef void (VideoThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendNewImage1)) {
                *result = 1;
            }
        }
        {
            typedef void (VideoThread::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendNewImage2)) {
                *result = 2;
            }
        }
        {
            typedef void (VideoThread::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendTagMes)) {
                *result = 3;
            }
        }
        {
            typedef void (VideoThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendMes)) {
                *result = 4;
            }
        }
        {
            typedef void (VideoThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&VideoThread::sendFinish)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject VideoThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_VideoThread.data,
      qt_meta_data_VideoThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *VideoThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoThread.stringdata))
        return static_cast<void*>(const_cast< VideoThread*>(this));
    return QThread::qt_metacast(_clname);
}

int VideoThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void VideoThread::sendNewImage(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoThread::sendNewImage1(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoThread::sendNewImage2(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoThread::sendTagMes(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoThread::sendMes(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VideoThread::sendFinish(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
