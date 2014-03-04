
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"
#include "zmq_subscribing_socket_thread.h"


class ZMQ_AbstractSocket : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  
  Q_PROPERTY(int socketType MEMBER socketType)
  
  int socketType = ZMQ_PUB;
  
  void make_socket() { action("MAKE", QString::number(ZMQ_PUB)); };
  
public:
  ZMQ_AbstractSocket() { make_socket(); };
};

class ZMQ_Push : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_PUSH)); };
  
public:
  ZMQ_Push() { make_socket(); };
};

class ZMQ_Pull : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_PULL)); };
  
public:
  ZMQ_Pull() { make_socket(); };
};

class ZMQ_Req : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_REQ)); };
  
public:
  ZMQ_Req() { make_socket(); };
};

class ZMQ_Rep : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_REP)); };
  
public:
  ZMQ_Rep() { make_socket(); };
};

class ZMQ_Pub : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_PUB)); };
  
public:
  ZMQ_Pub() { make_socket(); };
};

class ZMQ_Sub : public ZMQ_SubscribingSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_SUB)); };
  
public:
  ZMQ_Sub() { make_socket(); };
};

class ZMQ_XPub : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_XPUB)); };
  
public:
  ZMQ_XPub() { make_socket(); };
};

class ZMQ_XSub : public ZMQ_SubscribingSocketThread
{
  Q_OBJECT
  void make_socket() { action("MAKE", QString::number(ZMQ_XSUB)); };
  
public:
  ZMQ_XSub() { make_socket(); };
  
protected:
  
  virtual void pm_subscribe(const QString& topic)
  { send(QStringList(QString("%01").append(topic).append("%%"))); }
  
  virtual void pm_unsubscribe(const QString& topic)
  { send(QStringList(QString("%00").append(topic).append("%%"))); }
  
};


#endif
