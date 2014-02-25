
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.CZMQ 1.0


Item {
  TestCase {
    id: test_rep
    name: "ZMQ_Rep"
    
    ZMQ_Rep { id: subject }
    
    function test_it() {
      subject.start()
      wait(500)
      console.log(subject)
      subject.bind("ipc:///tmp/test2")
      wait(10000)
    }
  }
}
