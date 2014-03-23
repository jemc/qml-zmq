
#include <QQmlExtensionPlugin>

#include <qqml.h>

#include "zmq_toplevel.h"
#include "zmq_context.h"
#include "zmq_abstract_socket_thread.h"
#include "zmq_util.h"


class ZMQPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    
public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType<ZMQ_Toplevel> (uri, 1, 0, "ZMQ");
        
        qmlRegisterType<ZMQ_AbstractSocketThread> (uri, 1, 0, "ZMQ_AbstractSocket");
        
        qmlRegisterType<ZMQ_Context> (uri, 1, 0, "ZContext");
        
        qmlRegisterType<ZMQ_Util> (uri, 1, 0, "ZUtil");
    };
};
