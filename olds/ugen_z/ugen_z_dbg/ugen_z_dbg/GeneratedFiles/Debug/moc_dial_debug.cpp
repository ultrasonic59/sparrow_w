/****************************************************************************
** Meta object code from reading C++ file 'dial_debug.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dial_debug.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dial_debug.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DialDebug_t {
    QByteArrayData data[18];
    char stringdata[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DialDebug_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DialDebug_t qt_meta_stringdata_DialDebug = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 10),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 14),
QT_MOC_LITERAL(4, 37, 10),
QT_MOC_LITERAL(5, 48, 10),
QT_MOC_LITERAL(6, 59, 10),
QT_MOC_LITERAL(7, 70, 10),
QT_MOC_LITERAL(8, 81, 10),
QT_MOC_LITERAL(9, 92, 10),
QT_MOC_LITERAL(10, 103, 10),
QT_MOC_LITERAL(11, 114, 11),
QT_MOC_LITERAL(12, 126, 11),
QT_MOC_LITERAL(13, 138, 25),
QT_MOC_LITERAL(14, 164, 10),
QT_MOC_LITERAL(15, 175, 10),
QT_MOC_LITERAL(16, 186, 12),
QT_MOC_LITERAL(17, 199, 12)
    },
    "DialDebug\0req_rd_xil\0\0alt_dat_req_t*\0"
    "req_wr_xil\0req_rd_stm\0req_wr_stm\0"
    "req_rd_msp\0req_wr_msp\0req_rd_udb\0"
    "req_wr_udb\0req_rd_upow\0req_wr_upow\0"
    "signal_send_udp_test_dats\0on_butt_rd\0"
    "on_butt_wr\0on_butt_send\0req_data_rdy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DialDebug[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    1,   92,    2, 0x06 /* Public */,
       5,    1,   95,    2, 0x06 /* Public */,
       6,    1,   98,    2, 0x06 /* Public */,
       7,    1,  101,    2, 0x06 /* Public */,
       8,    1,  104,    2, 0x06 /* Public */,
       9,    1,  107,    2, 0x06 /* Public */,
      10,    1,  110,    2, 0x06 /* Public */,
      11,    1,  113,    2, 0x06 /* Public */,
      12,    1,  116,    2, 0x06 /* Public */,
      13,    1,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  122,    2, 0x08 /* Private */,
      15,    0,  123,    2, 0x08 /* Private */,
      16,    0,  124,    2, 0x08 /* Private */,
      17,    1,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void DialDebug::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DialDebug *_t = static_cast<DialDebug *>(_o);
        switch (_id) {
        case 0: _t->req_rd_xil((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 1: _t->req_wr_xil((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 2: _t->req_rd_stm((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 3: _t->req_wr_stm((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 4: _t->req_rd_msp((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->req_wr_msp((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->req_rd_udb((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 7: _t->req_wr_udb((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 8: _t->req_rd_upow((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 9: _t->req_wr_upow((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        case 10: _t->signal_send_udp_test_dats((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 11: _t->on_butt_rd(); break;
        case 12: _t->on_butt_wr(); break;
        case 13: _t->on_butt_send(); break;
        case 14: _t->req_data_rdy((*reinterpret_cast< alt_dat_req_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_rd_xil)) {
                *result = 0;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_wr_xil)) {
                *result = 1;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_rd_stm)) {
                *result = 2;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_wr_stm)) {
                *result = 3;
            }
        }
        {
            typedef void (DialDebug::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_rd_msp)) {
                *result = 4;
            }
        }
        {
            typedef void (DialDebug::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_wr_msp)) {
                *result = 5;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_rd_udb)) {
                *result = 6;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_wr_udb)) {
                *result = 7;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_rd_upow)) {
                *result = 8;
            }
        }
        {
            typedef void (DialDebug::*_t)(alt_dat_req_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::req_wr_upow)) {
                *result = 9;
            }
        }
        {
            typedef void (DialDebug::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialDebug::signal_send_udp_test_dats)) {
                *result = 10;
            }
        }
    }
}

const QMetaObject DialDebug::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialDebug.data,
      qt_meta_data_DialDebug,  qt_static_metacall, 0, 0}
};


const QMetaObject *DialDebug::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DialDebug::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialDebug.stringdata))
        return static_cast<void*>(const_cast< DialDebug*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialDebug::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void DialDebug::req_rd_xil(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DialDebug::req_wr_xil(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DialDebug::req_rd_stm(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DialDebug::req_wr_stm(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DialDebug::req_rd_msp(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DialDebug::req_wr_msp(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DialDebug::req_rd_udb(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void DialDebug::req_wr_udb(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void DialDebug::req_rd_upow(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void DialDebug::req_wr_upow(alt_dat_req_t * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void DialDebug::signal_send_udp_test_dats(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_END_MOC_NAMESPACE
