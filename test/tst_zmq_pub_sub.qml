
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_Pub,Sub"
    
    
    ZMQ_Pub {
      id: pub
      binds: "ipc:///tmp/test"
    }
    
    ZMQ_Sub {
      id: sub
      property var lastMessage:   []
      
      connects: "ipc:///tmp/test"
      onReceive: lastMessage = message
    }
    
    
    function test_message() {
      wait(100)
      sub.subscribe("topics")
      pub.send(["topics.x.y","message"])
      wait(100)
      compare(sub.lastMessage, ["topics.x.y","message"])
    }
  }
}
