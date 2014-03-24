
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "inproc"
    
    
    ZPull {
      id: pull
      binds: "inproc://test"
      
      property var lastMessage: undefined
      onReceive: lastMessage = message
    }
    ZPush {
      id: push
      connects: "inproc://test"
    }
    
    ZContext { id:otherContext }
    ZPull {
      id: otherpull
      context: otherContext
      binds: "inproc://test"
      
      property var lastMessage: undefined
      onReceive: lastMessage = message
    }
    ZPush {
      id: otherpush
      context: otherContext
      connects: "inproc://test"
    }
    
    function test_talk_but_no_crosstalk() {
      push.send(["the","message"])
      wait(100)
      compare(pull.lastMessage, ["the","message"])
      compare(otherpull.lastMessage, undefined)
      
      otherpush.send(["other","data"])
      wait(100)
      compare(pull.lastMessage, ["the","message"])
      compare(otherpull.lastMessage, ["other","data"])
    }
    
  }
}
