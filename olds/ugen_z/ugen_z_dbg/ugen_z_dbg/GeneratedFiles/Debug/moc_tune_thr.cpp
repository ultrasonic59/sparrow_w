/****************************************************************************
** Meta object code from reading C++ file 'tune_thr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tune_thr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tune_thr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_c_tune_thr_t {
    QByteArrayData data[7];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_c_tune_thr_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_c_tune_thr_t qt_meta_stringdata_c_tune_thr = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 16),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 16),
QT_MOC_LITERAL(4, 46, 17),
QT_MOC_LITERAL(5, 64, 12),
QT_MOC_LITERAL(6, 77, 6)
    },
    "c_tune_thr\0signal_draw_osc0\0\0"
    "signal_draw_amps\0req_timer_timeout\0"
    "on_off_timer\0on_off"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_c_tune_thr[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void c_tune_thr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        c_tune_thr *_t = static_cast<c_tune_thr *>(_o);
        switch (_id) {
        case 0: _t->signal_draw_osc0(); break;
        case 1: _t->signal_draw_amps(); break;
        case 2: _t->req_timer_timeout(); break;
        case 3: _t->on_off_timer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (c_tune_thr::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&c_tune_thr::signal_draw_osc0)) {
                *result = 0;
            }
        }
        {
            typedef void (c_tune_thr::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&c_tune_thr::signal_draw_amps)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject c_tune_thr::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_c_tune_thr.data,
      qt_meta_data_c_tune_thr,  qt_static_metacall, 0, 0}
};


const QMetaObject *c_tune_thr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *c_tune_thr::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_c_tune_thr.stringdata))
        return static_cast<void*>(const_cast< c_tune_thr*>(this));
    return QObject::qt_metacast(_clname);
}

int c_tune_thr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void c_tune_thr::signal_draw_osc0()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void c_tune_thr::signal_draw_amps()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
