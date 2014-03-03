
#ifndef QML_ZMQ_SOCKETS_H
#define QML_ZMQ_SOCKETS_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"


class ZMQ_Req : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REQ); };
};

class ZMQ_Rep : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_REP); };
};

class ZMQ_Pub : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_PUB); };
};

class ZMQ_Sub : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void* make_socket(void* context) { return zmq_socket(context, ZMQ_SUB); };
  
  Q_PROPERTY(QStringList subscriptions READ subscriptions WRITE setSubscriptions \
                                  NOTIFY subscriptionsChanged)
  
  QStringList m_subs;
  
  void pm_subscribe(const QString& topic)
  { action("SSOP", QString("%1=%2").arg(ZMQ_SUBSCRIBE).arg(topic)); }
  
  void pm_unsubscribe(const QString& topic)
  { action("SSOP", QString("%1=%2").arg(ZMQ_UNSUBSCRIBE).arg(topic)); }
  
signals:
  
  void subscriptionsChanged();
  
public:
  
  QStringList subscriptions() { return m_subs; }
  
  void setSubscriptions(QStringList subs)
  {
    foreach(const QString &s, m_subs.toSet()-subs.toSet()) pm_unsubscribe(s);
    foreach(const QString &s, subs.toSet()-m_subs.toSet())   pm_subscribe(s);
    m_subs = subs;
    emit subscriptionsChanged();
  }
  
public slots:
  
  void subscribe(const QString& topic)
  {
    pm_subscribe(topic);
    m_subs.removeAll(topic); m_subs.append(topic);
    emit subscriptionsChanged();
  }
  
  void unsubscribe(const QString& topic)
  {
    pm_unsubscribe(topic);
    m_subs.removeAll(topic);
    emit subscriptionsChanged();
  }
};


#endif
