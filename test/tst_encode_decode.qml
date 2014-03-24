
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
    
    function initTestCase() { wait(250) }
    
    function init_data() {
      return [
        {source:"foo\xF3bar", encoded:"foo%F3bar"},
      ]
    }
    
    function test_latin1(data) {
      var str = data.source
      var enc = data.encoded
      
      compare(ZUtil.convertLatin1ToData(str), enc)
      compare(ZUtil.convertDataToLatin1(enc), str)
      
      spy.clear()
      push.send(ZUtil.convertLatin1ToData(str))
      spy.wait()
      compare(ZUtil.convertDataToLatin1(spy.signalArguments[0][0][0]), str)
    }
  }
}
