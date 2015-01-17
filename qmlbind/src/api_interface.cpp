#include "qmlbind/interface.h"
#include "interface.h"
#include "util.h"
#include <private/qmetaobjectbuilder_p.h>
#include <private/qobject_p.h>

using namespace QmlBind;

extern "C" {

qmlbind_interface qmlbind_interface_new(qmlbind_class_handle classHandle, const char *className, qmlbind_interface_handlers handlers)
{
    return newSharedPointer(new Interface(className, classHandle, handlers));
}

void qmlbind_interface_release(qmlbind_interface interface)
{
    delete interface;
}

int qmlbind_interface_add_method(
    qmlbind_interface interface,
    qmlbind_method_handle handle,
    const char *name,
    int arity
)
{
    return (*interface)->addMethod(handle, name, arity).index();
}

int qmlbind_interface_add_signal(
    qmlbind_interface interface,
    const char *name,
    int arity,
    const char **params
)
{
    QList<QByteArray> paramList;
    for (int i = 0; i < arity; ++i) {
        paramList << params[i];
    }
    return (*interface)->addSignal(name, paramList).index();
}

int qmlbind_interface_add_property(
    qmlbind_interface interface,
    qmlbind_getter_handle getterHandle,
    qmlbind_setter_handle setterHandle,
    const char *name,
    int notifierSignalIndex
)
{
    return (*interface)->addProperty(getterHandle, setterHandle, name, notifierSignalIndex).index();
}

}
