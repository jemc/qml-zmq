
import QtQuick 2.1
import QmlSpec 1.0

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "encode_decode"
    
    ZPush { id:push; binds:   "inproc://encode_decode_test" }
    ZPull { id:pull; connects:"inproc://encode_decode_test" }
    SignalSpy { id:spy; target:pull; signalName:"receive" }
    
    function test_latin1_data() {
      return [
        {source:"foo\xF3bar", encoded:"foo%F3bar"},
        {source:"foo\x00bar", encoded:"foo%00bar"},
        {source:"100%", encoded:"100%25"},
      ]
    }
    
    function test_utf8_data() {
      return [
        {source:"foo\xF3bar", encoded:"foo%C3%B3bar"},
        {source:"foo\x00bar", encoded:"foo%00bar"},
        {source:"100%", encoded:"100%25"},
        {source:"\u0001", encoded:"%01"},
        {source:"\u0012", encoded:"%12"},
        {source:"\u0123", encoded:"%C4%A3"},
        {source:"\u1234", encoded:"%E1%88%B4"},
        {source:"èëœ", encoded:"%C3%A8%C3%AB%C5%93"},
        {source:"€àáß", encoded:"%E2%82%AC%C3%A0%C3%A1%C3%9F"},
        {source:"文字化け", encoded:"%E6%96%87%E5%AD%97%E5%8C%96%E3%81%91"},
      ]
    }
    
    function test_latin1(data) {
      compare(ZUtil.convertLatin1ToData(data.source), data.encoded)
      compare(ZUtil.convertDataToLatin1(data.encoded), data.source)
      
      spy.clear()
      push.send(ZUtil.convertLatin1ToData(data.source))
      spy.wait()
      compare(ZUtil.convertDataToLatin1(spy.signalArguments[0][0][0]), data.source)
    }
    
    function test_utf8(data) {
      compare(ZUtil.convertUtf8ToData(data.source), data.encoded)
      compare(ZUtil.convertDataToUtf8(data.encoded), data.source)
      
      spy.clear()
      push.send(ZUtil.convertUtf8ToData(data.source))
      spy.wait()
      compare(ZUtil.convertDataToUtf8(spy.signalArguments[0][0][0]), data.source)
    }
  }
}
