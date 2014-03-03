
#ifndef QML_ZMQ_SUBSCRIBING_SOCKET_THREAD_H
#define QML_ZMQ_SUBSCRIBING_SOCKET_THREAD_H

#include <QtQml>

#include <zmq.h>

#include "zmq_abstract_socket_thread.h"


class ZMQ_SubscribingSocketThread : public ZMQ_AbstractSocketThread
{
  Q_OBJECT
  void _() {};
  
  Q_PROPERTY(QStringList subscriptions READ subscriptions     \
                                       WRITE setSubscriptions \
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
