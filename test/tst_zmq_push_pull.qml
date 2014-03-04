
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_Push,Pull"
    
    
    ZPush {
      id: push
      binds: "ipc:///tmp/test_push_pull"
    }
    
    ZPull {
      id: pull
      connects: "ipc:///tmp/test_push_pull"
      
      property var lastReply:   []
      onReceive: lastReply = message
    }
    
    
    function test_messages() {
      wait(250)
      
      push.send("message")
      wait(100)
      compare(pull.lastReply, ["message"])
      
      push.send("message2")
      wait(100)
      compare(pull.lastReply, ["message2"])
    }
    
  }
}
