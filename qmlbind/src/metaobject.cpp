#include "metaobject.h"
#include "interface.h"
#include "wrapper.h"
#include <QJSValue>
#include <QMetaMethod>
#include <QDebug>

namespace QmlBind {

MetaObject::MetaObject(const QSharedPointer<const Interface> &interface) :
    mInterface(interface),
    mHandlers(interface->handlers()),
    mMethods(interface->methodMap()),
    mProperties(interface->propertyMap()),
    mPrototype(interface->metaObjectBuilder().toMetaObject())
{
    if (!mHandlers.call_method) {
        qFatal("qmlbind: call_method handler not specified");
    }
    if (!mHandlers.set_property) {
        qFatal("qmlbind: set_property handler not specified");
    }
    if (!mHandlers.get_property) {
        qFatal("qmlbind: get_property handler not specified");
    }

    d = mPrototype->d;
}

MetaObject::~MetaObject()
{
}

int MetaObject::metaCall(QObject *object, Call call, int index, void **argv) const
{
    index = object->QObject::qt_metacall(call, index ,argv);
    if (index < 0) {
        return index;
    }

    qmlbind_object_handle objectHandle = static_cast<Wrapper *>(object)->handle();

    switch(call) {
    case QMetaObject::ReadProperty:
    {
        int count = propertyCount() - propertyOffset();
        if (index < count) {
            QJSValue *value = mHandlers.get_property(objectHandle, mProperties[index].getterHandle);
            *static_cast<QJSValue *>(argv[0]) = *value;
            delete value;
        }
        index -= count;
        break;
    }
    case QMetaObject::WriteProperty:
    {
        int count = propertyCount() - propertyOffset();
        if (index < count) {
            mHandlers.set_property(objectHandle, mProperties[index].setterHandle, static_cast<QJSValue *>(argv[0]));
        }
        index -= count;
        break;
    }
    case QMetaObject::InvokeMetaMethod:
    {
        int count = methodCount() - methodOffset();
        if (index < count) {
            if (method(index + methodOffset()).methodType() == QMetaMethod::Signal) {
                QMetaObject::activate(object, this, index, argv);
            }
            else {
                Interface::Method method = mMethods[index];
                QJSValue *result = mHandlers.call_method(objectHandle, method.handle, method.arity, reinterpret_cast<QJSValue **>(argv + 1));
                if (argv[0]) {
                    *static_cast<QJSValue *>(argv[0]) = *result;
                }
                delete result;
            }
        }
        index -= count;
        break;
    }
    case QMetaObject::ResetProperty:
    case QMetaObject::QueryPropertyDesignable:
    case QMetaObject::QueryPropertyScriptable:
    case QMetaObject::QueryPropertyStored:
    case QMetaObject::QueryPropertyEditable:
    case QMetaObject::QueryPropertyUser:
    {
        int count = propertyCount() - propertyOffset();
        index -= count;
        break;
    }
    case QMetaObject::RegisterPropertyMetaType:
    {
        int count = propertyCount() - propertyOffset();

        if (index < count) {
            *static_cast<int *>(argv[0]) = -1;
        }
        index -= count;
        break;
    }
    default:
        break;
    }

    return index;
}

} // namespace QmlBind
