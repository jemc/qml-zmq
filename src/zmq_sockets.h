
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"


class ZMQ_Rep : public ZMQ_AbstractSocketThread
{
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REP); };
};

class ZMQ_Req : public ZMQ_AbstractSocketThread
{
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REQ); };
};


#endif
