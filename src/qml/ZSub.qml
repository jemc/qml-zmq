
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0

ZMQ_Socket { socketType: ZMQ.SUB
  
  id: socket
  
  property var priv: QtObject {
    function   subscribe(topic) { action("SSOP", "%1=%2".arg(ZMQ.SUBSCRIBE)  .arg(topic)) }
    function unsubscribe(topic) { action("SSOP", "%1=%2".arg(ZMQ.UNSUBSCRIBE).arg(topic)) }
  }
  
  function   subscribe(topic) {
    for(var i = subscriptions.length - 1; i >= 0; i--) {
      if(subscriptions[i] === topic) {
        subscriptions.splice(i, 1);
      }
    }
    subscriptions.push(topic)
    subscriptionsChanged()
    priv.subscribe(topic)
  }
  function unsubscribe(topic) {
    for(var i = subscriptions.length - 1; i >= 0; i--) {
      if(subscriptions[i] === topic) {
        subscriptions.splice(i, 1);
      }
    }
    subscriptionsChanged()
    priv.unsubscribe(topic)
  }
  
  property var subscriptions: []
  property var _last_subscriptions: []
  
  Component.onCompleted: {
    _last_subscriptions = []
    subscriptionsChanged()
  }
  
  onSubscriptionsChanged: {
    if(!(_last_subscriptions instanceof Array))
      _last_subscriptions = []
    if(!(subscriptions instanceof Array))
      subscriptions = [subscriptions]
    else
    {
      subscriptions.forEach(function(element, index, array) {
        if(_last_subscriptions.indexOf(element) == -1)
          priv.subscribe(element)
      })
      
      _last_subscriptions.forEach(function(element, index, array) {
        if(subscriptions.indexOf(element) == -1)
          priv.unsubscribe(element)
      })
      
      _last_subscriptions = subscriptions
    }
  }
}
