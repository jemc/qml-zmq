
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZXPub,ZXSub"
    
    
    ZXPub {
      id: pub
      binds: "ipc:///tmp/test"
      
      property var lastSubscription: []
      onReceive: lastSubscription = message
    }
    
    ZXSub {
      id: sub
      connects: "ipc:///tmp/test"
      subscriptions: "topic"
      
      property var lastMessage: []
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
      wait(50)
      compare(sub.lastMessage, ["topic.x.y","message"])
      
      pub.send(["topic.zzz","message2"])
      wait(50)
      compare(sub.lastMessage, ["topic.zzz","message2"])
      
      sub.unsubscribe("topic")
      sub.lastMessage = []
      wait(50)
      pub.send(["topic.aaa","message3"])
      wait(50)
      compare(sub.lastMessage, [])
      
      sub.subscribe("topic")
      sub.lastMessage = []
      wait(50)
      pub.send(["topic.b.c","message4"])
      wait(50)
      compare(sub.lastMessage, ["topic.b.c","message4"])
    }
    
    function test_subscription_message() {
      wait(250)
      compare(pub.lastSubscription, ["%01topic%%"])
      
      sub.subscribe("other")
      wait(50)
      compare(pub.lastSubscription, ["%01other%%"])
      
      sub.unsubscribe("other")
      wait(50)
      compare(pub.lastSubscription, ["%00other%%"])
    }
    
    function test_00_subscriptions() { // This test must run first
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
