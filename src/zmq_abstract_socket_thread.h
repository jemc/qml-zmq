
#ifndef QML_ZMQ_ABSTRACT_SOCKET_THREAD_H
#define QML_ZMQ_ABSTRACT_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>

#include "zmq_helper.h"
#include "zmq_context.h"


class ZMQ_AbstractSocketThread : public QThread, private ZMQ_Helper
{
  Q_OBJECT
  
  Q_PROPERTY(int socketType MEMBER socketType)
  Q_PROPERTY(ZMQ_Context* context MEMBER zctx)
  
  void _() {};
  
public:
  
  int socketType = 0;
  
  void* context = NULL;
  ZMQ_Context* zctx = NULL;
  
private:
  
  void run() Q_DECL_OVERRIDE {
    
    int num_pollables = 4;
    zmq_pollitem_t pollables[num_pollables];
    
    // Main socket
    void* ps_actual = zmq_socket(zctx->pointer, socketType);
    
    // Make all sockets pollable
    pollables[0].events = ZMQ_POLLIN;  pollables[0].socket = ps_actual;
    pollables[1].events = ZMQ_POLLIN;  pollables[1].socket = ps_send;
    pollables[2].events = ZMQ_POLLIN;  pollables[2].socket = ps_action;
    pollables[3].events = ZMQ_POLLIN;  pollables[3].socket = ps_kill;
    
    int not_dead = 1;
    
    while(not_dead) {
      
      if(zmq_poll(pollables, num_pollables, -1) != -1) {
        if     (pollables[0].revents) { // ps_actual
          emit receive(recv_array(ps_actual));
        }
        else if(pollables[1].revents) { // ps_send
          send_array(ps_actual, recv_array(ps_send));
          send_string(ps_send, "OKAY", 0);
        }
        else if(pollables[2].revents) { // ps_action
          QString action = recv_string(ps_action);
          QString string = recv_string(ps_action);
          
          if     (action == "BIND") { z_bind      (ps_actual, string); }
          else if(action == "UNBI") { z_unbind    (ps_actual, string); }
          else if(action == "CONN") { z_connect   (ps_actual, string); }
          else if(action == "DSCN") { z_disconnect(ps_actual, string); }
          else if(action == "SSOP") {
            QStringList parts = string.split("=");
            int          code = parts[0].toInt();
            char*        cstr = parts[1].toLocal8Bit().data();
            errchk("zmq_setsockopt",
                    zmq_setsockopt(ps_actual, code, cstr, parts[1].count()));
          }
          
          send_string(ps_action, "OKAY", 0);
        }
        else if(pollables[3].revents) { // ps_kill
          recv_array(ps_kill); // Clear the incoming message
          not_dead = 0;
        }
      }
      else not_dead = 0;
    }
    
    zmq_close(ps_actual);
    
    exit(0);
  }
  
signals:
  
  void receive(const QStringList& message);
  void sendCalled(const QStringList& message);
  
  void socketTypeChanged();
  
public slots:
  
  void send(const QStringList& message) {
    if(s_send!=NULL) {
      send_array(s_send,message);
      recv_array(s_send);
    }
    emit sendCalled(message);
  }
  
  void start() {
    make_inproc_sockets();
    QThread::start();
  }
  
  void stop() {
    if(s_kill != NULL) {
      send_string(s_kill, "", 0);
      quit();
      if(!wait(1000)) {
        terminate();
        qWarning() << this << "thread failed to stop.";
      }
      destroy_inproc_sockets();
    }
  }
  
  bool action(const QString& action, const QString& payload) {
    if(s_action == NULL) return 0;
    
    send_array(s_action, (QStringList() << action << payload));
    QStringList result = recv_array(s_action);
    return 1;
  }
  
private:
  
  void* ps_context = NULL;
  
  void* ps_send   = NULL;
  void*  s_send   = NULL;
  void* ps_action = NULL;
  void*  s_action = NULL;
  void* ps_kill   = NULL;
  void*  s_kill   = NULL;
  
  void make_inproc_sockets() {
    ps_context = zmq_ctx_new();
    
    // Socket to get send from user code and send to actual
    ps_send = zmq_socket(ps_context, ZMQ_REP);
     s_send = zmq_socket(ps_context, ZMQ_REQ);
    zmq_bind  (ps_send, "inproc://s_send");
    zmq_connect(s_send, "inproc://s_send");
    
    // Socket to stop and bind or connect or some other action
    ps_action = zmq_socket(ps_context, ZMQ_REP);
     s_action = zmq_socket(ps_context, ZMQ_REQ);
    zmq_bind  (ps_action, "inproc://s_action");
    zmq_connect(s_action, "inproc://s_action");
    
    // Socket to kill the thread
    ps_kill = zmq_socket(ps_context, ZMQ_PULL);
     s_kill = zmq_socket(ps_context, ZMQ_PUSH);
    zmq_bind  (ps_kill, "inproc://s_kill");
    zmq_connect(s_kill, "inproc://s_kill");
    
    // Use default global context for actual socket unless another was given
    if(zctx == NULL) zctx = ZMQ_Context::global();
  }
  
  void destroy_inproc_sockets() {
    zmq_close(ps_send);   ps_send   = NULL;
    zmq_close (s_send);    s_send   = NULL;
    zmq_close(ps_action); ps_action = NULL;
    zmq_close (s_action);  s_action = NULL;
    zmq_close(ps_kill);   ps_kill   = NULL;
    zmq_close (s_kill);    s_kill   = NULL;
    
    zmq_ctx_destroy(ps_context);
  }
  
public slots:
  
public:
  
  // ZMQ_AbstractSocketThread() { start(); };
  ~ZMQ_AbstractSocketThread() { stop(); };
  
};

#endif
