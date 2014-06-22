/****************************************************************************
** Meta object code from reading C++ file 'OptionPageManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../OptionPageManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OptionPageManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OptionPageManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,
      43,   18,   18,   18, 0x05,
      48,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   57,   18,   18, 0x08,
     128,   18,   18,   18, 0x08,
     145,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OptionPageManager[] = {
    "OptionPageManager\0\0index\0CurrentIndex(int)\0"
    "OK()\0Cancel()\0current,previous\0"
    "CurrentIndexChange(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "CancelAndClose()\0OKAndClose()\0"
};

void OptionPageManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OptionPageManager *_t = static_cast<OptionPageManager *>(_o);
        switch (_id) {
        case 0: _t->CurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OK(); break;
        case 2: _t->Cancel(); break;
        case 3: _t->CurrentIndexChange((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 4: _t->CancelAndClose(); break;
        case 5: _t->OKAndClose(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OptionPageManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OptionPageManager::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OptionPageManager,
      qt_meta_data_OptionPageManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OptionPageManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OptionPageManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OptionPageManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OptionPageManager))
        return static_cast<void*>(const_cast< OptionPageManager*>(this));
    return QWidget::qt_metacast(_clname);
}

int OptionPageManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void OptionPageManager::CurrentIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OptionPageManager::OK()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void OptionPageManager::Cancel()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
