#include "qmlbind/component.h"
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>

extern "C" {

qmlbind_component *qmlbind_component_new(qmlbind_engine *engine)
{
    return new QQmlComponent(engine);
}

void qmlbind_component_delete(qmlbind_component *component)
{
    delete component;
}

void qmlbind_component_load_path(qmlbind_component *component, const char *path)
{
    component->loadUrl(QUrl::fromLocalFile(path));
}

void qmlbind_component_set_data(qmlbind_component *component, const char *data, const char *path)
{
    component->setData(data, QUrl::fromLocalFile(path));
}

qmlbind_value *qmlbind_component_create(qmlbind_component *component)
{
    QObject *object = component->create();
    QQmlEngine::setObjectOwnership(object, QQmlEngine::JavaScriptOwnership);
    return new QJSValue(QQmlEngine::contextForObject(object)->engine()->newQObject(object));
}

}
