
#ifndef QML_ZMQ_ABSTRACT_SOCKET_THREAD_H
#define QML_ZMQ_ABSTRACT_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>

#include "zmq_helper.h"


class ZMQ_AbstractSocketThread : public QThread, private ZMQ_Helper
{
  Q_OBJECT
  void _() {};
  
  virtual void* make_socket(void* context) = 0;
  
  void run() Q_DECL_OVERRIDE
  {
    // Context from which all of this thread's sockets spring
    void *context = zmq_ctx_new();
    
    // Array of pollable items
    int num_pollables = 4;
    zmq_pollitem_t pollables[num_pollables];
    
    // Main socket and fake receive socket
    void *ps_actual = make_socket(context);
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
    
    // Socket to stop and bind or connect or some other action
    void *ps_kill = zmq_socket(context, ZMQ_PULL);
           s_kill = zmq_socket(context, ZMQ_PUSH);
    zmq_bind  (ps_kill, "inproc://s_kill");
    zmq_connect(s_kill, "inproc://s_kill");
    pollables[3].socket = ps_kill;
    pollables[3].events = ZMQ_POLLIN;
    
    int not_dead = 1;
    
    while(not_dead) {
      
      // printf("\nPolling in thread %p...\n", QThread::currentThread());
      
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
          
          // printf("ZMQ Socket Action: %s\n", action.toLocal8Bit().data());
          
          if(action == "BIND")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Binding on %s\n", c_string);
            errchk("run, zmq_bind", zmq_bind(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "CONN")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Connecting to %s\n", c_string);
            errchk("run, zmq_connect", zmq_connect(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else
            send_string(ps_action, QString("NOPE"), 0);
        }
        else if(pollables[3].revents) { // ps_kill
          recv_array(ps_kill); // Clear the incoming message
          // printf("ZMQ Socket Info: Killing\n");
          not_dead = 0;
        }
      }
    }
    
    ///
    // ZMQ Cleanup
    
    errchk("run, zmq_close ps_actual", zmq_close(ps_actual));
    
    errchk("run, zmq_close ps_send"  , zmq_close(ps_send));
    errchk("run, zmq_close s_send"   , zmq_close(s_send));   s_send   = NULL;
    
    errchk("run, zmq_close ps_action", zmq_close(ps_action));
    errchk("run, zmq_close s_action" , zmq_close(s_action)); s_action = NULL;
    
    errchk("run, zmq_close ps_kill",   zmq_close(ps_kill));
    errchk("run, zmq_close s_kill" ,   zmq_close(s_kill));   s_kill   = NULL;
    
    zmq_ctx_destroy(context);
    
    exit(0);
  }
  
signals:
  
  void receive(const QStringList& message);
  
public slots:
  
  void send(const QStringList& payload)
  {
    // printf("Send in thread %p...\n", QThread::currentThread());
    send_array(s_send, payload);
  }
  
  void bind(const QString& endpt)
  { action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { action("CONN", endpt); }
  
  void stop()
  {
    if(s_kill != NULL)
    {
      send_string(s_kill, QString(""), 0);
      quit();
      wait();
    }
  }
  
private:
  
  void* s_send   = NULL;
  void* s_action = NULL;
  void* s_kill   = NULL;
  
protected:
  
  void action(const char* action, const QString& payload)
  {
    if(s_action != NULL)
    {
      // printf("Action in thread %p...\n", QThread::currentThread());
      
      send_array(s_action, (QStringList() << action << payload));
      QStringList result = recv_array(s_action);
      
      // printf("Result: %s\n", result[0].toLocal8Bit().data());
    }
  }
  
public:
  
  ZMQ_AbstractSocketThread() { start(); };
  ~ZMQ_AbstractSocketThread() { stop(); };
  
};

#endif
