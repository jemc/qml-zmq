
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_Push,Pull"
    
    
    ZMQ_Push {
      id: push
      binds: "ipc:///tmp/test_push_pull"
    }
    
    ZMQ_Pull {
      id: pull
      property var lastReply:   []
      
      connects: "ipc:///tmp/test_push_pull"
      onReceive: lastReply = message
    }
    
    
    function test_messages() {
      push.send("message")
      wait(100)
      compare(pull.lastReply, ["message"])
      
      push.send("message2")
      wait(100)
      compare(pull.lastReply, ["message2"])
    }
    
  }
}
