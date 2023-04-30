/****************************************************************************
** Meta object code from reading C++ file 'serverlistener.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../serverlistener.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverlistener.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ServerListener_t {
    QByteArrayData data[17];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ServerListener_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ServerListener_t qt_meta_stringdata_ServerListener = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ServerListener"
QT_MOC_LITERAL(1, 15, 12), // "errorChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "ErrorCodes"
QT_MOC_LITERAL(4, 40, 4), // "code"
QT_MOC_LITERAL(5, 45, 11), // "errorString"
QT_MOC_LITERAL(6, 57, 9), // "connected"
QT_MOC_LITERAL(7, 67, 12), // "disconnected"
QT_MOC_LITERAL(8, 80, 23), // "tournamentUptimeChanged"
QT_MOC_LITERAL(9, 104, 6), // "uptime"
QT_MOC_LITERAL(10, 111, 15), // "playerConnected"
QT_MOC_LITERAL(11, 127, 6), // "player"
QT_MOC_LITERAL(12, 134, 11), // "playerMoved"
QT_MOC_LITERAL(13, 146, 18), // "broadcastForServer"
QT_MOC_LITERAL(14, 165, 17), // "intialiseListener"
QT_MOC_LITERAL(15, 183, 22), // "setUpServerConnections"
QT_MOC_LITERAL(16, 206, 12) // "dataRecieved"

    },
    "ServerListener\0errorChanged\0\0ErrorCodes\0"
    "code\0errorString\0connected\0disconnected\0"
    "tournamentUptimeChanged\0uptime\0"
    "playerConnected\0player\0playerMoved\0"
    "broadcastForServer\0intialiseListener\0"
    "setUpServerConnections\0dataRecieved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServerListener[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       6,    0,   69,    2, 0x06 /* Public */,
       7,    0,   70,    2, 0x06 /* Public */,
       8,    1,   71,    2, 0x06 /* Public */,
      10,    1,   74,    2, 0x06 /* Public */,
      12,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   80,    2, 0x09 /* Protected */,
      14,    0,   81,    2, 0x09 /* Protected */,
      15,    0,   82,    2, 0x09 /* Protected */,
      16,    0,   83,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ServerListener::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ServerListener *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorChanged((*reinterpret_cast< ErrorCodes(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        case 3: _t->tournamentUptimeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->playerConnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->playerMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->broadcastForServer(); break;
        case 7: _t->intialiseListener(); break;
        case 8: _t->setUpServerConnections(); break;
        case 9: _t->dataRecieved(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ServerListener::*)(ErrorCodes , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::errorChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ServerListener::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::connected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ServerListener::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::disconnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ServerListener::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::tournamentUptimeChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ServerListener::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::playerConnected)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ServerListener::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerListener::playerMoved)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ServerListener::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ServerListener.data,
    qt_meta_data_ServerListener,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ServerListener::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerListener::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ServerListener.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ServerListener::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void ServerListener::errorChanged(ErrorCodes _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ServerListener::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ServerListener::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ServerListener::tournamentUptimeChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ServerListener::playerConnected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ServerListener::playerMoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
