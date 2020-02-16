/****************************************************************************
** Meta object code from reading C++ file 'blocks_graphics_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/blocks_graphics_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'blocks_graphics_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AuxItem_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuxItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuxItem_t qt_meta_stringdata_AuxItem = {
    {
QT_MOC_LITERAL(0, 0, 7) // "AuxItem"

    },
    "AuxItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuxItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AuxItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject AuxItem::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_AuxItem.data,
    qt_meta_data_AuxItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AuxItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuxItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuxItem.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(this);
    if (!strcmp(_clname, "org.qt-project.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(this);
    return QObject::qt_metacast(_clname);
}

int AuxItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_BackgroundItem_t {
    QByteArrayData data[9];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BackgroundItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BackgroundItem_t qt_meta_stringdata_BackgroundItem = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BackgroundItem"
QT_MOC_LITERAL(1, 15, 15), // "bookmarkChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 6), // "size_t"
QT_MOC_LITERAL(4, 39, 5), // "index"
QT_MOC_LITERAL(5, 45, 5), // "moved"
QT_MOC_LITERAL(6, 51, 25), // "onWindowActivationChanged"
QT_MOC_LITERAL(7, 77, 14), // "isActiveWindow"
QT_MOC_LITERAL(8, 92, 13) // "onIdleTimeout"

    },
    "BackgroundItem\0bookmarkChanged\0\0size_t\0"
    "index\0moved\0onWindowActivationChanged\0"
    "isActiveWindow\0onIdleTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BackgroundItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   38,    2, 0x0a /* Public */,
       8,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,

       0        // eod
};

void BackgroundItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BackgroundItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->bookmarkChanged((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 1: _t->moved(); break;
        case 2: _t->onWindowActivationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onIdleTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BackgroundItem::*)(size_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackgroundItem::bookmarkChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BackgroundItem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BackgroundItem::moved)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BackgroundItem::staticMetaObject = { {
    &AuxItem::staticMetaObject,
    qt_meta_stringdata_BackgroundItem.data,
    qt_meta_data_BackgroundItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BackgroundItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackgroundItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BackgroundItem.stringdata0))
        return static_cast<void*>(this);
    return AuxItem::qt_metacast(_clname);
}

int BackgroundItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AuxItem::qt_metacall(_c, _id, _a);
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
void BackgroundItem::bookmarkChanged(size_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BackgroundItem::moved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_ForegroundItem_t {
    QByteArrayData data[6];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ForegroundItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ForegroundItem_t qt_meta_stringdata_ForegroundItem = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ForegroundItem"
QT_MOC_LITERAL(1, 15, 17), // "onBookmarkChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 6), // "size_t"
QT_MOC_LITERAL(4, 41, 5), // "index"
QT_MOC_LITERAL(5, 47, 7) // "onMoved"

    },
    "ForegroundItem\0onBookmarkChanged\0\0"
    "size_t\0index\0onMoved"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ForegroundItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       5,    0,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void ForegroundItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ForegroundItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onBookmarkChanged((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 1: _t->onMoved(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ForegroundItem::staticMetaObject = { {
    &AuxItem::staticMetaObject,
    qt_meta_stringdata_ForegroundItem.data,
    qt_meta_data_ForegroundItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ForegroundItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ForegroundItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ForegroundItem.stringdata0))
        return static_cast<void*>(this);
    return AuxItem::qt_metacast(_clname);
}

int ForegroundItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AuxItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_BlocksGraphicsView_t {
    QByteArrayData data[33];
    char stringdata0[549];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BlocksGraphicsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BlocksGraphicsView_t qt_meta_stringdata_BlocksGraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 18), // "BlocksGraphicsView"
QT_MOC_LITERAL(1, 19, 12), // "sceneUpdated"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 11), // "treeChanged"
QT_MOC_LITERAL(4, 45, 15), // "intervalChanged"
QT_MOC_LITERAL(5, 61, 32), // "const::profiler_gui::TreeBlocks&"
QT_MOC_LITERAL(6, 94, 7), // "_blocks"
QT_MOC_LITERAL(7, 102, 23), // "::profiler::timestamp_t"
QT_MOC_LITERAL(8, 126, 19), // "_session_begin_time"
QT_MOC_LITERAL(9, 146, 5), // "_left"
QT_MOC_LITERAL(10, 152, 6), // "_right"
QT_MOC_LITERAL(11, 159, 7), // "_strict"
QT_MOC_LITERAL(12, 167, 25), // "onWindowActivationChanged"
QT_MOC_LITERAL(13, 193, 21), // "repaintHistogramImage"
QT_MOC_LITERAL(14, 215, 12), // "repaintScene"
QT_MOC_LITERAL(15, 228, 24), // "onGraphicsScrollbarWheel"
QT_MOC_LITERAL(16, 253, 9), // "_scenePos"
QT_MOC_LITERAL(17, 263, 11), // "_wheelDelta"
QT_MOC_LITERAL(18, 275, 22), // "onScrollbarValueChange"
QT_MOC_LITERAL(19, 298, 30), // "onGraphicsScrollbarValueChange"
QT_MOC_LITERAL(20, 329, 16), // "onFlickerTimeout"
QT_MOC_LITERAL(21, 346, 13), // "onIdleTimeout"
QT_MOC_LITERAL(22, 360, 21), // "onHierarchyFlagChange"
QT_MOC_LITERAL(23, 382, 6), // "_value"
QT_MOC_LITERAL(24, 389, 22), // "onSelectedThreadChange"
QT_MOC_LITERAL(25, 412, 23), // "::profiler::thread_id_t"
QT_MOC_LITERAL(26, 436, 2), // "id"
QT_MOC_LITERAL(27, 439, 21), // "onSelectedBlockChange"
QT_MOC_LITERAL(28, 461, 12), // "_block_index"
QT_MOC_LITERAL(29, 474, 17), // "onRefreshRequired"
QT_MOC_LITERAL(30, 492, 19), // "onThreadViewChanged"
QT_MOC_LITERAL(31, 512, 15), // "onZoomSelection"
QT_MOC_LITERAL(32, 528, 20) // "onInspectCurrentView"

    },
    "BlocksGraphicsView\0sceneUpdated\0\0"
    "treeChanged\0intervalChanged\0"
    "const::profiler_gui::TreeBlocks&\0"
    "_blocks\0::profiler::timestamp_t\0"
    "_session_begin_time\0_left\0_right\0"
    "_strict\0onWindowActivationChanged\0"
    "repaintHistogramImage\0repaintScene\0"
    "onGraphicsScrollbarWheel\0_scenePos\0"
    "_wheelDelta\0onScrollbarValueChange\0"
    "onGraphicsScrollbarValueChange\0"
    "onFlickerTimeout\0onIdleTimeout\0"
    "onHierarchyFlagChange\0_value\0"
    "onSelectedThreadChange\0::profiler::thread_id_t\0"
    "id\0onSelectedBlockChange\0_block_index\0"
    "onRefreshRequired\0onThreadViewChanged\0"
    "onZoomSelection\0onInspectCurrentView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BlocksGraphicsView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    0,  105,    2, 0x06 /* Public */,
       4,    5,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  117,    2, 0x0a /* Public */,
      13,    0,  118,    2, 0x0a /* Public */,
      14,    0,  119,    2, 0x08 /* Private */,
      15,    2,  120,    2, 0x08 /* Private */,
      18,    1,  125,    2, 0x08 /* Private */,
      19,    1,  128,    2, 0x08 /* Private */,
      20,    0,  131,    2, 0x08 /* Private */,
      21,    0,  132,    2, 0x08 /* Private */,
      22,    1,  133,    2, 0x08 /* Private */,
      24,    1,  136,    2, 0x08 /* Private */,
      27,    1,  139,    2, 0x08 /* Private */,
      29,    0,  142,    2, 0x08 /* Private */,
      30,    0,  143,    2, 0x08 /* Private */,
      31,    0,  144,    2, 0x08 /* Private */,
      32,    1,  145,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, QMetaType::Bool,    6,    8,    9,   10,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal, QMetaType::Int,   16,   17,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QReal,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   23,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, QMetaType::UInt,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void BlocksGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BlocksGraphicsView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sceneUpdated(); break;
        case 1: _t->treeChanged(); break;
        case 2: _t->intervalChanged((*reinterpret_cast< const::profiler_gui::TreeBlocks(*)>(_a[1])),(*reinterpret_cast< ::profiler::timestamp_t(*)>(_a[2])),(*reinterpret_cast< ::profiler::timestamp_t(*)>(_a[3])),(*reinterpret_cast< ::profiler::timestamp_t(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 3: _t->onWindowActivationChanged(); break;
        case 4: _t->repaintHistogramImage(); break;
        case 5: _t->repaintScene(); break;
        case 6: _t->onGraphicsScrollbarWheel((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onScrollbarValueChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->onGraphicsScrollbarValueChange((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 9: _t->onFlickerTimeout(); break;
        case 10: _t->onIdleTimeout(); break;
        case 11: _t->onHierarchyFlagChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onSelectedThreadChange((*reinterpret_cast< ::profiler::thread_id_t(*)>(_a[1]))); break;
        case 13: _t->onSelectedBlockChange((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 14: _t->onRefreshRequired(); break;
        case 15: _t->onThreadViewChanged(); break;
        case 16: _t->onZoomSelection(); break;
        case 17: _t->onInspectCurrentView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BlocksGraphicsView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BlocksGraphicsView::sceneUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BlocksGraphicsView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BlocksGraphicsView::treeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BlocksGraphicsView::*)(const ::profiler_gui::TreeBlocks & , ::profiler::timestamp_t , ::profiler::timestamp_t , ::profiler::timestamp_t , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BlocksGraphicsView::intervalChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BlocksGraphicsView::staticMetaObject = { {
    &QGraphicsView::staticMetaObject,
    qt_meta_stringdata_BlocksGraphicsView.data,
    qt_meta_data_BlocksGraphicsView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BlocksGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlocksGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BlocksGraphicsView.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int BlocksGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void BlocksGraphicsView::sceneUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BlocksGraphicsView::treeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BlocksGraphicsView::intervalChanged(const ::profiler_gui::TreeBlocks & _t1, ::profiler::timestamp_t _t2, ::profiler::timestamp_t _t3, ::profiler::timestamp_t _t4, bool _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_ThreadNamesWidget_t {
    QByteArrayData data[9];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadNamesWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadNamesWidget_t qt_meta_stringdata_ThreadNamesWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ThreadNamesWidget"
QT_MOC_LITERAL(1, 18, 25), // "onWindowActivationChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 25), // "setVerticalScrollbarRange"
QT_MOC_LITERAL(4, 71, 9), // "_minValue"
QT_MOC_LITERAL(5, 81, 9), // "_maxValue"
QT_MOC_LITERAL(6, 91, 12), // "onTreeChange"
QT_MOC_LITERAL(7, 104, 13), // "onIdleTimeout"
QT_MOC_LITERAL(8, 118, 12) // "repaintScene"

    },
    "ThreadNamesWidget\0onWindowActivationChanged\0"
    "\0setVerticalScrollbarRange\0_minValue\0"
    "_maxValue\0onTreeChange\0onIdleTimeout\0"
    "repaintScene"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadNamesWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    2,   40,    2, 0x08 /* Private */,
       6,    0,   45,    2, 0x08 /* Private */,
       7,    0,   46,    2, 0x08 /* Private */,
       8,    0,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ThreadNamesWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ThreadNamesWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onWindowActivationChanged(); break;
        case 1: _t->setVerticalScrollbarRange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onTreeChange(); break;
        case 3: _t->onIdleTimeout(); break;
        case 4: _t->repaintScene(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ThreadNamesWidget::staticMetaObject = { {
    &QGraphicsView::staticMetaObject,
    qt_meta_stringdata_ThreadNamesWidget.data,
    qt_meta_data_ThreadNamesWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ThreadNamesWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadNamesWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadNamesWidget.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int ThreadNamesWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_DiagramWidget_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DiagramWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DiagramWidget_t qt_meta_stringdata_DiagramWidget = {
    {
QT_MOC_LITERAL(0, 0, 13) // "DiagramWidget"

    },
    "DiagramWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DiagramWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void DiagramWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DiagramWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DiagramWidget.data,
    qt_meta_data_DiagramWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DiagramWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiagramWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DiagramWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DiagramWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
