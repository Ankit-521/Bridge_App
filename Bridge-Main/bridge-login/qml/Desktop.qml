import QtQuick 2.14
import Bridge.Constants 1.0
import "./components"

Rectangle {

    SplashScreen {
        z: 2
        //visible: false
        id: splashScreen
    }

    // wait for 5 miliseconds and show the SplashScreen
    Timer {
        running: true
        interval: 300
        repeat: false
        onTriggered: {

            splashScreen.show()
        }
    }

    // wait for 4 seconds and the hide the SplashScreen
    Timer {
        interval: 4000
        running: true
        repeat: false
        onTriggered: {
            splashScreen.hide()
            authManager.checkIfSignedIn()
            loginPage.opacity = 1
        }
    }

    LoginPage {

        width: parent.width
        height: parent.height
        id: loginPage
        opacity: 0
        visible: (opacity > 0.1)
    }
}
