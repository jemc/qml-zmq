
import QtQuick 2.1

import org.jemc.qml.ZMQ 1.0


ZMQ_Socket {
  socketType: ZMQ.ROUTER
  
  signal __(); // Meaningless signal to force type naming
}
