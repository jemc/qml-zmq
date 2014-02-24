
#ifndef QML_ZMQ_REP_H
#define QML_ZMQ_REP_H

#include <QtQml>

#include <zmq.h>


class ZMQ_Rep : public QObject
{
    Q_OBJECT
    
public:
    
    ZMQ_Rep(QObject* parent = 0)
    { Q_UNUSED(parent); create(); };
    
public slots:
    
    void bind(QString endpt)
    { create();
      errchk(zmq_bind(socket, endpt.toLocal8Bit())); }
    
private:
    
    void create()
    { if(!ctx) ctx = zmq_ctx_new();
      if(!socket) socket = zmq_socket(ctx, ZMQ_REP); };
    
    int errchk(int err)
    { if(err) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
      return err; }
    
    void* ctx;
    void* socket;
};


#endif
