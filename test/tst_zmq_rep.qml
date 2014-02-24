
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.CZMQ 1.0


Item {
  TestCase {
    id: test_rep
    name: "ZMQ_Rep"
    
    ZMQ_Rep { id: subject }
    
    function test_it() {
      subject.bind("ipc:///tmp/test")
      // subject.bind("whup://huh?")
      console.log(subject)
    }
  }
}
