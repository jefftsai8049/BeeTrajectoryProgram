/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata[449];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 7),
QT_MOC_LITERAL(2, 19, 0),
QT_MOC_LITERAL(3, 20, 7),
QT_MOC_LITERAL(4, 28, 20),
QT_MOC_LITERAL(5, 49, 19),
QT_MOC_LITERAL(6, 69, 13),
QT_MOC_LITERAL(7, 83, 4),
QT_MOC_LITERAL(8, 88, 14),
QT_MOC_LITERAL(9, 103, 14),
QT_MOC_LITERAL(10, 118, 30),
QT_MOC_LITERAL(11, 149, 23),
QT_MOC_LITERAL(12, 173, 23),
QT_MOC_LITERAL(13, 197, 23),
QT_MOC_LITERAL(14, 221, 23),
QT_MOC_LITERAL(15, 245, 23),
QT_MOC_LITERAL(16, 269, 24),
QT_MOC_LITERAL(17, 294, 28),
QT_MOC_LITERAL(18, 323, 22),
QT_MOC_LITERAL(19, 346, 16),
QT_MOC_LITERAL(20, 363, 8),
QT_MOC_LITERAL(21, 372, 22),
QT_MOC_LITERAL(22, 395, 22),
QT_MOC_LITERAL(23, 418, 30)
    },
    "MainWindow\0Recslot\0\0Vidslot\0"
    "on_loadVideo_clicked\0on_runVideo_clicked\0"
    "on_update_img\0qimg\0on_update_img1\0"
    "on_update_img2\0on_pushButton_hcChange_clicked\0"
    "on_pushButton_2_clicked\0on_pushButton_6_clicked\0"
    "on_pushButton_7_clicked\0on_actionCap1_triggered\0"
    "on_actionCap2_triggered\0"
    "on_RawDataRecord_clicked\0"
    "on_RawDataRecordStop_clicked\0"
    "on_AnalyzeData_clicked\0show_file_finish\0"
    "filename\0on_CombineData_clicked\0"
    "on_Data_Divide_clicked\0"
    "on_individual_bee_info_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x0a /* Public */,
       3,    0,  115,    2, 0x0a /* Public */,
       4,    0,  116,    2, 0x08 /* Private */,
       5,    0,  117,    2, 0x08 /* Private */,
       6,    1,  118,    2, 0x08 /* Private */,
       8,    1,  121,    2, 0x08 /* Private */,
       9,    1,  124,    2, 0x08 /* Private */,
      10,    0,  127,    2, 0x08 /* Private */,
      11,    0,  128,    2, 0x08 /* Private */,
      12,    0,  129,    2, 0x08 /* Private */,
      13,    0,  130,    2, 0x08 /* Private */,
      14,    0,  131,    2, 0x08 /* Private */,
      15,    0,  132,    2, 0x08 /* Private */,
      16,    0,  133,    2, 0x08 /* Private */,
      17,    0,  134,    2, 0x08 /* Private */,
      18,    0,  135,    2, 0x08 /* Private */,
      19,    1,  136,    2, 0x08 /* Private */,
      21,    0,  139,    2, 0x08 /* Private */,
      22,    0,  140,    2, 0x08 /* Private */,
      23,    0,  141,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void, QMetaType::QImage,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->Recslot(); break;
        case 1: _t->Vidslot(); break;
        case 2: _t->on_loadVideo_clicked(); break;
        case 3: _t->on_runVideo_clicked(); break;
        case 4: _t->on_update_img((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 5: _t->on_update_img1((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 6: _t->on_update_img2((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 7: _t->on_pushButton_hcChange_clicked(); break;
        case 8: _t->on_pushButton_2_clicked(); break;
        case 9: _t->on_pushButton_6_clicked(); break;
        case 10: _t->on_pushButton_7_clicked(); break;
        case 11: _t->on_actionCap1_triggered(); break;
        case 12: _t->on_actionCap2_triggered(); break;
        case 13: _t->on_RawDataRecord_clicked(); break;
        case 14: _t->on_RawDataRecordStop_clicked(); break;
        case 15: _t->on_AnalyzeData_clicked(); break;
        case 16: _t->show_file_finish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->on_CombineData_clicked(); break;
        case 18: _t->on_Data_Divide_clicked(); break;
        case 19: _t->on_individual_bee_info_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
