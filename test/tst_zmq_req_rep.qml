
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_Req,Rep"
    
    
    ZMQ_Rep {
      id: rep
      binds: "ipc:///tmp/test"
      onReceive: send(message)
    }
    
    ZMQ_Req {
      id: req
      property var lastReply:   []
      
      connects: "ipc:///tmp/test"
      onReceive: lastReply = message
    }
    
    
    function test_single_part() {
      req.send("single-part message")
      wait(100)
      compare(req.lastReply, ["single-part message"])
    }
    
    function test_multi_part() {
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
  }
}
