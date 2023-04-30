import QtQuick 2.14
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import "logic.js" as Logic
Rectangle{
     id: claim
     width: 300
     height:300
     property int claim_num:-1
     property string claim_msg: ""
     property int tricks_left:13
     signal makeclaim(int claim_num, string claim_msg)
     border.color: "blue"

     function f(currIndex){
        return currIndex
       }

    Grid {
        id: grid
        rows: 2
        columns: 7
        spacing: 3
        height:120
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 5
        }

        Repeater {
            id:rep
            model:tricks_left+1
            Button {

                width: (grid.width-grid.spacing*6)/7
                height: (grid.height-grid.spacing*4)/5
                  text:f(index)


                onClicked:{
                    claim_num=index;

                }
            }
        }
    }
    Text{
        id:num
        visible: claim_num!=-1
        anchors{
            bottom:claim_title.top
            margins: 2
           }
        font.pixelSize: 19
        color: "green"
        text: claim_num
        anchors.horizontalCenter: parent.horizontalCenter

    }

    Text{
        id:claim_title
        anchors{
            bottom:text.top
            margins: 2
           }
        font.pixelSize: 19
        color: "green"
        text: "Write claim message"
        anchors.horizontalCenter: parent.horizontalCenter

    }
    TextEdit{
        id:text
        text:""
        width:parent.width
        height: parent.height/3
        anchors{
           bottom: submit.top
            left: parent.left
            right: parent.right
            margins: 20
        }


    }
    Button{
        id:submit
        implicitWidth: parent.width/2
         implicitHeight: parent.height/10
        anchors {
            left: parent.left
            bottom: parent.bottom
            margins: 5
        }
       text: qsTr("Submit")
       onClicked: {
           claim_msg=text.text;
           claim.visible=false;
           if(claim_num!=-1){
               claim.makeclaim(claim_num,claim_msg);
           }

           text.text="";
       }
    }

    Button{
        id:cancel
        implicitWidth: parent.width/2
          implicitHeight: parent.height/10
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 5
        }
       text: qsTr("cancel")
        onClicked: {
            claim.visible=false;

            claim_num=-1;
            text.text="";
        }
    }
}
