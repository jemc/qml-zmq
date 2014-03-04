
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"
#include "zmq_subscribing_socket_thread.h"


class ZMQ_Toplevel : public QObject
{
  Q_OBJECT
  
public:
  
  Q_ENUMS(SocketTypes)
  enum SocketTypes {
    PAIR   = ZMQ_PAIR,
    PUB    = ZMQ_PUB,
    SUB    = ZMQ_SUB,
    REQ    = ZMQ_REQ,
    REP    = ZMQ_REP,
    DEALER = ZMQ_DEALER,
    ROUTER = ZMQ_ROUTER,
    PULL   = ZMQ_PULL,
    PUSH   = ZMQ_PUSH,
    XPUB   = ZMQ_XPUB,
    XSUB   = ZMQ_XSUB,
    STREAM = ZMQ_STREAM
  };
  
  Q_ENUMS(SocketOptions)
  enum SocketOptions {
    AFFINITY            = ZMQ_AFFINITY,
    IDENTITY            = ZMQ_IDENTITY,
    SUBSCRIBE           = ZMQ_SUBSCRIBE,
    UNSUBSCRIBE         = ZMQ_UNSUBSCRIBE,
    RATE                = ZMQ_RATE,
    RECOVERY_IVL        = ZMQ_RECOVERY_IVL,
    SNDBUF              = ZMQ_SNDBUF,
    RCVBUF              = ZMQ_RCVBUF,
    RCVMORE             = ZMQ_RCVMORE,
    FD                  = ZMQ_FD,
    EVENTS              = ZMQ_EVENTS,
    TYPE                = ZMQ_TYPE,
    LINGER              = ZMQ_LINGER,
    RECONNECT_IVL       = ZMQ_RECONNECT_IVL,
    BACKLOG             = ZMQ_BACKLOG,
    RECONNECT_IVL_MAX   = ZMQ_RECONNECT_IVL_MAX,
    MAXMSGSIZE          = ZMQ_MAXMSGSIZE,
    SNDHWM              = ZMQ_SNDHWM,
    RCVHWM              = ZMQ_RCVHWM,
    MULTICAST_HOPS      = ZMQ_MULTICAST_HOPS,
    RCVTIMEO            = ZMQ_RCVTIMEO,
    SNDTIMEO            = ZMQ_SNDTIMEO,
    LAST_ENDPOINT       = ZMQ_LAST_ENDPOINT,
    ROUTER_MANDATORY    = ZMQ_ROUTER_MANDATORY,
    TCP_KEEPALIVE       = ZMQ_TCP_KEEPALIVE,
    TCP_KEEPALIVE_CNT   = ZMQ_TCP_KEEPALIVE_CNT,
    TCP_KEEPALIVE_IDLE  = ZMQ_TCP_KEEPALIVE_IDLE,
    TCP_KEEPALIVE_INTVL = ZMQ_TCP_KEEPALIVE_INTVL,
    TCP_ACCEPT_FILTER   = ZMQ_TCP_ACCEPT_FILTER,
    IMMEDIATE           = ZMQ_IMMEDIATE,
    XPUB_VERBOSE        = ZMQ_XPUB_VERBOSE,
    ROUTER_RAW          = ZMQ_ROUTER_RAW,
    IPV6                = ZMQ_IPV6,
    MECHANISM           = ZMQ_MECHANISM,
    PLAIN_SERVER        = ZMQ_PLAIN_SERVER,
    PLAIN_USERNAME      = ZMQ_PLAIN_USERNAME,
    PLAIN_PASSWORD      = ZMQ_PLAIN_PASSWORD,
    CURVE_SERVER        = ZMQ_CURVE_SERVER,
    CURVE_PUBLICKEY     = ZMQ_CURVE_PUBLICKEY,
    CURVE_SECRETKEY     = ZMQ_CURVE_SECRETKEY,
    CURVE_SERVERKEY     = ZMQ_CURVE_SERVERKEY,
    PROBE_ROUTER        = ZMQ_PROBE_ROUTER,
    REQ_CORRELATE       = ZMQ_REQ_CORRELATE,
    REQ_RELAXED         = ZMQ_REQ_RELAXED,
    CONFLATE            = ZMQ_CONFLATE,
    ZAP_DOMAIN          = ZMQ_ZAP_DOMAIN,
    ROUTER_HANDOVER     = ZMQ_ROUTER_HANDOVER,
    TOS                 = ZMQ_TOS,
    IPC_FILTER_PID      = ZMQ_IPC_FILTER_PID,
    IPC_FILTER_UID      = ZMQ_IPC_FILTER_UID,
    IPC_FILTER_GID      = ZMQ_IPC_FILTER_GID,
    CONNECT_RID         = ZMQ_CONNECT_RID,
  };
};


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

// class ZMQ_Pub : public ZMQ_AbstractSocketThread
// { public: ZMQ_Pub() { socketType = ZMQ_PUB; start(); }; };

// class ZMQ_Sub : public ZMQ_SubscribingSocketThread
// { public: ZMQ_Sub() { socketType = ZMQ_SUB; start(); }; };

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
