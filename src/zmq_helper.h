
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
    QByteArray bytes;
    bytes = string.toLatin1();
    int drop_term = 0;
    
    // "%%" at the end indicates not to transmit a null terminator
    if(bytes.endsWith("%%"))
    { drop_term = 1; bytes.chop(2); }
    
    // Un-escape all non-latin1 characters from percent encoding
    bytes = QByteArray::fromPercentEncoding(bytes);
    
    return errchk("send_string, zmq_send", zmq_send(socket, 
                  bytes.data(), bytes.count()+1-drop_term, flags));
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
    #define LATIN1_CHARS \
    " !\"#$&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
    
    const char* c_string;
    size_t c_size;
    QByteArray bytes;
    int drop_term = 0;
    zmq_msg_t msg;
    
    errchk("recv_string, zmq_msg_init", zmq_msg_init(&msg));
    errchk("recv_string, zmq_recvmsg", zmq_recvmsg(socket, &msg, 0));
    c_string = (char*)zmq_msg_data(&msg);
    c_size = zmq_msg_size(&msg);
    bytes = QByteArray(c_string, c_size);
    
    // Drop null terminator if present
    if(c_string[c_size-1]==0)
    { drop_term = 1; bytes.chop(1); }
    
    // Escape all non-latin1 characters with percent encoding
    bytes = bytes.toPercentEncoding(LATIN1_CHARS);
    
    // Append "%%" to indicate no null terminator was received
    if(!drop_term) bytes.append("%%");
    
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
};

#endif
