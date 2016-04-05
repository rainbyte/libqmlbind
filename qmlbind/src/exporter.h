#pragma once

#include "qmlbind/qmlbind_global.h"
#include "backref.h"
#include <private/qmetaobjectbuilder_p.h>
#include <QSharedPointer>

namespace QmlBind {

class Exporter
{
public:

    struct Method {
        QByteArray name;
        int arity;
    };

    struct Property {
        QByteArray name;
    };

    Exporter(const char *className, const Backref &classRef);

    void addMethod(const char *name, int arity);
    void addSignal(const char *name, const QList<QByteArray> &args);
    void addProperty(const char *name, const char *notifier);
    std::shared_ptr<Interface> interface() const { return mClassRef.interface(); }

    Backref classRef() const { return mClassRef; }
    const QMetaObjectBuilder &metaObjectBuilder() const { return mBuilder; }
    QHash<int, Method> methodMap() const { return mMethodMap; }
    QHash<int, Property> propertyMap() const { return mPropertyMap; }
    QHash<QByteArray, int> signalIndexMap() const { return mSignalIndexMap; }

private:

    Backref mClassRef;
    QHash<int, Method> mMethodMap;
    QHash<int, Property> mPropertyMap;
    QHash<QByteArray, int> mSignalIndexMap;

    QMetaObjectBuilder mBuilder;
};

} // namespace QmlBind
