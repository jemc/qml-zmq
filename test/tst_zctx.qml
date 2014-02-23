
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.CZMQ 1.0


Item {
  TestCase {
    id: test_zctx
    name: "ZCtx"
    
    ZCtx { id: subject }
    
    function test_it() {
      console.log(subject)
    }
  }
}
