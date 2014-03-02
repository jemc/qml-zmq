
#ifndef QML_ZMQ_ABSTRACT_SOCKET_THREAD_H
#define QML_ZMQ_ABSTRACT_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>


class ZMQ_AbstractSocketThread : public QThread
{
  Q_OBJECT
  void _() {};
  
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
