/****************************************************************************
** Meta object code from reading C++ file 'round_progress_widget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/round_progress_widget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'round_progress_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RoundProgressIndicator_t {
    QByteArrayData data[23];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoundProgressIndicator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoundProgressIndicator_t qt_meta_stringdata_RoundProgressIndicator = {
    {
QT_MOC_LITERAL(0, 0, 22), // "RoundProgressIndicator"
QT_MOC_LITERAL(1, 23, 13), // "buttonClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 4), // "role"
QT_MOC_LITERAL(4, 43, 11), // "sizeChanged"
QT_MOC_LITERAL(5, 55, 13), // "setBackground"
QT_MOC_LITERAL(6, 69, 5), // "color"
QT_MOC_LITERAL(7, 75, 8), // "setColor"
QT_MOC_LITERAL(8, 84, 14), // "setButtonColor"
QT_MOC_LITERAL(9, 99, 13), // "setButtonSize"
QT_MOC_LITERAL(10, 113, 4), // "size"
QT_MOC_LITERAL(11, 118, 17), // "setIndicatorWidth"
QT_MOC_LITERAL(12, 136, 5), // "width"
QT_MOC_LITERAL(13, 142, 13), // "setCrossWidth"
QT_MOC_LITERAL(14, 156, 9), // "onTimeout"
QT_MOC_LITERAL(15, 166, 10), // "background"
QT_MOC_LITERAL(16, 177, 11), // "buttonColor"
QT_MOC_LITERAL(17, 189, 10), // "buttonSize"
QT_MOC_LITERAL(18, 200, 14), // "indicatorWidth"
QT_MOC_LITERAL(19, 215, 10), // "crossWidth"
QT_MOC_LITERAL(20, 226, 11), // "buttonStyle"
QT_MOC_LITERAL(21, 238, 10), // "buttonRole"
QT_MOC_LITERAL(22, 249, 5) // "style"

    },
    "RoundProgressIndicator\0buttonClicked\0"
    "\0role\0sizeChanged\0setBackground\0color\0"
    "setColor\0setButtonColor\0setButtonSize\0"
    "size\0setIndicatorWidth\0width\0setCrossWidth\0"
    "onTimeout\0background\0buttonColor\0"
    "buttonSize\0indicatorWidth\0crossWidth\0"
    "buttonStyle\0buttonRole\0style"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoundProgressIndicator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       9,  106, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   78,    2, 0x0a /* Public */,
       5,    1,   81,    2, 0x0a /* Public */,
       7,    1,   84,    2, 0x0a /* Public */,
       7,    1,   87,    2, 0x0a /* Public */,
       8,    1,   90,    2, 0x0a /* Public */,
       8,    1,   93,    2, 0x0a /* Public */,
       9,    1,   96,    2, 0x0a /* Public */,
      11,    1,   99,    2, 0x0a /* Public */,
      13,    1,  102,    2, 0x0a /* Public */,
      14,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QColor,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QColor,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QReal,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::QColor, 0x00095103,
      15, QMetaType::QColor, 0x00095103,
      16, QMetaType::QColor, 0x00095103,
      17, QMetaType::QReal, 0x00095103,
      18, QMetaType::Int, 0x00095103,
      19, QMetaType::Int, 0x00095103,
      20, QMetaType::QString, 0x00095103,
      21, QMetaType::QString, 0x00095103,
      22, QMetaType::QString, 0x00095103,

       0        // eod
};

