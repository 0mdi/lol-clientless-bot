/****************************************************************************
** Meta object code from reading C++ file 'mastercpclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mastercpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mastercpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MasterCPClient_t {
	QByteArrayData data[8];
	char stringdata[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MasterCPClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MasterCPClient_t qt_meta_stringdata_MasterCPClient = {
	{
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 9),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 12),
QT_MOC_LITERAL(4, 39, 9),
QT_MOC_LITERAL(5, 49, 13),
QT_MOC_LITERAL(6, 63, 17),
QT_MOC_LITERAL(7, 81, 20)
	},
	"MasterCPClient\0connected\0\0disconnected\0"
	"readyRead\0onDebugAction\0onAddOrderPressed\0"
	"onCreateOrderPressed\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MasterCPClient[] = {
	// content:
		  7,       // revision
		  0,       // classname
		  0,    0, // classinfo
		  6,   14, // methods
		  0,    0, // properties
		  0,    0, // enums/sets
		  0,    0, // constructors
		  0,       // flags
		  0,       // signalCount

	// slots: name, argc, parameters, tag, flags
		  1,    0,   44,    2, 0x0a,
		  3,    0,   45,    2, 0x0a,
		  4,    0,   46,    2, 0x0a,
		  5,    0,   47,    2, 0x0a,
		  6,    0,   48,    2, 0x0a,
		  7,    0,   49,    2, 0x0a,

		  // slots: parameters
			 QMetaType::Void,
			 QMetaType::Void,
			 QMetaType::Void,
			 QMetaType::Void,
			 QMetaType::Void,
			 QMetaType::Void,

				0        // eod
};

void MasterCPClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
	if (_c == QMetaObject::InvokeMetaMethod) {
		MasterCPClient *_t = static_cast<MasterCPClient *>(_o);
		switch (_id) {
		case 0: _t->connected(); break;
		case 1: _t->disconnected(); break;
		case 2: _t->readyRead(); break;
		case 3: _t->onDebugAction(); break;
		case 4: _t->onAddOrderPressed(); break;
		case 5: _t->onCreateOrderPressed(); break;
		default:;
		}
	}
	Q_UNUSED(_a);
}

const QMetaObject MasterCPClient::staticMetaObject = {
	{ &QMainWindow::staticMetaObject, qt_meta_stringdata_MasterCPClient.data,
	  qt_meta_data_MasterCPClient,  qt_static_metacall, 0, 0}
};

const QMetaObject *MasterCPClient::metaObject() const
{
	return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MasterCPClient::qt_metacast(const char *_clname)
{
	if (!_clname) return 0;
	if (!strcmp(_clname, qt_meta_stringdata_MasterCPClient.stringdata))
		return static_cast<void*>(const_cast<MasterCPClient*>(this));
	return QMainWindow::qt_metacast(_clname);
}

int MasterCPClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
	_id = QMainWindow::qt_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;
	if (_c == QMetaObject::InvokeMetaMethod) {
		if (_id < 6)
			qt_static_metacall(this, _c, _id, _a);
		_id -= 6;
	}
	else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
		if (_id < 6)
			*reinterpret_cast<int*>(_a[0]) = -1;
		_id -= 6;
	}
	return _id;
}
QT_END_MOC_NAMESPACE