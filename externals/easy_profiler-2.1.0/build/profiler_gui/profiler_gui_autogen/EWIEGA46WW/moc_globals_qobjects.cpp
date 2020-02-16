/****************************************************************************
** Meta object code from reading C++ file 'globals_qobjects.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/globals_qobjects.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'globals_qobjects.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_profiler_gui__GlobalSignals_t {
    QByteArrayData data[50];
    char stringdata0[876];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_profiler_gui__GlobalSignals_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_profiler_gui__GlobalSignals_t qt_meta_stringdata_profiler_gui__GlobalSignals = {
    {
QT_MOC_LITERAL(0, 0, 27), // "profiler_gui::GlobalSignals"
QT_MOC_LITERAL(1, 28, 10), // "closeEvent"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 23), // "allDataGoingToBeDeleted"
QT_MOC_LITERAL(4, 64, 10), // "fileOpened"
QT_MOC_LITERAL(5, 75, 21), // "selectedThreadChanged"
QT_MOC_LITERAL(6, 97, 23), // "::profiler::thread_id_t"
QT_MOC_LITERAL(7, 121, 3), // "_id"
QT_MOC_LITERAL(8, 125, 20), // "selectedBlockChanged"
QT_MOC_LITERAL(9, 146, 8), // "uint32_t"
QT_MOC_LITERAL(10, 155, 12), // "_block_index"
QT_MOC_LITERAL(11, 168, 22), // "selectedBlockIdChanged"
QT_MOC_LITERAL(12, 191, 22), // "::profiler::block_id_t"
QT_MOC_LITERAL(13, 214, 23), // "itemsExpandStateChanged"
QT_MOC_LITERAL(14, 238, 18), // "blockStatusChanged"
QT_MOC_LITERAL(15, 257, 27), // "::profiler::EasyBlockStatus"
QT_MOC_LITERAL(16, 285, 7), // "_status"
QT_MOC_LITERAL(17, 293, 17), // "connectionChanged"
QT_MOC_LITERAL(18, 311, 10), // "_connected"
QT_MOC_LITERAL(19, 322, 21), // "blocksRefreshRequired"
QT_MOC_LITERAL(20, 344, 24), // "expectedFrameTimeChanged"
QT_MOC_LITERAL(21, 369, 26), // "autoAdjustHistogramChanged"
QT_MOC_LITERAL(22, 396, 22), // "autoAdjustChartChanged"
QT_MOC_LITERAL(23, 419, 35), // "displayOnlyFramesOnHistogramC..."
QT_MOC_LITERAL(24, 455, 20), // "hierarchyFlagChanged"
QT_MOC_LITERAL(25, 476, 27), // "threadNameDecorationChanged"
QT_MOC_LITERAL(26, 504, 18), // "hexThreadIdChanged"
QT_MOC_LITERAL(27, 523, 15), // "refreshRequired"
QT_MOC_LITERAL(28, 539, 21), // "blocksTreeModeChanged"
QT_MOC_LITERAL(29, 561, 12), // "rulerVisible"
QT_MOC_LITERAL(30, 574, 12), // "sceneCleared"
QT_MOC_LITERAL(31, 587, 16), // "sceneSizeChanged"
QT_MOC_LITERAL(32, 604, 4), // "left"
QT_MOC_LITERAL(33, 609, 5), // "right"
QT_MOC_LITERAL(34, 615, 29), // "sceneVisibleRegionSizeChanged"
QT_MOC_LITERAL(35, 645, 5), // "width"
QT_MOC_LITERAL(36, 651, 28), // "sceneVisibleRegionPosChanged"
QT_MOC_LITERAL(37, 680, 3), // "pos"
QT_MOC_LITERAL(38, 684, 10), // "lockCharts"
QT_MOC_LITERAL(39, 695, 12), // "unlockCharts"
QT_MOC_LITERAL(40, 708, 12), // "chartWheeled"
QT_MOC_LITERAL(41, 721, 5), // "delta"
QT_MOC_LITERAL(42, 727, 18), // "chartSliderChanged"
QT_MOC_LITERAL(43, 746, 11), // "selectValue"
QT_MOC_LITERAL(44, 758, 10), // "_thread_id"
QT_MOC_LITERAL(45, 769, 12), // "_value_index"
QT_MOC_LITERAL(46, 782, 32), // "const::profiler::ArbitraryValue&"
QT_MOC_LITERAL(47, 815, 6), // "_value"
QT_MOC_LITERAL(48, 822, 25), // "customWindowHeaderChanged"
QT_MOC_LITERAL(49, 848, 27) // "windowHeaderPositionChanged"

    },
    "profiler_gui::GlobalSignals\0closeEvent\0"
    "\0allDataGoingToBeDeleted\0fileOpened\0"
    "selectedThreadChanged\0::profiler::thread_id_t\0"
    "_id\0selectedBlockChanged\0uint32_t\0"
    "_block_index\0selectedBlockIdChanged\0"
    "::profiler::block_id_t\0itemsExpandStateChanged\0"
    "blockStatusChanged\0::profiler::EasyBlockStatus\0"
    "_status\0connectionChanged\0_connected\0"
    "blocksRefreshRequired\0expectedFrameTimeChanged\0"
    "autoAdjustHistogramChanged\0"
    "autoAdjustChartChanged\0"
    "displayOnlyFramesOnHistogramChanged\0"
    "hierarchyFlagChanged\0threadNameDecorationChanged\0"
    "hexThreadIdChanged\0refreshRequired\0"
    "blocksTreeModeChanged\0rulerVisible\0"
    "sceneCleared\0sceneSizeChanged\0left\0"
    "right\0sceneVisibleRegionSizeChanged\0"
    "width\0sceneVisibleRegionPosChanged\0"
    "pos\0lockCharts\0unlockCharts\0chartWheeled\0"
    "delta\0chartSliderChanged\0selectValue\0"
    "_thread_id\0_value_index\0"
    "const::profiler::ArbitraryValue&\0"
    "_value\0customWindowHeaderChanged\0"
    "windowHeaderPositionChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_profiler_gui__GlobalSignals[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      31,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  169,    2, 0x06 /* Public */,
       3,    0,  170,    2, 0x06 /* Public */,
       4,    0,  171,    2, 0x06 /* Public */,
       5,    1,  172,    2, 0x06 /* Public */,
       8,    1,  175,    2, 0x06 /* Public */,
      11,    1,  178,    2, 0x06 /* Public */,
      13,    0,  181,    2, 0x06 /* Public */,
      14,    2,  182,    2, 0x06 /* Public */,
      17,    1,  187,    2, 0x06 /* Public */,
      19,    1,  190,    2, 0x06 /* Public */,
      20,    0,  193,    2, 0x06 /* Public */,
      21,    0,  194,    2, 0x06 /* Public */,
      22,    0,  195,    2, 0x06 /* Public */,
      23,    0,  196,    2, 0x06 /* Public */,
      24,    1,  197,    2, 0x06 /* Public */,
      25,    0,  200,    2, 0x06 /* Public */,
      26,    0,  201,    2, 0x06 /* Public */,
      27,    0,  202,    2, 0x06 /* Public */,
      28,    0,  203,    2, 0x06 /* Public */,
      29,    1,  204,    2, 0x06 /* Public */,
      30,    0,  207,    2, 0x06 /* Public */,
      31,    2,  208,    2, 0x06 /* Public */,
      34,    1,  213,    2, 0x06 /* Public */,
      36,    1,  216,    2, 0x06 /* Public */,
      38,    0,  219,    2, 0x06 /* Public */,
      39,    0,  220,    2, 0x06 /* Public */,
      40,    2,  221,    2, 0x06 /* Public */,
      42,    1,  226,    2, 0x06 /* Public */,
      43,    3,  229,    2, 0x06 /* Public */,
      48,    0,  236,    2, 0x06 /* Public */,
      49,    0,  237,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 15,    7,   16,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,   32,   33,
    QMetaType::Void, QMetaType::QReal,   35,
    QMetaType::Void, QMetaType::QReal,   37,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QReal, QMetaType::Int,   37,   41,
    QMetaType::Void, QMetaType::QReal,   37,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 46,   44,   45,   47,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void profiler_gui::GlobalSignals::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GlobalSignals *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeEvent(); break;
        case 1: _t->allDataGoingToBeDeleted(); break;
        case 2: _t->fileOpened(); break;
        case 3: _t->selectedThreadChanged((*reinterpret_cast< ::profiler::thread_id_t(*)>(_a[1]))); break;
        case 4: _t->selectedBlockChanged((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 5: _t->selectedBlockIdChanged((*reinterpret_cast< ::profiler::block_id_t(*)>(_a[1]))); break;
        case 6: _t->itemsExpandStateChanged(); break;
        case 7: _t->blockStatusChanged((*reinterpret_cast< ::profiler::block_id_t(*)>(_a[1])),(*reinterpret_cast< ::profiler::EasyBlockStatus(*)>(_a[2]))); break;
        case 8: _t->connectionChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->blocksRefreshRequired((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->expectedFrameTimeChanged(); break;
        case 11: _t->autoAdjustHistogramChanged(); break;
        case 12: _t->autoAdjustChartChanged(); break;
        case 13: _t->displayOnlyFramesOnHistogramChanged(); break;
        case 14: _t->hierarchyFlagChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->threadNameDecorationChanged(); break;
        case 16: _t->hexThreadIdChanged(); break;
        case 17: _t->refreshRequired(); break;
        case 18: _t->blocksTreeModeChanged(); break;
        case 19: _t->rulerVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->sceneCleared(); break;
        case 21: _t->sceneSizeChanged((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 22: _t->sceneVisibleRegionSizeChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 23: _t->sceneVisibleRegionPosChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 24: _t->lockCharts(); break;
        case 25: _t->unlockCharts(); break;
        case 26: _t->chartWheeled((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 27: _t->chartSliderChanged((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 28: _t->selectValue((*reinterpret_cast< ::profiler::thread_id_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< const::profiler::ArbitraryValue(*)>(_a[3]))); break;
        case 29: _t->customWindowHeaderChanged(); break;
        case 30: _t->windowHeaderPositionChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::closeEvent)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::allDataGoingToBeDeleted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::fileOpened)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(::profiler::thread_id_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::selectedThreadChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(uint32_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::selectedBlockChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(::profiler::block_id_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::selectedBlockIdChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::itemsExpandStateChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(::profiler::block_id_t , ::profiler::EasyBlockStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::blockStatusChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::connectionChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::blocksRefreshRequired)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::expectedFrameTimeChanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::autoAdjustHistogramChanged)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::autoAdjustChartChanged)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::displayOnlyFramesOnHistogramChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::hierarchyFlagChanged)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::threadNameDecorationChanged)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::hexThreadIdChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::refreshRequired)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::blocksTreeModeChanged)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::rulerVisible)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::sceneCleared)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(qreal , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::sceneSizeChanged)) {
                *result = 21;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::sceneVisibleRegionSizeChanged)) {
                *result = 22;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::sceneVisibleRegionPosChanged)) {
                *result = 23;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::lockCharts)) {
                *result = 24;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::unlockCharts)) {
                *result = 25;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(qreal , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::chartWheeled)) {
                *result = 26;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::chartSliderChanged)) {
                *result = 27;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)(::profiler::thread_id_t , uint32_t , const ::profiler::ArbitraryValue & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::selectValue)) {
                *result = 28;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::customWindowHeaderChanged)) {
                *result = 29;
                return;
            }
        }
        {
            using _t = void (GlobalSignals::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalSignals::windowHeaderPositionChanged)) {
                *result = 30;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject profiler_gui::GlobalSignals::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_profiler_gui__GlobalSignals.data,
    qt_meta_data_profiler_gui__GlobalSignals,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *profiler_gui::GlobalSignals::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *profiler_gui::GlobalSignals::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_profiler_gui__GlobalSignals.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int profiler_gui::GlobalSignals::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void profiler_gui::GlobalSignals::closeEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void profiler_gui::GlobalSignals::allDataGoingToBeDeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void profiler_gui::GlobalSignals::fileOpened()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void profiler_gui::GlobalSignals::selectedThreadChanged(::profiler::thread_id_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void profiler_gui::GlobalSignals::selectedBlockChanged(uint32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void profiler_gui::GlobalSignals::selectedBlockIdChanged(::profiler::block_id_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void profiler_gui::GlobalSignals::itemsExpandStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void profiler_gui::GlobalSignals::blockStatusChanged(::profiler::block_id_t _t1, ::profiler::EasyBlockStatus _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void profiler_gui::GlobalSignals::connectionChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void profiler_gui::GlobalSignals::blocksRefreshRequired(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void profiler_gui::GlobalSignals::expectedFrameTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void profiler_gui::GlobalSignals::autoAdjustHistogramChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void profiler_gui::GlobalSignals::autoAdjustChartChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void profiler_gui::GlobalSignals::displayOnlyFramesOnHistogramChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void profiler_gui::GlobalSignals::hierarchyFlagChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void profiler_gui::GlobalSignals::threadNameDecorationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void profiler_gui::GlobalSignals::hexThreadIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}

// SIGNAL 17
void profiler_gui::GlobalSignals::refreshRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void profiler_gui::GlobalSignals::blocksTreeModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 18, nullptr);
}

// SIGNAL 19
void profiler_gui::GlobalSignals::rulerVisible(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void profiler_gui::GlobalSignals::sceneCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 20, nullptr);
}

// SIGNAL 21
void profiler_gui::GlobalSignals::sceneSizeChanged(qreal _t1, qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}

// SIGNAL 22
void profiler_gui::GlobalSignals::sceneVisibleRegionSizeChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void profiler_gui::GlobalSignals::sceneVisibleRegionPosChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 23, _a);
}

// SIGNAL 24
void profiler_gui::GlobalSignals::lockCharts()
{
    QMetaObject::activate(this, &staticMetaObject, 24, nullptr);
}

// SIGNAL 25
void profiler_gui::GlobalSignals::unlockCharts()
{
    QMetaObject::activate(this, &staticMetaObject, 25, nullptr);
}

// SIGNAL 26
void profiler_gui::GlobalSignals::chartWheeled(qreal _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 26, _a);
}

// SIGNAL 27
void profiler_gui::GlobalSignals::chartSliderChanged(qreal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 27, _a);
}

// SIGNAL 28
void profiler_gui::GlobalSignals::selectValue(::profiler::thread_id_t _t1, uint32_t _t2, const ::profiler::ArbitraryValue & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void profiler_gui::GlobalSignals::customWindowHeaderChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 29, nullptr);
}

// SIGNAL 30
void profiler_gui::GlobalSignals::windowHeaderPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 30, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
