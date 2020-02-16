/****************************************************************************
** Meta object code from reading C++ file 'bookmarks_editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../profiler_gui/bookmarks_editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bookmarks_editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BookmarkEditor_t {
    QByteArrayData data[9];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BookmarkEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BookmarkEditor_t qt_meta_stringdata_BookmarkEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BookmarkEditor"
QT_MOC_LITERAL(1, 15, 15), // "bookmarkRemoved"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 6), // "size_t"
QT_MOC_LITERAL(4, 39, 5), // "index"
QT_MOC_LITERAL(5, 45, 13), // "onSaveClicked"
QT_MOC_LITERAL(6, 59, 15), // "onDeleteClicked"
QT_MOC_LITERAL(7, 75, 20), // "onColorButtonClicked"
QT_MOC_LITERAL(8, 96, 8) // "onReject"

    },
    "BookmarkEditor\0bookmarkRemoved\0\0size_t\0"
    "index\0onSaveClicked\0onDeleteClicked\0"
    "onColorButtonClicked\0onReject"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BookmarkEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   42,    2, 0x08 /* Private */,
       6,    1,   45,    2, 0x08 /* Private */,
       7,    1,   48,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

       0        // eod
};

void BookmarkEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BookmarkEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->bookmarkRemoved((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 1: _t->onSaveClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onDeleteClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onColorButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onReject(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BookmarkEditor::*)(size_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BookmarkEditor::bookmarkRemoved)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BookmarkEditor::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_BookmarkEditor.data,
    qt_meta_data_BookmarkEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BookmarkEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BookmarkEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BookmarkEditor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int BookmarkEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void BookmarkEditor::bookmarkRemoved(size_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
