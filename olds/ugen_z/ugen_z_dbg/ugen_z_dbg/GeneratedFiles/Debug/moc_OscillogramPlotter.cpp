/****************************************************************************
** Meta object code from reading C++ file 'OscillogramPlotter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../OscillogramPlotter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OscillogramPlotter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OscillogramPlotter_t {
    QByteArrayData data[18];
    char stringdata[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OscillogramPlotter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OscillogramPlotter_t qt_meta_stringdata_OscillogramPlotter = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 20),
QT_MOC_LITERAL(2, 40, 0),
QT_MOC_LITERAL(3, 41, 7),
QT_MOC_LITERAL(4, 49, 17),
QT_MOC_LITERAL(5, 67, 20),
QT_MOC_LITERAL(6, 88, 7),
QT_MOC_LITERAL(7, 96, 17),
QT_MOC_LITERAL(8, 114, 19),
QT_MOC_LITERAL(9, 134, 5),
QT_MOC_LITERAL(10, 140, 5),
QT_MOC_LITERAL(11, 146, 5),
QT_MOC_LITERAL(12, 152, 16),
QT_MOC_LITERAL(13, 169, 20),
QT_MOC_LITERAL(14, 190, 3),
QT_MOC_LITERAL(15, 194, 17),
QT_MOC_LITERAL(16, 212, 21),
QT_MOC_LITERAL(17, 234, 18)
    },
    "OscillogramPlotter\0BegOscIntermedChange\0"
    "\0new_beg\0BegOscFinalChange\0"
    "EndOscIntermedChange\0new_end\0"
    "EndOscFinalChange\0SetStrobIntermedPos\0"
    "strob\0begin\0porog\0SetStrobFinalPos\0"
    "SetStrobIntermedLeft\0len\0SetStrobFinalLeft\0"
    "SetStrobIntermedRight\0SetStrobFinalRight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OscillogramPlotter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    0,   67,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,
       7,    0,   71,    2, 0x06 /* Public */,
       8,    3,   72,    2, 0x06 /* Public */,
      12,    0,   79,    2, 0x06 /* Public */,
      13,    3,   80,    2, 0x06 /* Public */,
      15,    0,   87,    2, 0x06 /* Public */,
      16,    2,   88,    2, 0x06 /* Public */,
      17,    0,   93,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   14,
    QMetaType::Void,

       0        // eod
};

void OscillogramPlotter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OscillogramPlotter *_t = static_cast<OscillogramPlotter *>(_o);
        switch (_id) {
        case 0: _t->BegOscIntermedChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->BegOscFinalChange(); break;
        case 2: _t->EndOscIntermedChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->EndOscFinalChange(); break;
        case 4: _t->SetStrobIntermedPos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: _t->SetStrobFinalPos(); break;
        case 6: _t->SetStrobIntermedLeft((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->SetStrobFinalLeft(); break;
        case 8: _t->SetStrobIntermedRight((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->SetStrobFinalRight(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (OscillogramPlotter::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::BegOscIntermedChange)) {
                *result = 0;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::BegOscFinalChange)) {
                *result = 1;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::EndOscIntermedChange)) {
                *result = 2;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::EndOscFinalChange)) {
                *result = 3;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobIntermedPos)) {
                *result = 4;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobFinalPos)) {
                *result = 5;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobIntermedLeft)) {
                *result = 6;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobFinalLeft)) {
                *result = 7;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobIntermedRight)) {
                *result = 8;
            }
        }
        {
            typedef void (OscillogramPlotter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OscillogramPlotter::SetStrobFinalRight)) {
                *result = 9;
            }
        }
    }
}

const QMetaObject OscillogramPlotter::staticMetaObject = {
    { &PositionTesterPlotter::staticMetaObject, qt_meta_stringdata_OscillogramPlotter.data,
      qt_meta_data_OscillogramPlotter,  qt_static_metacall, 0, 0}
};


const QMetaObject *OscillogramPlotter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OscillogramPlotter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OscillogramPlotter.stringdata))
        return static_cast<void*>(const_cast< OscillogramPlotter*>(this));
    return PositionTesterPlotter::qt_metacast(_clname);
}

int OscillogramPlotter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PositionTesterPlotter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void OscillogramPlotter::BegOscIntermedChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OscillogramPlotter::BegOscFinalChange()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void OscillogramPlotter::EndOscIntermedChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OscillogramPlotter::EndOscFinalChange()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void OscillogramPlotter::SetStrobIntermedPos(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void OscillogramPlotter::SetStrobFinalPos()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void OscillogramPlotter::SetStrobIntermedLeft(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void OscillogramPlotter::SetStrobFinalLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void OscillogramPlotter::SetStrobIntermedRight(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void OscillogramPlotter::SetStrobFinalRight()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
