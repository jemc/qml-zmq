
#ifndef QML_ZMQ_ABSTRACT_SOCKET_THREAD_H
#define QML_ZMQ_ABSTRACT_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>


class ZMQ_AbstractSocketThread : public QThread
{
  Q_OBJECT
  void _() {};
  
  virtual void* make_socket(void* context) = 0;
  
  void run() Q_DECL_OVERRIDE
  {
    // Context from which all of this thread's sockets spring
    void *context = zmq_ctx_new();
    
    // Array of pollable items
    int num_pollables = 3;
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
            errchk(zmq_bind(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "CONN")
          {
            c_string = string.toLocal8Bit().data();
            // printf("ZMQ Socket Info: Connecting to %s\n", c_string);
            errchk(zmq_connect(ps_actual, c_string));
            send_string(ps_action, QString("OKAY"), 0);
          }
          else if(action == "KILL")
          {
            // printf("ZMQ Socket Info: Killing\n");
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
    
    errchk(zmq_close(ps_send));
    errchk(zmq_close(s_send));
    
    errchk(zmq_close(ps_action));
    // errchk(zmq_close(s_action));  (will be closed in receiving thread)
    
    
    exit(0);
  }
  
signals:
  
  void receive(const QStringList& data);
  
public slots:
  
  void send(const QStringList& payload)
  {
    // printf("Send in thread %p...\n", QThread::currentThread());
    send_array(s_send, payload);
  }
  
  void action(const char* action, const QString& payload)
  {
    // printf("Action in thread %p...\n", QThread::currentThread());
    
    send_array(s_action, (QStringList() << action << payload));
    QStringList result = recv_array(s_action);
    
    // If thread is dead, close this socket - all others closed in thread
    if(result[0] == "DEAD")
      errchk(zmq_close(s_action));
    
    // printf("Result: %s\n", result[0].toLocal8Bit().data());
  }
  
  void bind(const QString& endpt)
  { action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { action("CONN", endpt); }
  
  void stop()
  { action("KILL", QString("")); }
  
private:
  
  void* s_send;
  void* s_action;
  
protected:
  
  int errchk(int err)
  { if(err==-1) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
  int send_string(void* socket, const QString& string, int flags)
  {
    QByteArray bytes = string.toLocal8Bit();
    return errchk(zmq_send(socket, bytes.data(), bytes.count()+1, flags));
  }
  
  int send_array(void* socket, const QStringList& list)
  {
    int list_size = list.size()-1;
    
    for (int i = 0; i < list_size; ++i)
      errchk(send_string(socket, list[i], ZMQ_SNDMORE));
    
    return errchk(send_string(socket, list[list_size], 0));
  }
  
  QString recv_string(void* socket)
  {
    QString string;
    zmq_msg_t msg;
    
    errchk(zmq_msg_init(&msg));
    errchk(zmq_recvmsg(socket, &msg, 0));
    string = (char*)zmq_msg_data(&msg);
    errchk(zmq_msg_close(&msg));
    
    return string;
  }
  
  QStringList recv_array(void* socket)
  {
    QStringList list;
    int rcv_more = 1;
    size_t length;
    
    while(rcv_more)
    {
      list << recv_string(socket);
      zmq_getsockopt(socket, ZMQ_RCVMORE, &rcv_more, &length);
    }
    
    return list;
  }
};

#endif
