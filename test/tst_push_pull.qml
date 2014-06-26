
import QtQuick 2.1
import QmlSpec 1.0

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZPush,ZPull"
    
    
    ZPush {
      id: push
      binds: "ipc:///tmp/test_push_pull"
    }
    
    ZPull {
      id: pull
      connects: "ipc:///tmp/test_push_pull"
      
      property var lastMessage:   []
      onReceive: lastMessage = message
    }
    
    function test_messages() {
      push.send("message")
      wait(100)
      compare(pull.lastMessage, ["message"])
      
      push.send("message2")
      wait(100)
      compare(pull.lastMessage, ["message2"])
    }
    
  }
}
