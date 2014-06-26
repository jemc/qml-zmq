
import QtQuick 2.1
import QmlSpec 1.0


TestSuite {
  name: "Main Suite"
  
  testFiles: [
    Qt.resolvedUrl("tst_push_pull.qml"),
    Qt.resolvedUrl("tst_req_rep.qml"),
    Qt.resolvedUrl("tst_req_router.qml"),
    Qt.resolvedUrl("tst_dealer_router.qml"),
    Qt.resolvedUrl("tst_pub_sub.qml"),
    Qt.resolvedUrl("tst_xpub_xsub.qml"),
    
    Qt.resolvedUrl("tst_inproc.qml"),
    Qt.resolvedUrl("tst_send_flags.qml"),
    Qt.resolvedUrl("tst_encode_decode.qml"),
  ]
  
  Component.onCompleted: run()
  onFinished: quit()
}
