
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test_rep
    name: "ZMQ_Rep"
    
    property var reply:   []
    
    ZMQ_Rep { id: rep
      binds: "ipc:///tmp/test"
      onReceive: send(message)
    }
    
    ZMQ_Req { id: req
      onReceive: test_rep.reply = message
    }
    
    function test_it() {
      req.connect("ipc:///tmp/test")
      wait(500)
      
      req.send("single-part message")
      wait(500)
      compare(reply, ["single-part message"])
      
      req.send(["multi","part","message"])
      wait(500)
      compare(reply, ["multi","part","message"])
    }
  }
}
