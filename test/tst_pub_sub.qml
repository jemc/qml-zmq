
import QtQuick 2.1
import QmlSpec 1.0

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
      
      property var lastSubscription
      property var subCount: 0
      onSubscription:   { lastSubscription   = topic; subCount += 1 }
      
      property var lastUnsubscription
      property var unsubCount: 0
      onUnsubscription: { lastUnsubscription = topic; unsubCount += 1 }
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
      compare(sub.subCount, 1)
      compare(sub.unsubCount, 0)
      compare(sub.lastSubscription, "topic")
      
      sub.subscribe("other")
      compare(sub.subscriptions, ["topic", "other"])
      compare(sub.subCount, 2)
      compare(sub.unsubCount, 0)
      compare(sub.lastSubscription, "other")
      
      sub.subscribe("other")
      compare(sub.subscriptions, ["topic", "other"])
      compare(sub.subCount, 2)
      compare(sub.unsubCount, 0)
      compare(sub.lastSubscription, "other")
      
      sub.unsubscribe("other")
      compare(sub.subscriptions, ["topic"])
      compare(sub.subCount, 2)
      compare(sub.unsubCount, 1)
      compare(sub.lastUnsubscription, "other")
      
      sub.subscriptions = ["topic", "other2"]
      compare(sub.subscriptions, ["topic", "other2"])
      compare(sub.subCount, 3)
      compare(sub.unsubCount, 1)
      compare(sub.lastSubscription, "other2")
      
      sub.subscriptions = ["topic"]
      compare(sub.subscriptions, ["topic"])
      compare(sub.subCount, 3)
      compare(sub.unsubCount, 2)
      compare(sub.lastUnsubscription, "other2")
    }
  }
}
