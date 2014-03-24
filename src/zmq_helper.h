
#ifndef QML_ZMQ_HELPER_H
#define QML_ZMQ_HELPER_H

#include <QtQml>

#include <zmq.h>

#include "zmq_util.h"


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
  
  int send_bytes(void* socket, const QByteArray& bytes, int flags=0)
  {
    int rc = zmq_send(socket, bytes.constData(), bytes.count(), flags);
    if(rc==-1 && flags&ZMQ_DONTWAIT && errno==EAGAIN)
      return rc;
    else
      return errchk("send_bytes, zmq_send", rc);
  }
  
  int send_string(void* socket, const QString& string, int flags=0)
  {
    return send_bytes(socket, ZMQ_Util::convertDataToBytes(string), flags);
  }
  
  int send_array(void* socket, const QStringList& list, int flags=0)
  {
    int list_size = list.size()-1;
    
    for (int i = 0; i < list_size; ++i)
      send_string(socket, list[i], ZMQ_SNDMORE|flags);
    
    return send_string(socket, list[list_size], flags);
  }
  
  int send_bytes_array(void* socket, const QList<QByteArray>& list, int flags=0)
  {
    int list_size = list.size()-1;
    
    for (int i = 0; i < list_size; ++i)
      send_bytes(socket, list[i], ZMQ_SNDMORE|flags);
    
    return send_bytes(socket, list[list_size], flags);
  }
  
  QByteArray recv_bytes(void* socket)
  {
    QByteArray bytes;
    zmq_msg_t msg;
    
    errchk("recv_bytes, zmq_msg_init", zmq_msg_init(&msg));
    errchk("recv_bytes, zmq_recvmsg", zmq_recvmsg(socket, &msg, 0));
    bytes = QByteArray((char*)zmq_msg_data(&msg), zmq_msg_size(&msg));
    
    errchk("recv_bytes, zmq_msg_close", zmq_msg_close(&msg));
    
    return bytes;
  }
  
  QString recv_string(void* socket)
  {
    return ZMQ_Util::convertBytesToData(recv_bytes(socket));
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
  
  QList<QByteArray> recv_bytes_array(void* socket)
  {
    QList<QByteArray> list;
    int rcv_more = 1;
    size_t length;
    
    while(rcv_more)
    {
      list << recv_bytes(socket);
      zmq_getsockopt(socket, ZMQ_RCVMORE, &rcv_more, &length);
    }
    
    return list;
  }
  
  
  int z_bind(void* socket, const QByteArray& endpt)
  { return errchk("z_bind", zmq_bind(socket, endpt.constData())); }
  
  int z_connect(void* socket, const QByteArray& endpt)
  { return errchk("z_connect", zmq_connect(socket, endpt.constData())); }
  
  int z_unbind(void* socket, const QByteArray& endpt) // TODO: restore error checking
  // { return errchk("z_unbind", zmq_unbind(socket, endpt.constData())); }
  { return zmq_unbind(socket, endpt.constData()); }
  
  int z_disconnect(void* socket, const QByteArray& endpt) // TODO: restore error checking
  // { return errchk("z_disconnect", zmq_disconnect(socket, endpt.constData())); }
  { return zmq_disconnect(socket, endpt.constData()); }
};

#endif
