/****************************************************************************
** Meta object code from reading C++ file 'window_header.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/window_header.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window_header.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WindowHeader_t {
    QByteArrayData data[14];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WindowHeader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WindowHeader_t qt_meta_stringdata_WindowHeader = {
    {
QT_MOC_LITERAL(0, 0, 12), // "WindowHeader"
QT_MOC_LITERAL(1, 13, 20), // "onWindowStateChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 21), // "onWindowHeaderChanged"
QT_MOC_LITERAL(4, 57, 29), // "onWindowHeaderPositionChanged"
QT_MOC_LITERAL(5, 87, 17), // "onMaximizeClicked"
QT_MOC_LITERAL(6, 105, 17), // "onMinimizeClicked"
QT_MOC_LITERAL(7, 123, 7), // "Buttons"
QT_MOC_LITERAL(8, 131, 6), // "Button"
QT_MOC_LITERAL(9, 138, 9), // "NoButtons"
QT_MOC_LITERAL(10, 148, 14), // "MinimizeButton"
QT_MOC_LITERAL(11, 163, 14), // "MaximizeButton"
QT_MOC_LITERAL(12, 178, 11), // "CloseButton"
QT_MOC_LITERAL(13, 190, 10) // "AllButtons"

    },
    "WindowHeader\0onWindowStateChanged\0\0"
    "onWindowHeaderChanged\0"
    "onWindowHeaderPositionChanged\0"
    "onMaximizeClicked\0onMinimizeClicked\0"
    "Buttons\0Button\0NoButtons\0MinimizeButton\0"
    "MaximizeButton\0CloseButton\0AllButtons"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WindowHeader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       1,   48, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       6,    1,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // enums: name, alias, flags, count, data
       7,    8, 0x1,    5,   53,

 // enum data: key, value
       9, uint(WindowHeader::NoButtons),
      10, uint(WindowHeader::MinimizeButton),
      11, uint(WindowHeader::MaximizeButton),
      12, uint(WindowHeader::CloseButton),
      13, uint(WindowHeader::AllButtons),

       0        // eod
};

void WindowHeader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WindowHeader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onWindowStateChanged(); break;
        case 1: _t->onWindowHeaderChanged(); break;
        case 2: _t->onWindowHeaderPositionChanged(); break;
        case 3: _t->onMaximizeClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onMinimizeClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WindowHeader::staticMetaObject = { {
    &QFrame::staticMetaObject,
    qt_meta_stringdata_WindowHeader.data,
    qt_meta_data_WindowHeader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WindowHeader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WindowHeader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WindowHeader.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int WindowHeader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
