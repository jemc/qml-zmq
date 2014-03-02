
#ifndef QML_ZMQ_HELPER_H
#define QML_ZMQ_HELPER_H

#include <QtQml>

#include <zmq.h>


class ZMQ_Helper
{
  void _() {};
  
protected:
  
  int errchk(const char* description, int err)
  {
    if(err==-1)
    {
      printf("ZMQ Socket Error in %s: %s\n", description, zmq_strerror(errno));
      exit(1);
    }
    return err;
  }
  
  int send_string(void* socket, const QString& string, int flags)
  {
    QByteArray bytes = string.toLocal8Bit();
    return errchk("send_string, zmq_send",
                  zmq_send(socket, bytes.data(), bytes.count()+1, flags));
  }
  
  int send_array(void* socket, const QStringList& list)
  {
    int list_size = list.size()-1;
    
    for (int i = 0; i < list_size; ++i)
      send_string(socket, list[i], ZMQ_SNDMORE);
    
    return send_string(socket, list[list_size], 0);
  }
  
  QString recv_string(void* socket)
  {
    QString string;
    zmq_msg_t msg;
    
    errchk("recv_string, zmq_msg_init", zmq_msg_init(&msg));
    errchk("recv_string, zmq_recvmsg", zmq_recvmsg(socket, &msg, 0));
    string = (char*)zmq_msg_data(&msg);
    errchk("recv_string, zmq_msg_close", zmq_msg_close(&msg));
    
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
