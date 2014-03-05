
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_inproc"
    
    
    ZReq {
      id: req
      binds: "inproc://test"
      
      property var lastReply:   []
      onReceive: lastReply = message
    }
    
    ZRep {
      id: rep
      connects: "inproc://test"
      
      onReceive: send(message)
    }
    
    
    function test_messages() {
      wait(250)
      
      req.send("message")
      wait(100)
      compare(req.lastReply, ["message"])
    }
    
  }
}
