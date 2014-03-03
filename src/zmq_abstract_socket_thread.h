
#ifndef QML_ZMQ_ABSTRACT_SOCKET_THREAD_H
#define QML_ZMQ_ABSTRACT_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>

#include "zmq_helper.h"


class ZMQ_AbstractSocketThread : public QThread, private ZMQ_Helper
{
  Q_OBJECT
  
  Q_PROPERTY(QStringList binds    READ binds    WRITE setBinds \
                                  NOTIFY bindsChanged)
  Q_PROPERTY(QStringList connects READ connects WRITE setConnects \
                                  NOTIFY connectsChanged)
  
  void _() {};
  
  virtual void* make_socket(void* context) = 0;
  
  void run() Q_DECL_OVERRIDE
  {
    void* context = zmq_ctx_new();
    
    int num_pollables = 4;
    zmq_pollitem_t pollables[num_pollables];
    
    // Main socket
    void* ps_actual = make_socket(context);
    
    // Make all sockets pollable
    pollables[0].socket = ps_actual;
    pollables[0].events = ZMQ_POLLIN;
    pollables[1].socket = ps_send;
    pollables[1].events = ZMQ_POLLIN;
    pollables[2].socket = ps_action;
    pollables[2].events = ZMQ_POLLIN;
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
          else if(action == "UNBI")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Unbinding on %s\n", c_string);
            errchk("run, zmq_unbind", zmq_unbind(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "CONN")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Connecting to %s\n", c_string);
            errchk("run, zmq_connect", zmq_connect(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "DSCN")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Disconnecting from %s\n", c_string);
            // TODO: replace error checking somehow
            // errchk("run, zmq_disconnect", zmq_disconnect(ps_actual, c_string));
            zmq_disconnect(ps_actual, c_string);
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "SSOP")
          {
            QStringList parts = string.split("=");
            int code  = parts[0].toInt();
            c_string  = parts[1].toLocal8Bit().data();
            // printf("ZMQ Socket Info: Setting option %d to %s\n", code, c_string);
            errchk("run, zmq_setsockopt",
                   zmq_setsockopt(ps_actual, code, c_string, parts[1].count()));
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
    
    zmq_close(ps_actual);
    zmq_ctx_destroy(context);
    
    exit(0);
  }
  
signals:
  
  void receive(const QStringList& message);
  
  void bindsChanged();
  void connectsChanged();
  
public slots:
  
  void send(const QStringList& payload)
  { send_array(s_send, payload); }
  
  void bind(const QString& endpt)
  { action("BIND", endpt);
    m_binds.removeAll(endpt); m_binds.append(endpt);
    emit bindsChanged(); }
  
  void unbind(const QString& endpt)
  { action("UNBI", endpt);
    m_binds.removeAll(endpt);
    emit bindsChanged(); }
  
  void connect(const QString& endpt)
  { action("CONN", endpt);
    m_conns.removeAll(endpt); m_conns.append(endpt);
    emit connectsChanged(); }
  
  void disconnect(const QString& endpt)
  { action("DSCN", endpt);
    m_conns.removeAll(endpt);
    emit connectsChanged(); }
  
  void stop()
  { if(s_kill != NULL)
    { send_string(s_kill, QString(""), 0);
      quit();
      wait(); } }
  
private:
  
  void* ps_context = NULL;
  
  void* ps_send   = NULL;
  void*  s_send   = NULL;
  void* ps_action = NULL;
  void*  s_action = NULL;
  void* ps_kill   = NULL;
  void*  s_kill   = NULL;
  
  void make_inproc_sockets()
  {
    ps_context = zmq_ctx_new();
    
    // Socket to get send from user code and send to actual
    ps_send = zmq_socket(ps_context, ZMQ_PULL);
     s_send = zmq_socket(ps_context, ZMQ_PUSH);
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
  }
  
  void destroy_inproc_sockets()
  {
    zmq_close(ps_send);   ps_send   = NULL;
    zmq_close (s_send);    s_send   = NULL;
    zmq_close(ps_action); ps_action = NULL;
    zmq_close (s_action);  s_action = NULL;
    zmq_close(ps_kill);   ps_kill   = NULL;
    zmq_close (s_kill);    s_kill   = NULL;
    
    zmq_ctx_destroy(ps_context);
  }
  
  QStringList m_binds;
  QStringList m_conns;
  
public:
  
  QStringList binds() { return m_binds; }
  
  void setBinds(QStringList binds)
  {
    foreach(const QString &s, m_binds.toSet()-binds.toSet()) action("UNBI", s);
    foreach(const QString &s, binds.toSet()-m_binds.toSet()) action("BIND", s);
    m_binds = binds;
  }
  
  QStringList connects() { return m_conns; }
  
  void setConnects(QStringList conns)
  {
    foreach(const QString &s, m_conns.toSet()-conns.toSet()) action("DSCN", s);
    foreach(const QString &s, conns.toSet()-m_conns.toSet()) action("CONN", s);
    m_conns = conns;
  }
  
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
  
  
  
  ZMQ_AbstractSocketThread() { make_inproc_sockets(); start(); };
  ~ZMQ_AbstractSocketThread() { stop(); destroy_inproc_sockets(); };
  
};

#endif
