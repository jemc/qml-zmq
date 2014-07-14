
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


ZMQ_AbstractSocket {
  id: socket
  context: ZContextDefault
  
  Component.onCompleted: {
    start()
    _bind_tracker.reapply()
    _connect_tracker.reapply()
  }
  
  property var binds: []
  property var connects: []
  
  property var _bind_tracker: ListTracker {
    target: socket;  set: socket.binds;  property: "binds"
    function    do_add(endpt) { action("BIND", endpt) }
    function do_remove(endpt) { action("UNBI", endpt) }
  }
  property var _connect_tracker: ListTracker {
    target: socket;  set: socket.connects;  property: "connects"
    function    do_add(endpt) { action("CONN", endpt) }
    function do_remove(endpt) { action("DSCN", endpt) }
  }
  
  function bind(endpt)       { _bind_tracker.add(endpt) }
  function unbind(endpt)     { _bind_tracker.remove(endpt) }
  function connect(endpt)    { _connect_tracker.add(endpt) }
  function disconnect(endpt) { _connect_tracker.remove(endpt) }
}
