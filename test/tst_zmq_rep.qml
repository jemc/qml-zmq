
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test_rep
    name: "ZMQ_Rep"
    
    ZMQ_Rep { id: subject
      onRequest: {
        console.log(data)
        send(["heyo","bro"])
      }
    }
    
    function test_it() {
      subject.start()
      wait(500)
      subject.bind("ipc:///tmp/test2")
      wait(500)
      subject.connect("ipc:///tmp/test3")
      wait(500)
      subject.fakeRequest(["this","is","a","fake","request"])
      wait(500)
      subject.stop()
      wait(500)
    }
  }
}
