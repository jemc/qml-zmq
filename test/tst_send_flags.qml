
import QtQuick 2.1
import QmlSpec 1.0

import org.jemc.qml.ZMQ 1.0


Item {
  TestCase {
    id: test
    name: "send_flags"
    
    ZPush {
      id: push
      binds: "ipc:///tmp/test"
    }
    
    ZPull {
      id: pull
      connects: "ipc:///tmp/test"
    }
    
    SignalSpy {
      id: spy
      target: pull
      signalName: "receive"
      
      function compareOne(expected) {
        wait()
        test.compare(spy.count, 1)
        var message = signalArguments[0][0]
        test.compare(Object.keys(message).length, expected.length)
        for(var i=0; i<expected.length; i++)
          test.compare(message[i], expected[i])
      }
    }
    
    function test_blocking_send() {
      spy.clear()
      
      compare(push.send("message"), true)
      
      spy.compareOne(["message"])
    }
    
    function test_nonblocking_send_true() {
      spy.clear()
      
      compare(push.send("message", ZMQ.DONTWAIT), true)
      
      spy.compareOne(["message"])
    }
    
    function test_nonblocking_send_false() {
      var orig = pull.connects
      pull.connects = []
      wait(50)
      spy.clear()
      
      compare(push.send("message", ZMQ.DONTWAIT), false)
      
      compare(spy.count, 0)
      pull.connects = orig
      wait(50)
    }
    
    function test_sendmore() {
      spy.clear()
      
      compare(push.send("multi", ZMQ.SNDMORE), true)
      compare(push.send("part", ZMQ.SNDMORE), true)
      compare(push.send("message1"), true)
      
      spy.compareOne(["multi","part","message1"])
      spy.clear()
      
      compare(push.send(["multi","part"], ZMQ.SNDMORE), true)
      compare(push.send("message2"), true)
      
      spy.compareOne(["multi","part","message2"])
      spy.clear()
      
      compare(push.send("multi", ZMQ.SNDMORE), true)
      compare(push.send(["part","message3"]), true)
      
      spy.compareOne(["multi","part","message3"])
    }
  }
}
