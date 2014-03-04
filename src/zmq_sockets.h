
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"
#include "zmq_subscribing_socket_thread.h"


class ZMQ_AbstractSocket : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  
  Q_PROPERTY(int socketType MEMBER socketType NOTIFY socketTypeChanged)
  
signals:
  
  void socketTypeChanged();
  
};

class ZMQ_Push : public ZMQ_AbstractSocketThread
{ public: ZMQ_Push() { socketType = ZMQ_PUSH; start(); }; };

class ZMQ_Pull : public ZMQ_AbstractSocketThread
{ public: ZMQ_Pull() { socketType = ZMQ_PULL; start(); }; };

class ZMQ_Req : public ZMQ_AbstractSocketThread
{ public: ZMQ_Req() { socketType = ZMQ_REQ; start(); }; };

class ZMQ_Rep : public ZMQ_AbstractSocketThread
{ public: ZMQ_Rep() { socketType = ZMQ_REP; start(); }; };

class ZMQ_Pub : public ZMQ_AbstractSocketThread
{ public: ZMQ_Pub() { socketType = ZMQ_PUB; start(); }; };

class ZMQ_Sub : public ZMQ_SubscribingSocketThread
{ public: ZMQ_Sub() { socketType = ZMQ_SUB; start(); }; };

class ZMQ_XPub : public ZMQ_AbstractSocketThread
{ public: ZMQ_XPub() { socketType = ZMQ_XPUB; start(); }; };

class ZMQ_XSub : public ZMQ_SubscribingSocketThread
{ public: ZMQ_XSub() { socketType = ZMQ_XSUB; start(); };

protected:
  
  virtual void pm_subscribe(const QString& topic)
  { send(QStringList(QString("%01").append(topic).append("%%"))); }
  
  virtual void pm_unsubscribe(const QString& topic)
  { send(QStringList(QString("%00").append(topic).append("%%"))); }
  
};


#endif
