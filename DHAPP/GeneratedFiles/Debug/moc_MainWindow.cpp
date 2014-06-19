/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      21,   11,   11,   11, 0x0a,
      31,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x0a,
      53,   11,   11,   11, 0x0a,
      64,   11,   11,   11, 0x0a,
      94,   81,   11,   11, 0x0a,
     113,   11,   11,   11, 0x0a,
     123,   11,   11,   11, 0x0a,
     132,   11,   11,   11, 0x0a,
     146,   11,   11,   11, 0x0a,
     166,   11,   11,   11, 0x0a,
     183,   11,   11,   11, 0x0a,
     199,   11,   11,   11, 0x0a,
     218,   11,   11,   11, 0x0a,
     233,   11,   11,   11, 0x0a,
     251,   11,   11,   11, 0x0a,
     266,   11,   11,   11, 0x0a,
     285,   11,   11,   11, 0x0a,
     304,   11,   11,   11, 0x0a,
     322,   11,   11,   11, 0x0a,
     331,   11,   11,   11, 0x0a,
     341,   11,   11,   11, 0x0a,
     351,   11,   11,   11, 0x0a,
     378,  368,   11,   11, 0x0a,
     416,  408,   11,   11, 0x08,
     447,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0NewPrj()\0SavePrj()\0"
    "SaveAsPrj()\0OpenPrj()\0closePrj()\0"
    "OpenRecentFile()\0img_filePath\0"
    "OpenImage(QString)\0ZoomOut()\0ZoomIn()\0"
    "ImportImage()\0onViewInActualPix()\0"
    "onViewInScreen()\0onActionClose()\0"
    "onActionCloseAll()\0onActionTile()\0"
    "onActionCascade()\0onActionNext()\0"
    "onActionPrevious()\0onSaveInpainting()\0"
    "ShowOptionPanel()\0mixbmp()\0UndoAct()\0"
    "RedoAct()\0JudgeEditState()\0bModified\0"
    "UpdateProjectFileStatus(bool)\0checked\0"
    "onOperationActionToggled(bool)\0"
    "onToolMenuAboutToShow()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->NewPrj(); break;
        case 1: _t->SavePrj(); break;
        case 2: _t->SaveAsPrj(); break;
        case 3: _t->OpenPrj(); break;
        case 4: _t->closePrj(); break;
        case 5: _t->OpenRecentFile(); break;
        case 6: _t->OpenImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->ZoomOut(); break;
        case 8: _t->ZoomIn(); break;
        case 9: _t->ImportImage(); break;
        case 10: _t->onViewInActualPix(); break;
        case 11: _t->onViewInScreen(); break;
        case 12: _t->onActionClose(); break;
        case 13: _t->onActionCloseAll(); break;
        case 14: _t->onActionTile(); break;
        case 15: _t->onActionCascade(); break;
        case 16: _t->onActionNext(); break;
        case 17: _t->onActionPrevious(); break;
        case 18: _t->onSaveInpainting(); break;
        case 19: _t->ShowOptionPanel(); break;
        case 20: _t->mixbmp(); break;
        case 21: _t->UndoAct(); break;
        case 22: _t->RedoAct(); break;
        case 23: _t->JudgeEditState(); break;
        case 24: _t->UpdateProjectFileStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->onOperationActionToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->onToolMenuAboutToShow(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 27)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
