
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


QtObject {
  id: tracker
  // Redefine these functions to be called on each add or removal
  function    do_add(x) {}
  function do_remove(x) {}
  
  function _removeAllFrom(array, element) {
    var removed = false
    
    for(var i = array.length - 1; i >= 0; i--) {
      if(array[i] === element) {
        array.splice(i, 1);
        removed = true
      }
    }
    
    return removed
  }
  
  function add(x) {
    var removed = _removeAllFrom(set, x)
    set.push(x)
    
    if(!removed) setChanged()
  }
  function remove(x) {
    var removed = _removeAllFrom(set, x)
    
    if(removed) setChanged()
  }
  
  property var target
  property var property
  
  property var set
  property var _last_set: []
  property var _back_set: []
  
  property var _back_binding: Binding {
    target:   tracker.target
    property: tracker.property
    value:    tracker._back_set
  }
  
  function reapply() {
    _last_set = []
    setChanged()
  }
  
  function array_compare(a, b) {
    if(!(a instanceof Array)) return false
    if(!(b instanceof Array)) return false
    
    var i = a.length
    if (i != b.length) return false
    while (i--) {
        if (a[i] !== b[i]) return false
    }
    return true
  }
  
  onSetChanged: {
    var _tmp_set
    
    if(set === undefined)            _tmp_set = []
    else if(!(set instanceof Array)) _tmp_set = [set]
    else                             _tmp_set = set.slice(0)
    
    if(_last_set === undefined)      _last_set = [];
    
    _tmp_set.forEach(function(element, index, array) {
      if(!(element===undefined) && _last_set.indexOf(element) == -1)
        do_add(element)
    })
    
    _last_set.forEach(function(element, index, array) {
      if(!(element===undefined) && _tmp_set.indexOf(element) == -1)
        do_remove(element)
    })
    
    _last_set = _tmp_set
    
    if(!array_compare(_back_set, _tmp_set)) {
      _back_set = _tmp_set
    }
  }
  
}
