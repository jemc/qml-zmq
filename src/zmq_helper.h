
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
  
  int send_string(void* socket, const QString& string, int flags=0)
  {
    QByteArray bytes;
    bytes = string.toLatin1();
    
    // Un-escape all non-latin1 characters from percent encoding
    bytes = QByteArray::fromPercentEncoding(bytes);
    
    int rc = zmq_send(socket, bytes.data(), bytes.count(), flags);
    if(rc==-1 && flags&ZMQ_DONTWAIT && errno==EAGAIN)
      return rc;
    else
      return errchk("send_string, zmq_send", rc);
  }
  
  int send_array(void* socket, const QStringList& list, int flags=0)
  {
    int list_size = list.size()-1;
    
    for (int i = 0; i < list_size; ++i)
      send_string(socket, list[i], ZMQ_SNDMORE|flags);
    
    return send_string(socket, list[list_size], flags);
  }
  
  QString recv_string(void* socket)
  {
    #define LATIN1_CHARS \
    " !\"#$&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
    
    QByteArray bytes;
    zmq_msg_t msg;
    
    errchk("recv_string, zmq_msg_init", zmq_msg_init(&msg));
    errchk("recv_string, zmq_recvmsg", zmq_recvmsg(socket, &msg, 0));
    bytes = QByteArray((char*)zmq_msg_data(&msg), zmq_msg_size(&msg));
    
    // Escape all non-latin1 characters with percent encoding
    bytes = bytes.toPercentEncoding(LATIN1_CHARS);
    
    errchk("recv_string, zmq_msg_close", zmq_msg_close(&msg));
    
    return QString::fromLatin1(bytes);
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
  
#define QSTR2C(_qstring) _qstring.toLatin1().data()
  
  int z_bind(void* socket, const QString& endpt)
  { return errchk("z_bind", zmq_bind(socket, QSTR2C(endpt))); }
  
  int z_connect(void* socket, const QString& endpt)
  { return errchk("z_connect", zmq_connect(socket, QSTR2C(endpt))); }
  
  int z_unbind(void* socket, const QString& endpt) // TODO: restore error checking
  // { return errchk("z_unbind", zmq_unbind(socket, QSTR2C(endpt))); }
  { return zmq_unbind(socket, QSTR2C(endpt)); }
  
  int z_disconnect(void* socket, const QString& endpt) // TODO: restore error checking
  // { return errchk("z_disconnect", zmq_disconnect(socket, QSTR2C(endpt))); }
  { return zmq_disconnect(socket, QSTR2C(endpt)); }
};

#endif
