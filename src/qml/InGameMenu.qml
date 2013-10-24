import QtQuick 1.0

Image {
    id: inGameMenu

    signal mainMenuClicked
    signal resumeClicked

    width: 640; height: 360
    source: "main_menu_bg.jpg"


    Rectangle {
        anchors.fill: parent
        opacity: 0.3
        color: "black"
    }

    Image {

        anchors.centerIn: parent
        fillMode: "PreserveAspectFit"

        width: parent.width * 0.6

        source: "menu_bg.png"
        smooth: true

        Column {
            anchors.centerIn: parent

            MenuButton {
                text: "Main menu"
                onClicked: inGameMenu.mainMenuClicked();
            }

            MenuButton {
                text: "Resume"
                onClicked: inGameMenu.resumeClicked();
            }
        }
    }
}
