
import QtQuick 2.1
import QmlSpec 1.0

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "ZReq,ZRouter"
    
    
    ZReq {
      id: req
      binds: "ipc:///tmp/test_req_router"
      
      property var lastReply:   []
      onReceive: lastReply = message
    }
    
    ZRouter {
      id: router
      connects: "ipc:///tmp/test_req_router"
      
      property var lastRequest:   []
      onReceive: {
        lastRequest = message
        send(message)
      }
    }
    
    function test_messages() {
      var routingInfo
      
      req.send("message")
      wait(100)
      routingInfo = router.lastRequest[0]
      compare(router.lastRequest[1], "")
      compare(router.lastRequest[2], "message")
      compare(req.lastReply, ["message"])
      
      req.send("message2")
      wait(100)
      compare(routingInfo, router.lastRequest[0])
      compare(router.lastRequest[1], "")
      compare(router.lastRequest[2], "message2")
      compare(req.lastReply, ["message2"])
    }
    
  }
}
