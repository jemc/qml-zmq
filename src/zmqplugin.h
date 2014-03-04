
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
        qmlRegisterType<ZMQ_Toplevel> (uri, 1, 0, "ZMQ");
        
        qmlRegisterType<ZMQ_AbstractSocketThread> (uri, 1, 0, "ZMQ_AbstractSocket");
    };
};
