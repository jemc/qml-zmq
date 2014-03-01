
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
    
    // Main socket and fake request socket
    void *s_actual = zmq_socket(context, ZMQ_REP);
         s_request = zmq_socket(context, ZMQ_REQ);
    zmq_bind   (s_actual,  "inproc://s_request");
    zmq_connect(s_request, "inproc://s_request");
    pollables[0].socket = s_actual;
    pollables[0].events = ZMQ_POLLIN;
    
    // Socket to stop and bind or connect or some other action
    void *ps_action = zmq_socket(context, ZMQ_REP);
           s_action = zmq_socket(context, ZMQ_REQ);
    zmq_bind  (ps_action, "inproc://s_action");
    zmq_connect(s_action, "inproc://s_action");
    pollables[1].socket = ps_action;
    pollables[1].events = ZMQ_POLLIN;
    
    
    
    while (1) {
      
      printf("\nWaiting...\n");
      printf("In thread %p...\n", QThread::currentThread());
      
      if(zmq_poll(pollables, num_pollables, -1))
      {
        if(pollables[0].revents) { // Actual socket
          QStringList str_req = recv_array(s_actual);
          emit request(str_req);
          zmq_send(s_actual, "OKAY", 5, 0);
        }
        else if(pollables[1].revents) { // ps_action
          
          char* c_string;
          QString action = recv_string(ps_action);
          QString string = recv_string(ps_action);
          
          printf("ZMQ Socket Action: %s\n", action.toLocal8Bit().data());
          
          if(action == "BIND")
          {
            c_string = string.toLocal8Bit().data();
            printf("ZMQ Socket Info: Binding on %s\n", c_string);
            errchk(zmq_bind(s_actual, c_string));
          }
          else if(action == "CONN")
          {
            c_string = string.toLocal8Bit().data();
            printf("ZMQ Socket Info: Connecting to %s\n", c_string);
            errchk(zmq_connect(s_actual, c_string));
          }
          
          send_string(ps_action, QString("OKAY"), 0);
        }
      }
    }
  }
  
  int errchk(int err)
  { if(err==-1) printf("ZMQ Socket Error: %s\n", zmq_strerror(errno));
    return err; }
  
signals:
  
  void request(const QStringList& data);
  
public:
  
  void action(const char* action, const QString& payload)
  {
    send_array(s_action, (QStringList() << action << payload));
    QStringList result = recv_array(s_action);
    printf("Result: %s\n", result[0].toLocal8Bit().data());
  }
  
  void fakeRequest(const QStringList& payload)
  {
    send_array(s_request, payload);
    QStringList result = recv_array(s_request);
    printf("Result: %s\n", result[0].toLocal8Bit().data());
  }
  
  
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
  
  void* s_request;
  void* s_action;
};

class ZMQ_Rep : public QObject
{
  Q_OBJECT
  void _() {};
  
signals:
  
  void request(const QStringList& data);
  
public slots:
  
  void start()
  { thread = new ZMQ_RepThread();
    
    QObject::connect(thread, &ZMQ_RepThread::request,
      [=](const QStringList& data) { emit request(data); });
    
    thread->start(); }
  
  void bind(const QString& endpt)
  { thread->action("BIND", endpt); }
  
  void connect(const QString& endpt)
  { thread->action("CONN", endpt); }
  
  void fakeRequest(const QStringList& data)
  { thread->fakeRequest(data); }
  
private:
  
  ZMQ_RepThread* thread = NULL;
};


#endif
