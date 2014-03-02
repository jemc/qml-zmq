
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test_rep
    name: "ZMQ_Rep"
    
    ZMQ_Rep { id: rep
      onReceive: {
        console.log(data)
        send(["heyo","bro"])
      }
    }
    
    ZMQ_Req { id: req
      onReceive: {
        console.log(data)
      }
    }
    
    function test_it() {
      rep.bind("ipc:///tmp/test")
      req.connect("ipc:///tmp/test")
      
      req.send(["this","is","a","fake","request"])
      wait(500)
    }
  }
}
