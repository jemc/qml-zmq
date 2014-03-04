
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


ZMQ_Socket { socketType: ZMQ.SUB
  
  id: socket
  
  property var subscriptions: []
  
  property var _sub_tracker: ListTracker {
    target: socket;  set: subscriptions;  property: "subscriptions"
    function    do_add(topic) { action("SSOP", "%1=%2".arg(ZMQ.SUBSCRIBE)  .arg(topic)) }
    function do_remove(topic) { action("SSOP", "%1=%2".arg(ZMQ.UNSUBSCRIBE).arg(topic)) }
  }
  
  function subscribe(topic)   { _sub_tracker.add(topic) }
  function unsubscribe(topic) { _sub_tracker.remove(topic) }
  onStarted: _sub_tracker.reapply()
}
