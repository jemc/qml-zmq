
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"
#include "zmq_subscribing_socket_thread.h"


class ZMQ_Req : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REQ); };
};

class ZMQ_Rep : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REP); };
};

class ZMQ_Pub : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_PUB); };
};

class ZMQ_Sub : public ZMQ_SubscribingSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_SUB); };
};


#endif
