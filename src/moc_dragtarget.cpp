/****************************************************************************
** Meta object code from reading C++ file 'dragtarget.h'
**
** Created: Fri Apr 11 15:34:48 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dragtarget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dragtarget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_dragTarget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      30,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x09,
      56,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_dragTarget[] = {
    "dragTarget\0\0createNewFolder()\0"
    "deleteItem()\0scrollDown()\0scrollUp()\0"
};

const QMetaObject dragTarget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_dragTarget,
      qt_meta_data_dragTarget, 0 }
};

const QMetaObject *dragTarget::metaObject() const
{
    return &staticMetaObject;
}

void *dragTarget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_dragTarget))
	return static_cast<void*>(const_cast< dragTarget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int dragTarget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createNewFolder(); break;
        case 1: deleteItem(); break;
        case 2: scrollDown(); break;
        case 3: scrollUp(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
