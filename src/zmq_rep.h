
#ifndef QML_ZMQ_REP_H
#define QML_ZMQ_REP_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"


class ZMQ_Rep : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void _() {};
  
  void run() Q_DECL_OVERRIDE
  {
    // Context from which all of this thread's sockets spring
    void *context = zmq_ctx_new();
    
    // Array of pollable items
    int num_pollables = 3;
    zmq_pollitem_t pollables[num_pollables];
    
    // Main socket and fake receive socket
    void *ps_actual = zmq_socket(context, ZMQ_REP);
             s_fake = zmq_socket(context, ZMQ_REQ);
    zmq_bind(ps_actual, "inproc://s_fake");
    zmq_connect(s_fake, "inproc://s_fake");
    pollables[0].socket = ps_actual;
    pollables[0].events = ZMQ_POLLIN;
    
    // Socket to get send from user code and send to actual
    void *ps_send = zmq_socket(context, ZMQ_PULL);
           s_send = zmq_socket(context, ZMQ_PUSH);
    zmq_bind  (ps_send, "inproc://s_send");
    zmq_connect(s_send, "inproc://s_send");
    pollables[1].socket = ps_send;
    pollables[1].events = ZMQ_POLLIN;
    
    // Socket to stop and bind or connect or some other action
    void *ps_action = zmq_socket(context, ZMQ_REP);
           s_action = zmq_socket(context, ZMQ_REQ);
    zmq_bind  (ps_action, "inproc://s_action");
    zmq_connect(s_action, "inproc://s_action");
    pollables[2].socket = ps_action;
    pollables[2].events = ZMQ_POLLIN;
    
    int not_dead = 1;
    
    while(not_dead) {
      
      printf("\nPolling in thread %p...\n", QThread::currentThread());
      
      if(zmq_poll(pollables, num_pollables, -1))
      {
        if(pollables[0].revents) { // Actual socket
          emit receive(recv_array(ps_actual));
        }
        else if(pollables[1].revents) { // ps_send
          send_array(ps_actual, recv_array(ps_send));
        }
        else if(pollables[2].revents) { // ps_action
          
          char* c_string;
          QString action = recv_string(ps_action);
          QString string = recv_string(ps_action);
          
          printf("ZMQ Socket Action: %s\n", action.toLocal8Bit().data());
          
          if(action == "BIND")
          {
            c_string = string.toLocal8Bit().data();
            printf("ZMQ Socket Info: Binding on %s\n", c_string);
            errchk(zmq_bind(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "CONN")
          {
            c_string = string.toLocal8Bit().data();
            printf("ZMQ Socket Info: Connecting to %s\n", c_string);
            errchk(zmq_connect(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "KILL")
          {
            printf("ZMQ Socket Info: Killing\n");
            not_dead = 0;
          }
          else
            send_string(ps_action, QString("NOPE"), 0);
        }
      }
    }
    
    send_string(ps_action, QString("DEAD"), 0);
    
    ///
    // Close sockets (except for s_action, which is closed in receiving thread)
    
    errchk(zmq_close(ps_actual));
    errchk(zmq_close(s_fake));
    
    errchk(zmq_close(ps_send));
    errchk(zmq_close(s_send));
    
    errchk(zmq_close(ps_action));
    // errchk(zmq_close(s_action));  (will be closed in receiving thread)
    
    
    exit(0);
  }
  
signals:
  
  void receive(const QStringList& data);
  
public slots:
  
  void fakeReceive(const QStringList& payload)
  {
    send_array(s_fake, payload);
  }
  
  void send(const QStringList& payload)
  {
    printf("Send in thread %p...\n", QThread::currentThread());
    send_array(s_send, payload);
  }
  
  void action(const char* action, const QString& payload)
  {
    printf("Action in thread %p...\n", QThread::currentThread());
    
    send_array(s_action, (QStringList() << action << payload));
    QStringList result = recv_array(s_action);
    
    // If thread is dead, close this socket - all others closed in thread
    if(result[0] == "DEAD")
      errchk(zmq_close(s_action));
    
    printf("Result: %s\n", result[0].toLocal8Bit().data());
  }
  
  void bind(const QString& endpt)
  { action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { action("CONN", endpt); }
  
  void stop()
  { action("KILL", QString("")); }
  
private:
  
  void* s_fake;
  void* s_send;
  void* s_action;
};


#endif
