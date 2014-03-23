
#include "zmq_util.h"


QObject* ZMQ_Util::qmlAttachedProperties(QObject* object)
{
    return new ZMQ_UtilAttached(object);
}