void RoundProgressIndicator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RoundProgressIndicator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->buttonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sizeChanged(); break;
        case 2: _t->setBackground((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: _t->setBackground((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 5: _t->setColor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->setButtonColor((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 7: _t->setButtonColor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setButtonSize((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 9: _t->setIndicatorWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setCrossWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RoundProgressIndicator::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressIndicator::buttonClicked)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RoundProgressIndicator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressIndicator::sizeChanged)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RoundProgressIndicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = _t->background(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = _t->buttonColor(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = _t->buttonSize(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->indicatorWidth(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->crossWidth(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->buttonStyleStr(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->buttonRoleStr(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->styleStr(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RoundProgressIndicator *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: _t->setBackground(*reinterpret_cast< QColor*>(_v)); break;
        case 2: _t->setButtonColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: _t->setButtonSize(*reinterpret_cast< qreal*>(_v)); break;
        case 4: _t->setIndicatorWidth(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setCrossWidth(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setButtonStyle(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setButtonRole(*reinterpret_cast< QString*>(_v)); break;
        case 8: _t->setStyle(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject RoundProgressIndicator::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_RoundProgressIndicator.data,
    qt_meta_data_RoundProgressIndicator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RoundProgressIndicator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoundProgressIndicator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RoundProgressIndicator.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RoundProgressIndicator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RoundProgressIndicator::buttonClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RoundProgressIndicator::sizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_RoundProgressWidget_t {
    QByteArrayData data[15];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoundProgressWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoundProgressWidget_t qt_meta_stringdata_RoundProgressWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "RoundProgressWidget"
QT_MOC_LITERAL(1, 20, 12), // "valueChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "value"
QT_MOC_LITERAL(4, 40, 8), // "finished"
QT_MOC_LITERAL(5, 49, 4), // "role"
QT_MOC_LITERAL(6, 54, 20), // "titlePositionChanged"
QT_MOC_LITERAL(7, 75, 8), // "setValue"
QT_MOC_LITERAL(8, 84, 5), // "reset"
QT_MOC_LITERAL(9, 90, 16), // "setTitlePosition"
QT_MOC_LITERAL(10, 107, 13), // "TitlePosition"
QT_MOC_LITERAL(11, 121, 3), // "pos"
QT_MOC_LITERAL(12, 125, 19), // "setTopTitlePosition"
QT_MOC_LITERAL(13, 145, 5), // "isTop"
QT_MOC_LITERAL(14, 151, 16) // "topTitlePosition"

    },
    "RoundProgressWidget\0valueChanged\0\0"
    "value\0finished\0role\0titlePositionChanged\0"
    "setValue\0reset\0setTitlePosition\0"
    "TitlePosition\0pos\0setTopTitlePosition\0"
    "isTop\0topTitlePosition"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoundProgressWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       1,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    0,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   56,    2, 0x0a /* Public */,
       8,    0,   59,    2, 0x0a /* Public */,
       9,    1,   60,    2, 0x0a /* Public */,
      12,    1,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Bool,   13,

 // properties: name, type, flags
      14, QMetaType::Bool, 0x00495103,

 // properties: notify_signal_id
       2,

       0        // eod
};

void RoundProgressWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RoundProgressWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->titlePositionChanged(); break;
        case 3: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->reset(); break;
        case 5: _t->setTitlePosition((*reinterpret_cast< TitlePosition(*)>(_a[1]))); break;
        case 6: _t->setTopTitlePosition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RoundProgressWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressWidget::valueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RoundProgressWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressWidget::finished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RoundProgressWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressWidget::titlePositionChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RoundProgressWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isTopTitlePosition(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RoundProgressWidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTopTitlePosition(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject RoundProgressWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_RoundProgressWidget.data,
    qt_meta_data_RoundProgressWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RoundProgressWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoundProgressWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RoundProgressWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RoundProgressWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RoundProgressWidget::valueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RoundProgressWidget::finished(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RoundProgressWidget::titlePositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_RoundProgressDialog_t {
    QByteArrayData data[13];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RoundProgressDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RoundProgressDialog_t qt_meta_stringdata_RoundProgressDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "RoundProgressDialog"
QT_MOC_LITERAL(1, 20, 12), // "valueChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "value"
QT_MOC_LITERAL(4, 40, 13), // "setBackground"
QT_MOC_LITERAL(5, 54, 5), // "color"
QT_MOC_LITERAL(6, 60, 15), // "setBorderRadius"
QT_MOC_LITERAL(7, 76, 6), // "radius"
QT_MOC_LITERAL(8, 83, 8), // "setValue"
QT_MOC_LITERAL(9, 92, 10), // "onFinished"
QT_MOC_LITERAL(10, 103, 4), // "role"
QT_MOC_LITERAL(11, 108, 10), // "background"
QT_MOC_LITERAL(12, 119, 12) // "borderRadius"

    },
    "RoundProgressDialog\0valueChanged\0\0"
    "value\0setBackground\0color\0setBorderRadius\0"
    "radius\0setValue\0onFinished\0role\0"
    "background\0borderRadius"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RoundProgressDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       2,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x0a /* Public */,
       4,    1,   50,    2, 0x0a /* Public */,
       6,    1,   53,    2, 0x0a /* Public */,
       8,    1,   56,    2, 0x0a /* Public */,
       9,    1,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,   10,

 // properties: name, type, flags
      11, QMetaType::QColor, 0x00095103,
      12, QMetaType::Int, 0x00095103,

       0        // eod
};

void RoundProgressDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RoundProgressDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setBackground((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: _t->setBackground((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->setBorderRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onFinished((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RoundProgressDialog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RoundProgressDialog::valueChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RoundProgressDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = _t->background(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->borderRadius(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RoundProgressDialog *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBackground(*reinterpret_cast< QColor*>(_v)); break;
        case 1: _t->setBorderRadius(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject RoundProgressDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_RoundProgressDialog.data,
    qt_meta_data_RoundProgressDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RoundProgressDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RoundProgressDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RoundProgressDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RoundProgressDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void RoundProgressDialog::valueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
