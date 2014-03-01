
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
    
    // Main socket
    void *actual = zmq_socket(context, ZMQ_REP);
       s_request = zmq_socket(context, ZMQ_REQ);
    zmq_bind   (actual,    "inproc://s_request");
    zmq_connect(s_request, "inproc://s_request");
    pollables[0].socket = actual;
    pollables[0].events = ZMQ_POLLIN;
    
    // Socket to stop and bind or connect or some other action
    void *ps_action = zmq_socket(context, ZMQ_REP);
           s_action = zmq_socket(context, ZMQ_REQ);
    zmq_bind  (ps_action, "inproc://s_action");
    zmq_connect(s_action, "inproc://s_action");
    pollables[1].socket = ps_action;
    pollables[1].events = ZMQ_POLLIN;
    
    
    int bufsize = 1024;
    int count = 0;
    char buffer[bufsize];
    
    
    while (1) {
      
      printf("\nWaiting...\n");
      printf("In thread %p...\n", QThread::currentThread());
      
      if(zmq_poll(pollables, num_pollables, -1))
      {
        if(pollables[0].revents) { // Actual socket
          count = zmq_recv(actual, buffer, bufsize, 0);
          buffer[count] = 0;
          printf("ZMQ Socket Info: Received request: %s\n", buffer);
          zmq_send(actual, "OKAY", 5, 0);
        }
        else if(pollables[1].revents) { // ps_action
          
          char* action;
          char* string;
          zmq_msg_t msg_action;
          zmq_msg_t msg_string;
          
          errchk(zmq_msg_init(&msg_action));
          errchk(zmq_recvmsg(ps_action, &msg_action, 0));
          action = (char*)zmq_msg_data(&msg_action);
          
          errchk(zmq_msg_init(&msg_string));
          errchk(zmq_recvmsg(ps_action, &msg_string, 0));
          string = (char*)zmq_msg_data(&msg_string);
          
          printf("ZMQ Socket Action: %s\n", action);
          
          if(strcmp(action, "BIND")==0)
          {
            printf("ZMQ Socket Info: Binding on %s\n", string);
            errchk(zmq_bind(actual, string));
          }
          else if(strcmp(action, "CONN")==0)
          {
            printf("ZMQ Socket Info: Connecting to %s\n", string);
            errchk(zmq_connect(actual, string));
          }
          else if(strcmp(action, "FAKE")==0)
          {
            printf("ZMQ Socket Info: Faking request: %s\n", string);
            errchk(zmq_connect(actual, string));
            emit request(QString(string));
          }
          
          
          zmq_send(ps_action, "OKAY", 5, 0);
          errchk(zmq_msg_close(&msg_action));
          errchk(zmq_msg_close(&msg_string));
        }
      }
    }
  }
  
  int errchk(int err)
  { if(err==-1) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
signals:
  
  void request(const QString& string);
  
public:
  
  void action(const char* action, const QString& payload)
  {
    char result [5]; // Expected result message size
    QByteArray bytes = payload.toLocal8Bit();
    zmq_send(s_action, action, strlen(action)+1, ZMQ_SNDMORE);
    zmq_send(s_action, bytes.data(), bytes.count()+1, 0);
    zmq_recv(s_action, result, sizeof(result), 0);
    printf("Result: %s\n", result);
  }
  
  void fakeRequest(const QString& payload)
  {
    char result [5]; // Expected result message size
    QByteArray bytes = payload.toLocal8Bit();
    zmq_send(s_request, bytes.data(), bytes.count()+1, 0);
    zmq_recv(s_request, result, sizeof(result), 0);
    printf("Result: %s\n", result);
  }
  
  void* s_request;
  void* s_action;
  void* context;
};

class ZMQ_Rep : public QObject
{
  Q_OBJECT
  void _() {};
  
signals:
  
  void request(const QString& string);
  
public slots:
  
  void start()
  { thread = new ZMQ_RepThread();
    
    QObject::connect(thread, &ZMQ_RepThread::request,
      [=](const QString& string) { emit request(string); });
    
    thread->start(); }
  
  void bind(const QString& endpt)
  { thread->action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { thread->action("CONN", endpt); }
  
  void fakeRequest(const QString& string)
  { thread->fakeRequest(string); }
  
private:
  
  ZMQ_RepThread* thread = NULL;
};


#endif
