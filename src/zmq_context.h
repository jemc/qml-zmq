
#ifndef QML_ZMQ_CONTEXT_H
#define QML_ZMQ_CONTEXT_H

#include <QtQml>

#include <zmq.h>


class ZMQ_Context : public QObject
{
  Q_OBJECT
  
public:
  // Access a static "global" context for default use
  static ZMQ_Context* global() { static ZMQ_Context g; return &g; };
  
  // The libzmq pointer for this context
  void* pointer;
  
  ZMQ_Context() { pointer = zmq_ctx_new(); }
  ~ZMQ_Context() { zmq_ctx_destroy(pointer); }
};

#endif
