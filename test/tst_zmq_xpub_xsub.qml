
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZMQ_XPub,XSub"
    
    
    // ZXPub {
    //   id: pub
    //   // binds: "ipc:///tmp/test"
      
    //   property var lastSubscription: []
    //   onReceive: lastSubscription = message
    // }
    
    // ZXSub {
    //   id: sub
    //   connects: "ipc:///tmp/test"
    //   subscriptions: "topic" // TODO: make this work
    //   Component.onCompleted: subscribe("topic")
      
    //   property var lastMessage: []
    //   onReceive: lastMessage = message
    // }
    
    
    // function test_messages() {
    //   wait(250)
      
    //   pub.send(["topic.x.y","message"])
    //   wait(50)
    //   compare(sub.lastMessage, ["topic.x.y","message"])
      
    //   pub.send(["topic.zzz","message2"])
    //   wait(50)
    //   compare(sub.lastMessage, ["topic.zzz","message2"])
      
    //   sub.unsubscribe("topic")
    //   sub.lastMessage = []
    //   wait(50)
    //   pub.send(["topic.aaa","message3"])
    //   wait(50)
    //   compare(sub.lastMessage, [])
      
    //   sub.subscribe("topic")
    //   sub.lastMessage = []
    //   wait(50)
    //   pub.send(["topic.b.c","message4"])
    //   wait(50)
    //   compare(sub.lastMessage, ["topic.b.c","message4"])
    // }
    
    // function test_subscription_message() {
    //   wait(250)
    //   compare(pub.lastSubscription, ["%01topic%%"])
      
    //   sub.subscribe("other")
    //   wait(50)
    //   compare(pub.lastSubscription, ["%01other%%"])
      
    //   sub.unsubscribe("other")
    //   wait(50)
    //   compare(pub.lastSubscription, ["%00other%%"])
    // }
    
    // function test_subscriptions() {
    //   compare(sub.subscriptions, ["topic"])
    //   sub.subscribe("other")
    //   compare(sub.subscriptions, ["topic", "other"])
    //   sub.subscribe("other")
    //   compare(sub.subscriptions, ["topic", "other"])
    //   sub.unsubscribe("other")
    //   compare(sub.subscriptions, ["topic"])
    //   sub.subscriptions = ["topic", "other"]
    //   compare(sub.subscriptions, ["topic", "other"])
    //   sub.subscriptions = ["topic"]
    //   compare(sub.subscriptions, ["topic"])
    // }
  }
}
