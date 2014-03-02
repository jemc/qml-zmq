
#ifndef QML_ZMQ_REQ_H
#define QML_ZMQ_REQ_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"


class ZMQ_Req : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void _() {};
  
  void* make_socket(void* context)
  { return zmq_socket(context, ZMQ_REQ); }
};


#endif
