/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/main_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DockWidget_t {
    QByteArrayData data[4];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DockWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DockWidget_t qt_meta_stringdata_DockWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DockWidget"
QT_MOC_LITERAL(1, 11, 11), // "toggleState"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 17) // "onTopLevelChanged"

    },
    "DockWidget\0toggleState\0\0onTopLevelChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DockWidget[] = {

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
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DockWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggleState(); break;
        case 1: _t->onTopLevelChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject DockWidget::staticMetaObject = { {
    &QDockWidget::staticMetaObject,
    qt_meta_stringdata_DockWidget.data,
    qt_meta_data_DockWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DockWidget.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int DockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[61];
    char stringdata0[1203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 17), // "activationChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "onThemeChange"
QT_MOC_LITERAL(4, 44, 17), // "onOpenFileClicked"
QT_MOC_LITERAL(5, 62, 17), // "onSaveFileClicked"
QT_MOC_LITERAL(6, 80, 15), // "onDeleteClicked"
QT_MOC_LITERAL(7, 96, 13), // "onExitClicked"
QT_MOC_LITERAL(8, 110, 17), // "onEncodingChanged"
QT_MOC_LITERAL(9, 128, 21), // "onRulerTextPosChanged"
QT_MOC_LITERAL(10, 150, 14), // "onUnitsChanged"
QT_MOC_LITERAL(11, 165, 25), // "onEnableDisableStatistics"
QT_MOC_LITERAL(12, 191, 32), // "onCollapseItemsAfterCloseChanged"
QT_MOC_LITERAL(13, 224, 33), // "onAllItemsExpandedByDefaultCh..."
QT_MOC_LITERAL(14, 258, 24), // "onBindExpandStatusChange"
QT_MOC_LITERAL(15, 283, 21), // "onHierarchyFlagChange"
QT_MOC_LITERAL(16, 305, 28), // "onDisplayRelevantStatsChange"
QT_MOC_LITERAL(17, 334, 18), // "onExpandAllClicked"
QT_MOC_LITERAL(18, 353, 20), // "onCollapseAllClicked"
QT_MOC_LITERAL(19, 374, 21), // "onViewportInfoClicked"
QT_MOC_LITERAL(20, 396, 22), // "onMaxBlocksCountChange"
QT_MOC_LITERAL(21, 419, 6), // "_value"
QT_MOC_LITERAL(22, 426, 15), // "onSpacingChange"
QT_MOC_LITERAL(23, 442, 15), // "onMinSizeChange"
QT_MOC_LITERAL(24, 458, 24), // "onHistogramMinSizeChange"
QT_MOC_LITERAL(25, 483, 18), // "onNarrowSizeChange"
QT_MOC_LITERAL(26, 502, 19), // "onFpsIntervalChange"
QT_MOC_LITERAL(27, 522, 18), // "onFpsHistoryChange"
QT_MOC_LITERAL(28, 541, 27), // "onFpsMonitorLineWidthChange"
QT_MOC_LITERAL(29, 569, 19), // "onFileReaderTimeout"
QT_MOC_LITERAL(30, 589, 25), // "onFrameTimeRequestTimeout"
QT_MOC_LITERAL(31, 615, 22), // "onListenerTimerTimeout"
QT_MOC_LITERAL(32, 638, 18), // "onFileReaderCancel"
QT_MOC_LITERAL(33, 657, 19), // "onEditBlocksClicked"
QT_MOC_LITERAL(34, 677, 21), // "onDescTreeDialogClose"
QT_MOC_LITERAL(35, 699, 21), // "onListenerDialogClose"
QT_MOC_LITERAL(36, 721, 16), // "onCaptureClicked"
QT_MOC_LITERAL(37, 738, 29), // "onGetBlockDescriptionsClicked"
QT_MOC_LITERAL(38, 768, 16), // "onConnectClicked"
QT_MOC_LITERAL(39, 785, 28), // "onEventTracingPriorityChange"
QT_MOC_LITERAL(40, 814, 8), // "_checked"
QT_MOC_LITERAL(41, 823, 26), // "onEventTracingEnableChange"
QT_MOC_LITERAL(42, 850, 21), // "onFrameTimeEditFinish"
QT_MOC_LITERAL(43, 872, 18), // "onFrameTimeChanged"
QT_MOC_LITERAL(44, 891, 17), // "onSnapshotClicked"
QT_MOC_LITERAL(45, 909, 29), // "onCustomWindowHeaderTriggered"
QT_MOC_LITERAL(46, 939, 27), // "onRightWindowHeaderPosition"
QT_MOC_LITERAL(47, 967, 26), // "onLeftWindowHeaderPosition"
QT_MOC_LITERAL(48, 994, 19), // "onBlockStatusChange"
QT_MOC_LITERAL(49, 1014, 20), // "profiler::block_id_t"
QT_MOC_LITERAL(50, 1035, 3), // "_id"
QT_MOC_LITERAL(51, 1039, 25), // "profiler::EasyBlockStatus"
QT_MOC_LITERAL(52, 1065, 7), // "_status"
QT_MOC_LITERAL(53, 1073, 13), // "onSelectValue"
QT_MOC_LITERAL(54, 1087, 21), // "profiler::thread_id_t"
QT_MOC_LITERAL(55, 1109, 10), // "_thread_id"
QT_MOC_LITERAL(56, 1120, 8), // "uint32_t"
QT_MOC_LITERAL(57, 1129, 12), // "_value_index"
QT_MOC_LITERAL(58, 1142, 24), // "profiler::ArbitraryValue"
QT_MOC_LITERAL(59, 1167, 19), // "checkFrameTimeReady"
QT_MOC_LITERAL(60, 1187, 15) // "validateLastDir"

    },
    "MainWindow\0activationChanged\0\0"
    "onThemeChange\0onOpenFileClicked\0"
    "onSaveFileClicked\0onDeleteClicked\0"
    "onExitClicked\0onEncodingChanged\0"
    "onRulerTextPosChanged\0onUnitsChanged\0"
    "onEnableDisableStatistics\0"
    "onCollapseItemsAfterCloseChanged\0"
    "onAllItemsExpandedByDefaultChange\0"
    "onBindExpandStatusChange\0onHierarchyFlagChange\0"
    "onDisplayRelevantStatsChange\0"
    "onExpandAllClicked\0onCollapseAllClicked\0"
    "onViewportInfoClicked\0onMaxBlocksCountChange\0"
    "_value\0onSpacingChange\0onMinSizeChange\0"
    "onHistogramMinSizeChange\0onNarrowSizeChange\0"
    "onFpsIntervalChange\0onFpsHistoryChange\0"
    "onFpsMonitorLineWidthChange\0"
    "onFileReaderTimeout\0onFrameTimeRequestTimeout\0"
    "onListenerTimerTimeout\0onFileReaderCancel\0"
    "onEditBlocksClicked\0onDescTreeDialogClose\0"
    "onListenerDialogClose\0onCaptureClicked\0"
    "onGetBlockDescriptionsClicked\0"
    "onConnectClicked\0onEventTracingPriorityChange\0"
    "_checked\0onEventTracingEnableChange\0"
    "onFrameTimeEditFinish\0onFrameTimeChanged\0"
    "onSnapshotClicked\0onCustomWindowHeaderTriggered\0"
    "onRightWindowHeaderPosition\0"
    "onLeftWindowHeaderPosition\0"
    "onBlockStatusChange\0profiler::block_id_t\0"
    "_id\0profiler::EasyBlockStatus\0_status\0"
    "onSelectValue\0profiler::thread_id_t\0"
    "_thread_id\0uint32_t\0_value_index\0"
    "profiler::ArbitraryValue\0checkFrameTimeReady\0"
    "validateLastDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      48,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  254,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,  255,    2, 0x09 /* Protected */,
       4,    1,  258,    2, 0x09 /* Protected */,
       5,    1,  261,    2, 0x09 /* Protected */,
       6,    1,  264,    2, 0x09 /* Protected */,
       7,    1,  267,    2, 0x09 /* Protected */,
       8,    1,  270,    2, 0x09 /* Protected */,
       9,    1,  273,    2, 0x09 /* Protected */,
      10,    1,  276,    2, 0x09 /* Protected */,
      11,    1,  279,    2, 0x09 /* Protected */,
      12,    1,  282,    2, 0x09 /* Protected */,
      13,    1,  285,    2, 0x09 /* Protected */,
      14,    1,  288,    2, 0x09 /* Protected */,
      15,    1,  291,    2, 0x09 /* Protected */,
      16,    1,  294,    2, 0x09 /* Protected */,
      17,    1,  297,    2, 0x09 /* Protected */,
      18,    1,  300,    2, 0x09 /* Protected */,
      19,    1,  303,    2, 0x09 /* Protected */,
      20,    1,  306,    2, 0x09 /* Protected */,
      22,    1,  309,    2, 0x09 /* Protected */,
      23,    1,  312,    2, 0x09 /* Protected */,
      24,    1,  315,    2, 0x09 /* Protected */,
      25,    1,  318,    2, 0x09 /* Protected */,
      26,    1,  321,    2, 0x09 /* Protected */,
      27,    1,  324,    2, 0x09 /* Protected */,
      28,    1,  327,    2, 0x09 /* Protected */,
      29,    0,  330,    2, 0x09 /* Protected */,
      30,    0,  331,    2, 0x09 /* Protected */,
      31,    0,  332,    2, 0x09 /* Protected */,
      32,    0,  333,    2, 0x09 /* Protected */,
      33,    1,  334,    2, 0x09 /* Protected */,
      34,    1,  337,    2, 0x09 /* Protected */,
      35,    1,  340,    2, 0x09 /* Protected */,
      36,    1,  343,    2, 0x09 /* Protected */,
      37,    1,  346,    2, 0x09 /* Protected */,
      38,    1,  349,    2, 0x09 /* Protected */,
      39,    1,  352,    2, 0x09 /* Protected */,
      41,    1,  355,    2, 0x09 /* Protected */,
      42,    0,  358,    2, 0x09 /* Protected */,
      43,    0,  359,    2, 0x09 /* Protected */,
      44,    1,  360,    2, 0x09 /* Protected */,
      45,    1,  363,    2, 0x09 /* Protected */,
      46,    1,  366,    2, 0x09 /* Protected */,
      47,    1,  369,    2, 0x09 /* Protected */,
      48,    2,  372,    2, 0x09 /* Protected */,
      53,    3,  377,    2, 0x09 /* Protected */,
      59,    0,  384,    2, 0x09 /* Protected */,
      60,    0,  385,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void, QMetaType::Bool,   40,
    QMetaType::Void, 0x80000000 | 49, 0x80000000 | 51,   50,   52,
    QMetaType::Void, 0x80000000 | 54, 0x80000000 | 56, 0x80000000 | 58,   55,   57,   21,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->activationChanged(); break;
        case 1: _t->onThemeChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onOpenFileClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onSaveFileClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onDeleteClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onExitClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onEncodingChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onRulerTextPosChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->onUnitsChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onEnableDisableStatistics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->onCollapseItemsAfterCloseChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onAllItemsExpandedByDefaultChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onBindExpandStatusChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onHierarchyFlagChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->onDisplayRelevantStatsChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->onExpandAllClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->onCollapseAllClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->onViewportInfoClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->onMaxBlocksCountChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onSpacingChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onMinSizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->onHistogramMinSizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->onNarrowSizeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->onFpsIntervalChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->onFpsHistoryChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->onFpsMonitorLineWidthChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->onFileReaderTimeout(); break;
        case 27: _t->onFrameTimeRequestTimeout(); break;
        case 28: _t->onListenerTimerTimeout(); break;
        case 29: _t->onFileReaderCancel(); break;
        case 30: _t->onEditBlocksClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->onDescTreeDialogClose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->onListenerDialogClose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->onCaptureClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->onGetBlockDescriptionsClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->onConnectClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: _t->onEventTracingPriorityChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->onEventTracingEnableChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->onFrameTimeEditFinish(); break;
        case 39: _t->onFrameTimeChanged(); break;
        case 40: _t->onSnapshotClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: _t->onCustomWindowHeaderTriggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 42: _t->onRightWindowHeaderPosition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 43: _t->onLeftWindowHeaderPosition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->onBlockStatusChange((*reinterpret_cast< profiler::block_id_t(*)>(_a[1])),(*reinterpret_cast< profiler::EasyBlockStatus(*)>(_a[2]))); break;
        case 45: _t->onSelectValue((*reinterpret_cast< profiler::thread_id_t(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< const profiler::ArbitraryValue(*)>(_a[3]))); break;
        case 46: _t->checkFrameTimeReady(); break;
        case 47: _t->validateLastDir(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::activationChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 48)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 48;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 48)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 48;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::activationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
