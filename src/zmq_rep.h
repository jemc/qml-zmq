
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
    // Context from which all sockets spring
    void *context = zmq_ctx_new();
    
    // Array of pollable items
    int num_pollables = 2;
    zmq_pollitem_t pollables[num_pollables];

#define ZMQML_MAKE_SOCKS(idx, s, st, ps, pst, path) \
    void *ps = zmq_socket(context, pst);            \
           s = zmq_socket(context, st);             \
    zmq_bind  (ps, path);                           \
    zmq_connect(s, path);                           \
    pollables[idx].socket = ps;                     \
    pollables[idx].events = ZMQ_POLLIN;             
    
    // Main socket
    void *actual = zmq_socket(context, ZMQ_REP);
    pollables[0].socket = actual;
    pollables[0].events = ZMQ_POLLIN;
    
    // Socket to stop and bind or connect or some other action
    ZMQML_MAKE_SOCKS(1, s_action, ZMQ_REQ, ps_action, ZMQ_REP, "inproc://s_action")
    
    
    int bufsize = 1024;
    int count = 0;
    char buffer[bufsize];
    
    
    while (1) {
      
      printf("\nWaiting...\n");
      printf("In thread %p...\n", QThread::currentThread());
      
#define ZMQML_RECV_BUFFER(s)                   \
      count = zmq_recv(s, buffer, bufsize, 0); \
      buffer[count] = 0;                       
      
      if(zmq_poll(pollables, num_pollables, -1))
      {
        if(pollables[0].revents) { // Actual socket
          ZMQML_RECV_BUFFER(actual)
          printf("ZMQ Socket Info: Received request: %s\n", buffer);
          zmq_send(actual, "OKAY", 4, 0);
        } else if(pollables[1].revents) { // ps_action
          
          // int64_t more;
          // size_t more_size = sizeof more;
          // do {
          //   /* Create an empty ØMQ message to hold the message part */
          //   zmq_msg_t part;
          //   errchk(zmq_msg_init(&part));
          //   /* Block until a message is available to be received from socket */
          //   printf("ZMQ Socket Thing\n");
          //   errchk(zmq_msg_recv(&part, ps_action, 0)==-1);
          //   printf("ZMQ Socket Thing\n");
          //   /* Determine if more message parts are to follow */
          //   errchk(zmq_getsockopt(ps_action, ZMQ_RCVMORE, &more, &more_size));
          //   zmq_msg_close(&part);
          // } while (more);
          
          
          ZMQML_RECV_BUFFER(ps_action)
          printf("ZMQ Socket Info: Binding on %s\n", buffer);
          errchk(zmq_bind(actual, buffer));
          zmq_send(ps_action, "OKAY", 4, 0);
        }
      }
    }
  }
  
  int errchk(int err)
  { if(err) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
public:
  
  void bind(const QString& endpt)
  {
    char buffer [4];
    QByteArray bytes = endpt.toLocal8Bit();
    zmq_send(s_action, bytes.data(), bytes.count(), 0);
    zmq_recv(s_action, buffer, 4, 0);
  }
  
  // void connect(const QString& endpt)
  // {
  //   char buffer [4];
  //   QByteArray bytes = endpt.toLocal8Bit();
  //   zmq_send(s_conn, bytes.data(), bytes.count(), 0);
  //   zmq_recv(s_conn, buffer, 4, 0);
  // }
  
  void* s_action;
  // void* s_conn;
};

class ZMQ_Rep : public QObject
{
  Q_OBJECT
  void _() {};
  
public:
  
public slots:
  
  void start()
  { thread = new ZMQ_RepThread(); thread->start(); }
  
  void bind(const QString& endpt)
  { thread->bind(endpt); }
  
  // void connect(const QString& endpt)
  // { thread->connect(endpt); }
  
private:
  
  ZMQ_RepThread* thread = NULL;
};


#endif
