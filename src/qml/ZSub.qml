
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0

ZMQ_Socket { socketType: ZMQ.SUB
  
  id: socket
  
  property var priv: QtObject {
    function    add(topic) { action("SSOP", "%1=%2".arg(ZMQ.SUBSCRIBE)  .arg(topic)) }
    function remove(topic) { action("SSOP", "%1=%2".arg(ZMQ.UNSUBSCRIBE).arg(topic)) }
  }
  
  
  function subscribe(topic)   { subtracker.add(topic) }
  function unsubscribe(topic) { subtracker.remove(topic) }
  
  onStarted: subtracker.reapply()
  
  property var subscriptions: []
  
  property var subtracker: QtObject {
    property var _priv: priv
    
    function _removeAllFrom(array, element) {
      for(var i = array.length - 1; i >= 0; i--)
        if(array[i] === element)
          array.splice(i, 1);
    }
    
    function add(topic) {
      _removeAllFrom(set, topic)
      set.push(topic)
      setChanged()
      _priv.add(topic)
    }
    function remove(topic) {
      _removeAllFrom(set, topic)
      setChanged()
      _priv.remove(topic)
    }
    
    property var set: subscriptions
    property var _last_set: []
    
    property var somebinding: Binding {
      target: socket
      property: "subscriptions"
      value: socket.subtracker.set
    }
    
    function reapply() {
      _last_set = []
      setChanged()
    }
    
    onSetChanged: {
      if(!(_last_set instanceof Array))
        _last_set = []
      if(!(set instanceof Array))
        set = [set]
      else
      {
        set.forEach(function(element, index, array) {
          if(_last_set.indexOf(element) == -1)
            _priv.add(element)
        })
        
        _last_set.forEach(function(element, index, array) {
          if(set.indexOf(element) == -1)
            _priv.remove(element)
        })
        
        _last_set = set
      }
    }
  }
}
