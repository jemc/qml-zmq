
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_inproc"
    
    
    ZSub {
      id: sub
      binds: "inproc://test"
      subscriptions: ""
      
      property var lastMessage: undefined
      onReceive: lastMessage = message
    }
    ZPub {
      id: pub
      connects: "inproc://test"
    }
    
    ZContext { id:otherContext }
    ZSub {
      id: othersub
      context: otherContext
      binds: "inproc://test"
      subscriptions: ""
      
      property var lastMessage: undefined
      onReceive: lastMessage = message
    }
    ZPub {
      id: otherpub
      context: otherContext
      connects: "inproc://test"
    }
    
    
    function test_talk_but_no_crosstalk() {
      wait(250)
      
      pub.send(["the","message"])
      wait(100)
      compare(sub.lastMessage, ["the","message"])
      compare(othersub.lastMessage, undefined)
      
      otherpub.send(["other","data"])
      wait(100)
      compare(sub.lastMessage, ["the","message"])
      compare(othersub.lastMessage, ["other","data"])
    }
    
  }
}
