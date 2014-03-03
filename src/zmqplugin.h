
#include <QQmlExtensionPlugin>

#include <qqml.h>

#include "zmq_sockets.h"


class ZMQPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType<ZMQ_Req>(uri, 1, 0, "ZMQ_Req");
        qmlRegisterType<ZMQ_Rep>(uri, 1, 0, "ZMQ_Rep");
        qmlRegisterType<ZMQ_Pub>(uri, 1, 0, "ZMQ_Pub");
        qmlRegisterType<ZMQ_Sub>(uri, 1, 0, "ZMQ_Sub");
    };
};
