
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
  }
}
