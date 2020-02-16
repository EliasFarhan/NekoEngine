/****************************************************************************
** Meta object code from reading C++ file 'arbitrary_value_inspector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/arbitrary_value_inspector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arbitrary_value_inspector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GraphicsChart_t {
    QByteArrayData data[3];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GraphicsChart_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GraphicsChart_t qt_meta_stringdata_GraphicsChart = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GraphicsChart"
QT_MOC_LITERAL(1, 14, 24), // "onAutoAdjustChartChanged"
QT_MOC_LITERAL(2, 39, 0) // ""

    },
    "GraphicsChart\0onAutoAdjustChartChanged\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GraphicsChart[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void GraphicsChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GraphicsChart *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onAutoAdjustChartChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject GraphicsChart::staticMetaObject = { {
    &GraphicsSliderArea::staticMetaObject,
    qt_meta_stringdata_GraphicsChart.data,
    qt_meta_data_GraphicsChart,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GraphicsChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphicsChart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsChart.stringdata0))
        return static_cast<void*>(this);
    return GraphicsSliderArea::qt_metacast(_clname);
}

int GraphicsChart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GraphicsSliderArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_ArbitraryValuesWidget_t {
    QByteArrayData data[40];
    char stringdata0[617];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArbitraryValuesWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArbitraryValuesWidget_t qt_meta_stringdata_ArbitraryValuesWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ArbitraryValuesWidget"
QT_MOC_LITERAL(1, 22, 5), // "clear"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "rebuild"
QT_MOC_LITERAL(4, 37, 21), // "profiler::thread_id_t"
QT_MOC_LITERAL(5, 59, 9), // "_threadId"
QT_MOC_LITERAL(6, 69, 23), // "profiler::block_index_t"
QT_MOC_LITERAL(7, 93, 11), // "_blockIndex"
QT_MOC_LITERAL(8, 105, 20), // "profiler::block_id_t"
QT_MOC_LITERAL(9, 126, 8), // "_blockId"
QT_MOC_LITERAL(10, 135, 6), // "select"
QT_MOC_LITERAL(11, 142, 24), // "profiler::ArbitraryValue"
QT_MOC_LITERAL(12, 167, 6), // "_value"
QT_MOC_LITERAL(13, 174, 12), // "_resetOthers"
QT_MOC_LITERAL(14, 187, 22), // "onHeaderSectionResized"
QT_MOC_LITERAL(15, 210, 12), // "logicalIndex"
QT_MOC_LITERAL(16, 223, 7), // "oldSize"
QT_MOC_LITERAL(17, 231, 7), // "newSize"
QT_MOC_LITERAL(18, 239, 23), // "onSelectedThreadChanged"
QT_MOC_LITERAL(19, 263, 22), // "onSelectedBlockChanged"
QT_MOC_LITERAL(20, 286, 8), // "uint32_t"
QT_MOC_LITERAL(21, 295, 12), // "_block_index"
QT_MOC_LITERAL(22, 308, 24), // "onSelectedBlockIdChanged"
QT_MOC_LITERAL(23, 333, 3), // "_id"
QT_MOC_LITERAL(24, 337, 19), // "onItemDoubleClicked"
QT_MOC_LITERAL(25, 357, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(26, 374, 5), // "_item"
QT_MOC_LITERAL(27, 380, 7), // "_column"
QT_MOC_LITERAL(28, 388, 13), // "onItemChanged"
QT_MOC_LITERAL(29, 402, 20), // "onCurrentItemChanged"
QT_MOC_LITERAL(30, 423, 20), // "onCollectionsTimeout"
QT_MOC_LITERAL(31, 444, 25), // "onRegularChartTypeChecked"
QT_MOC_LITERAL(32, 470, 8), // "_checked"
QT_MOC_LITERAL(33, 479, 28), // "onComplexityChartTypeChecked"
QT_MOC_LITERAL(34, 508, 23), // "onFilterComboBoxChanged"
QT_MOC_LITERAL(35, 532, 6), // "_index"
QT_MOC_LITERAL(36, 539, 25), // "onFilterWindowSizeChanged"
QT_MOC_LITERAL(37, 565, 5), // "_size"
QT_MOC_LITERAL(38, 571, 20), // "onExportToCsvClicked"
QT_MOC_LITERAL(39, 592, 24) // "onOpenInNewWindowClicked"

    },
    "ArbitraryValuesWidget\0clear\0\0rebuild\0"
    "profiler::thread_id_t\0_threadId\0"
    "profiler::block_index_t\0_blockIndex\0"
    "profiler::block_id_t\0_blockId\0select\0"
    "profiler::ArbitraryValue\0_value\0"
    "_resetOthers\0onHeaderSectionResized\0"
    "logicalIndex\0oldSize\0newSize\0"
    "onSelectedThreadChanged\0onSelectedBlockChanged\0"
    "uint32_t\0_block_index\0onSelectedBlockIdChanged\0"
    "_id\0onItemDoubleClicked\0QTreeWidgetItem*\0"
    "_item\0_column\0onItemChanged\0"
    "onCurrentItemChanged\0onCollectionsTimeout\0"
    "onRegularChartTypeChecked\0_checked\0"
    "onComplexityChartTypeChecked\0"
    "onFilterComboBoxChanged\0_index\0"
    "onFilterWindowSizeChanged\0_size\0"
    "onExportToCsvClicked\0onOpenInNewWindowClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArbitraryValuesWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x0a /* Public */,
       3,    0,  110,    2, 0x0a /* Public */,
       3,    3,  111,    2, 0x0a /* Public */,
      10,    2,  118,    2, 0x0a /* Public */,
      10,    1,  123,    2, 0x2a /* Public | MethodCloned */,
      14,    3,  126,    2, 0x08 /* Private */,
      18,    1,  133,    2, 0x08 /* Private */,
      19,    1,  136,    2, 0x08 /* Private */,
      22,    1,  139,    2, 0x08 /* Private */,
      24,    2,  142,    2, 0x08 /* Private */,
      28,    2,  147,    2, 0x08 /* Private */,
      29,    2,  152,    2, 0x08 /* Private */,
      30,    0,  157,    2, 0x08 /* Private */,
      31,    1,  158,    2, 0x08 /* Private */,
      33,    1,  161,    2, 0x08 /* Private */,
      34,    1,  164,    2, 0x08 /* Private */,
      36,    1,  167,    2, 0x08 /* Private */,
      38,    1,  170,    2, 0x08 /* Private */,
      39,    1,  173,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, 0x80000000 | 8,    5,    7,    9,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Bool,   12,   13,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,   17,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 8,   23,
    QMetaType::Void, 0x80000000 | 25, QMetaType::Int,   26,   27,
    QMetaType::Void, 0x80000000 | 25, QMetaType::Int,   26,   27,
    QMetaType::Void, 0x80000000 | 25, 0x80000000 | 25,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::Int,   37,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void ArbitraryValuesWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArbitraryValuesWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clear(); break;
        case 1: _t->rebuild(); break;
        case 2: _t->rebuild((*reinterpret_cast< profiler::thread_id_t(*)>(_a[1])),(*reinterpret_cast< profiler::block_index_t(*)>(_a[2])),(*reinterpret_cast< profiler::block_id_t(*)>(_a[3]))); break;
        case 3: _t->select((*reinterpret_cast< const profiler::ArbitraryValue(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->select((*reinterpret_cast< const profiler::ArbitraryValue(*)>(_a[1]))); break;
        case 5: _t->onHeaderSectionResized((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->onSelectedThreadChanged((*reinterpret_cast< profiler::thread_id_t(*)>(_a[1]))); break;
        case 7: _t->onSelectedBlockChanged((*reinterpret_cast< uint32_t(*)>(_a[1]))); break;
        case 8: _t->onSelectedBlockIdChanged((*reinterpret_cast< profiler::block_id_t(*)>(_a[1]))); break;
        case 9: _t->onItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->onCurrentItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 12: _t->onCollectionsTimeout(); break;
        case 13: _t->onRegularChartTypeChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->onComplexityChartTypeChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->onFilterComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->onFilterWindowSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->onExportToCsvClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->onOpenInNewWindowClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArbitraryValuesWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ArbitraryValuesWidget.data,
    qt_meta_data_ArbitraryValuesWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArbitraryValuesWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArbitraryValuesWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArbitraryValuesWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ArbitraryValuesWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
