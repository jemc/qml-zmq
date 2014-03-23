
import QtTest 1.0
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "encode_decode"
    
    ZPush { id:push; binds:"ipc:///tmp/test" }
    ZPull { id:pull; connects:"ipc:///tmp/test" }
    SignalSpy { id:spy; target:pull; signalName:"receive" }
    
    property var strings: [
      ["foo\xF3bar", "foo%F3bar"],
    ]
    
    function initTestCase() { wait(250) }
    
    function test_it() {
      for(var i=0; i<strings.length; i++){
        var str = strings[i][0]
        var enc = strings[i][1]
        
        compare(ZUtil.convertLatin1ToData(str), enc)
        compare(ZUtil.convertDataToLatin1(enc), str)
        
        spy.clear()
        push.send(ZUtil.convertLatin1ToData(str))
        spy.wait()
        compare(ZUtil.convertDataToLatin1(spy.signalArguments[0][0][0]), str)
      }
    }
  }
}
