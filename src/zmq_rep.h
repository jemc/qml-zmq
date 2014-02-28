
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
          zmq_send(actual, "OKAY", 4, 0);
        }
        else if(pollables[1].revents) { // ps_action
          
          char* command;
          char* payload;
          zmq_msg_t msg_command;
          zmq_msg_t msg_payload;
          
          errchk(zmq_msg_init(&msg_command));
          count = errchk(zmq_recvmsg(ps_action, &msg_command, 0));
          command = (char*)zmq_msg_data(&msg_command);
          
          errchk(zmq_msg_init(&msg_payload));
          count = errchk(zmq_recvmsg(ps_action, &msg_payload, 0));
          payload = (char*)zmq_msg_data(&msg_payload);
          
          printf("ZMQ Socket Action: %s\n", command);
          
          if(strcmp(command, "BIND")==0)
          {
            printf("ZMQ Socket Info: Binding on %s\n", payload);
            errchk(zmq_bind(actual, payload));
          }
          else if(strcmp(command, "CONN")==0)
          {
            printf("ZMQ Socket Info: Connecting to %s\n", payload);
            errchk(zmq_bind(actual, payload));
          }
          
          zmq_send(ps_action, "OKAY", 4, 0);
          errchk(zmq_msg_close(&msg_command));
          errchk(zmq_msg_close(&msg_payload));
        }
      }
    }
  }
  
  int errchk(int err)
  { if(err==-1) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
public:
  
  void action(const char* action, const QString& payload)
  {
    char result [5];
    QByteArray bytes = payload.toLocal8Bit();
    zmq_send(s_action, action, strlen(action)+1, ZMQ_SNDMORE);
    zmq_send(s_action, bytes.data(), bytes.count()+1, 0);
    zmq_recv(s_action, result, sizeof(result), 0);
    printf("Result: %s\n", result);
  }
  
  void* s_action;
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
  { thread->action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { thread->action("CONN", endpt); }
  
private:
  
  ZMQ_RepThread* thread = NULL;
};


#endif
