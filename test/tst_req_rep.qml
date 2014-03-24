
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZReq,ZRep"
    
    
    ZRep {
      id: rep
      binds: "ipc:///tmp/test"
      
      onReceive: send(message)
    }
    
    ZReq {
      id: req
      connects: "ipc:///tmp/test"
      
      property var lastReply:   []
      onReceive: lastReply = message
      
      property var lastSent: undefined
      onSendCalled: lastSent = message
    }
    
    
    function test_single_part() {
      wait(100)
      req.send("single-part message")
      wait(100)
      compare(req.lastReply, ["single-part message"])
    }
    
    function test_multi_part() {
      wait(100)
      req.send(["multi","part","message"])
      wait(100)
      compare(req.lastReply, ["multi","part","message"])
    }
    
    function test_binds() {
      compare(rep.binds, ["ipc:///tmp/test"])
      rep.bind("ipc:///tmp/other")
      compare(rep.binds, ["ipc:///tmp/test", "ipc:///tmp/other"])
      rep.bind("ipc:///tmp/other")
      compare(rep.binds, ["ipc:///tmp/test", "ipc:///tmp/other"])
      rep.unbind("ipc:///tmp/other")
      compare(rep.binds, ["ipc:///tmp/test"])
      rep.binds = ["ipc:///tmp/test", "ipc:///tmp/other"]
      compare(rep.binds, ["ipc:///tmp/test", "ipc:///tmp/other"])
      rep.binds = ["ipc:///tmp/test"]
      compare(rep.binds, ["ipc:///tmp/test"])
    }
    
    function test_connects() {
      compare(rep.connects, [])
      rep.connect("ipc:///tmp/other")
      compare(rep.connects, ["ipc:///tmp/other"])
      rep.connect("ipc:///tmp/other")
      compare(rep.connects, ["ipc:///tmp/other"])
      rep.disconnect("ipc:///tmp/other")
      compare(rep.connects, [])
      rep.connects = ["ipc:///tmp/other"]
      compare(rep.connects, ["ipc:///tmp/other"])
      rep.connects = []
      compare(rep.connects, [])
    }

    function test_onSendCalled() {
      req.send(["the","message"])
      compare(req.lastSent, ["the","message"])
    }
  }
}
