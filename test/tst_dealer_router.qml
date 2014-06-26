
import QtQuick 2.1
import QmlSpec 1.0

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZDealer,ZRouter"
    
    
    ZDealer {
      id: dealer
      binds: "ipc:///tmp/test_dealer_router"
      
      property var lastReply:   []
      onReceive: lastReply = message
    }
    
    ZRouter {
      id: router
      connects: "ipc:///tmp/test_dealer_router"
      
      property var lastRequest:   []
      onReceive: {
        lastRequest = message
        send(message)
      }
    }
    
    function test_messages() {
      var routingInfo
      
      dealer.send("message")
      wait(100)
      routingInfo = router.lastRequest[0]
      compare(router.lastRequest[1], "message")
      compare(dealer.lastReply, ["message"])
      
      dealer.send("message2")
      wait(100)
      compare(routingInfo, router.lastRequest[0])
      compare(router.lastRequest[1], "message2")
      compare(dealer.lastReply, ["message2"])
    }
    
  }
}
