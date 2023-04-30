import QtQuick 2.14
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
//PopUp window when alert is received by opponents "ankit"
ApplicationWindow{
     id: alertDialog
     width: 300
     height:300
     title: "ALERT"
     property string alert :""
     Button {
         id:explain
         implicitWidth: alertDialog.width/2
         implicitHeight: alertDialog.height/3
         font.pixelSize: parent.width / 10
         anchors {
             top: parent.top
             left: parent.left
             right: agree.left
              margins: 5

         }
             text: qsTr("Explain")
             onClicked: {message.text=alert;message.visible=true;agree.visible=false;explain.visible=false;}
        }


     Button {
         id:agree
         implicitWidth: alertDialog.width/2
         implicitHeight: alertDialog.height/3
         font.pixelSize: parent.width / 10
         anchors {
             left: explain.right
             top: parent.top
             right: parent.right
              margins: 5

         }
             text: qsTr("Continue")
             onClicked: {alertDialog.close();agree.visible=false;explain.visible=false;}
        }

     Text {
                 visible:false
                 id: message
                 color: "black"
                 font.pixelSize: 15

                 text: "alert visible"

                 horizontalAlignment: Text.AlignHCenter
        }


}

