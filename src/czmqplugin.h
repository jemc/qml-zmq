
#include <QQmlExtensionPlugin>
#include <qqml.h>


#include "zctx.h"


class CZMQPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType<ZMQ_Rep>(uri, 1, 0, "ZMQ_Rep");
    };
};
