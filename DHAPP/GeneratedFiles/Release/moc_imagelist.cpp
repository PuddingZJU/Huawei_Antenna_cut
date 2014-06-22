/****************************************************************************
** Meta object code from reading C++ file 'imagelist.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imagelist.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagelist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageList[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   10,   10,   10, 0x08,
      64,   55,   49,   10, 0x08,
      97,   85,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ImageList[] = {
    "ImageList\0\0imageDoubleClicked(QString)\0"
    "refresh()\0QSize\0img_size\0getScaledSize(QSize)\0"
    "item,column\0DisplayImage(QTreeWidgetItem*,int)\0"
};

void ImageList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ImageList *_t = static_cast<ImageList *>(_o);
        switch (_id) {
        case 0: _t->imageDoubleClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->refresh(); break;
        case 2: { QSize _r = _t->getScaledSize((*reinterpret_cast< QSize(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QSize*>(_a[0]) = _r; }  break;
        case 3: _t->DisplayImage((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ImageList::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ImageList::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_ImageList,
      qt_meta_data_ImageList, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImageList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImageList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImageList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageList))
        return static_cast<void*>(const_cast< ImageList*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int ImageList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ImageList::imageDoubleClicked(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
