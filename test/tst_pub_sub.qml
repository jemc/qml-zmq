
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZPub,ZSub"
    
    
    ZPub {
      id: pub
      binds: "ipc:///tmp/test_pub_sub"
    }
    
    ZSub {
      id: sub
      connects: "ipc:///tmp/test_pub_sub"
      subscriptions: "topic"
      
      property var lastMessage:   []
      onReceive: lastMessage = message
    }
    
    
    function initTestCase() { wait(250) }
    
    function test_messages() {
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
