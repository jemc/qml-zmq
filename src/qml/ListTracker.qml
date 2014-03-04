
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


QtObject {
  id: tracker
  // Redefine these functions to be called on each add or removal
  function    do_add(x) {}
  function do_remove(x) {}
  
  function _removeAllFrom(array, element) {
    for(var i = array.length - 1; i >= 0; i--)
      if(array[i] === element)
        array.splice(i, 1);
  }
  
  function add(x) {
    _removeAllFrom(set, x); set.push(x)
    setChanged()
    do_add(x)
  }
  function remove(x) {
    _removeAllFrom(set, x)
    setChanged()
    do_remove(x)
  }
  
  property var target
  property var property
  
  property var set
  property var _last_set: []
  
  property var _back_binding: Binding {
    target:   tracker.target
    property: tracker.property
    value:    tracker.set
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
        if(!(element===undefined) && _last_set.indexOf(element) == -1)
          do_add(element)
      })
      
      _last_set.forEach(function(element, index, array) {
        if(!(element===undefined) && set.indexOf(element) == -1)
          do_remove(element)
      })
      
      _last_set = set
    }
  }
}
