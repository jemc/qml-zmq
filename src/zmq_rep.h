
#ifndef QML_ZMQ_REP_H
#define QML_ZMQ_REP_H

#include <QtQml>

#include <zmq.h>


class ZMQ_RepThread : public QThread
{
  Q_OBJECT
  void _() {};
  
  void run() Q_DECL_OVERRIDE
  {
    // Main socket
    void *context = zmq_ctx_new();
    void *actual = zmq_socket(context, ZMQ_REP);
    
    // Socket to stop and bind
    void *ps_bind = zmq_socket(context, ZMQ_REP);
           s_bind = zmq_socket(context, ZMQ_REQ);
    zmq_bind  (ps_bind, "inproc://s_bind");
    zmq_connect(s_bind, "inproc://s_bind");
    
    zmq_pollitem_t items[2];
    items[0].socket = actual;
    items[0].events = ZMQ_POLLIN;
    items[1].socket = ps_bind;
    items[1].events = ZMQ_POLLIN;
    
    
    int bufsize = 1024;
    int count = 0;
    char buffer[bufsize];
    
    while (1) {
      
      printf("\nWaiting...\n");
      printf("In thread %p...\n", QThread::currentThread());
      
      if(zmq_poll(items, 2, -1))
      {
        if(items[0].revents) { // Actual socket
          count = zmq_recv(actual, buffer, bufsize, 0); buffer[count] = 0;
          printf("ZMQ Socket Info: Received request: %s\n", buffer);
          zmq_send(actual, "OKAY", 4, 0);
        } else if(items[1].revents) { // ps_bind
          count = zmq_recv(ps_bind, buffer, bufsize, 0); buffer[count] = 0;
          printf("ZMQ Socket Info: Binding on %s\n", buffer);
          zmq_bind(actual, buffer);
          zmq_send(ps_bind, "OKAY", 4, 0);
        }
      }
    }
  }
  
  int errchk(int err)
  { if(err) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
public:
  
  void bind(QString* endpt)
  {
    char buffer [10];
    QByteArray bytes = endpt->toLocal8Bit();
    zmq_send(s_bind, bytes.data(), bytes.count(), 0);
    zmq_recv(s_bind, buffer, 4, 0);
  }
  
  void* s_bind;
};

class ZMQ_Rep : public QObject
{
  Q_OBJECT
  void _() {};
  
public:
  
public slots:
  
  void start()
  {
    thread = new ZMQ_RepThread();
    thread->start();
  }
  
  void bind(QString endpt)
  { thread->bind(&endpt); }
  
private:
  
  ZMQ_RepThread* thread = NULL;
};


#endif
