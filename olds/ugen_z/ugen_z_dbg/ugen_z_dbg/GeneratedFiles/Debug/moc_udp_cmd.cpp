/****************************************************************************
** Meta object code from reading C++ file 'udp_cmd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../udp_cmd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udp_cmd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_c_udp_cmd_t {
    QByteArrayData data[11];
    char stringdata[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_c_udp_cmd_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_c_udp_cmd_t qt_meta_stringdata_c_udp_cmd = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 11),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 28),
QT_MOC_LITERAL(4, 52, 3),
QT_MOC_LITERAL(5, 56, 13),
QT_MOC_LITERAL(6, 70, 13),
QT_MOC_LITERAL(7, 84, 3),
QT_MOC_LITERAL(8, 88, 13),
QT_MOC_LITERAL(9, 102, 14),
QT_MOC_LITERAL(10, 117, 4)
    },
    "c_udp_cmd\0socketError\0\0"
    "QAbstractSocket::SocketError\0err\0"
    "udp_ready_dat\0write_udp_cmd\0tba\0"
    "write_udp_dat\0write_udp_dats\0bind"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_c_udp_cmd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    1,   48,    2, 0x0a /* Public */,
       8,    1,   51,    2, 0x0a /* Public */,
       9,    1,   54,    2, 0x0a /* Public */,
      10,    0,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void,

       0        // eod
};

void c_udp_cmd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        c_udp_cmd *_t = static_cast<c_udp_cmd *>(_o);
        switch (_id) {
        case 0: _t->socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 1: _t->udp_ready_dat(); break;
        case 2: _t->write_udp_cmd((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->write_udp_dat((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->write_udp_dats((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->bind(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject c_udp_cmd::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_c_udp_cmd.data,
      qt_meta_data_c_udp_cmd,  qt_static_metacall, 0, 0}
};


const QMetaObject *c_udp_cmd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *c_udp_cmd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_c_udp_cmd.stringdata))
        return static_cast<void*>(const_cast< c_udp_cmd*>(this));
    return QObject::qt_metacast(_clname);
}

int c_udp_cmd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
