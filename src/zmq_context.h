
#ifndef QML_ZMQ_CONTEXT_H
#define QML_ZMQ_CONTEXT_H

#include <QtQml>

#include <zmq.h>


class ZMQ_Context : public QObject
{
  Q_OBJECT
  
public:
  
  // The libzmq pointer for this context
  void* pointer = NULL;
  
  ZMQ_Context() {
    pointer = zmq_ctx_new();
  };
  
  ~ZMQ_Context() {
    if(pointer) zmq_ctx_destroy(pointer);
    pointer = NULL;
  };
};

#endif
