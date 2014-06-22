/****************************************************************************
** Meta object code from reading C++ file 'SegmentationOptionPage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SegmentationOptionPage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SegmentationOptionPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SegmentationOptionPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   24,   23,   23, 0x08,
      69,   63,   23,   23, 0x08,
      92,   23,   23,   23, 0x08,
     111,   23,   23,   23, 0x08,
     125,   23,   23,   23, 0x08,
     139,   23,   23,   23, 0x08,
     160,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SegmentationOptionPage[] = {
    "SegmentationOptionPage\0\0method\0"
    "segment_method_changed(QString)\0state\0"
    "canny_chk_changed(int)\0use_canny_or_log()\0"
    "GetSettings()\0SetSettings()\0"
    "chose_object_color()\0chose_bkg_color()\0"
};

void SegmentationOptionPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SegmentationOptionPage *_t = static_cast<SegmentationOptionPage *>(_o);
        switch (_id) {
        case 0: _t->segment_method_changed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->canny_chk_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->use_canny_or_log(); break;
        case 3: _t->GetSettings(); break;
        case 4: _t->SetSettings(); break;
        case 5: _t->chose_object_color(); break;
        case 6: _t->chose_bkg_color(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SegmentationOptionPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SegmentationOptionPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SegmentationOptionPage,
      qt_meta_data_SegmentationOptionPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SegmentationOptionPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SegmentationOptionPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SegmentationOptionPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SegmentationOptionPage))
        return static_cast<void*>(const_cast< SegmentationOptionPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int SegmentationOptionPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
