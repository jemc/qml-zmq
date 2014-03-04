
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_Pub,Sub"
    
    
    ZPub {
      id: pub
      
      // binds
      Component.onCompleted: bind("ipc:///tmp/test")
    }
    
    ZSub {
      id: sub
      property var lastMessage:   []
      
      // connects: "ipc:///tmp/test"
      subscriptions: "topic"
      onReceive: lastMessage = message
      
      Component.onCompleted: {
        connect("ipc:///tmp/test")
        // subscribe("topic")
      }
    }
    
    
    function test_messages() {
      // pub.bind("ipc:///tmp/test")
      wait(250)
      
      pub.send(["topic.x.y","message"])
      wait(100)
      compare(sub.lastMessage, ["topic.x.y","message"])
      
      pub.send(["topic.zzz","message2"])
      wait(100)
      compare(sub.lastMessage, ["topic.zzz","message2"])
    }
    
    
    function test_subscriptions() {
      compare(sub.subscriptions, ["topic"])
      sub.subscribe("other")
      compare(sub.subscriptions, ["topic", "other"])
      sub.subscribe("other")
      compare(sub.subscriptions, ["topic", "other"])
      sub.unsubscribe("other")
      compare(sub.subscriptions, ["topic"])
      sub.subscriptions = ["topic", "other"]
      compare(sub.subscriptions, ["topic", "other"])
      sub.subscriptions = ["topic"]
      compare(sub.subscriptions, ["topic"])
    }
    
  }
}
