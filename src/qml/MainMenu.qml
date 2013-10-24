import QtQuick 1.0

Image {
    id: mainMenu

    signal newGameClicked
    signal infoClicked
    signal exitClicked

    width: 640; height: 360
    source: "main_menu_bg.jpg"

    Image {
        id: title

        anchors {
            top: parent.top; topMargin: 0.05 * parent.height
            left: parent.left; leftMargin: 0.08 * parent.width
        }

        width: parent.width * 0.3
        fillMode: Image.PreserveAspectFit

        source: "blok_title.png"
        smooth: true
    }

    Image {
        id: menu

        anchors {
            horizontalCenter: title.horizontalCenter
            top: title.bottom; topMargin: 0.02 * parent.height
        }

        width: parent.width * 0.5
        fillMode: Image.PreserveAspectFit

        source: "menu_bg.png"
        smooth: true

        Column {
            id: column

            property real buttonHeight: menu.height * 0.9 / 3

            anchors {
                top: parent.top; topMargin: 5
                horizontalCenter: parent.horizontalCenter
            }

            MenuButton {
                id: newGameButton

                text: "New game"
                onClicked: mainMenu.newGameClicked();
                height: column.buttonHeight
            }

            MenuButton {
                id: infoViewButton

                text: "Info"
                onClicked: mainMenu.infoClicked();
                height: column.buttonHeight
            }

            MenuButton {
                id: exitButton

                text: "Exit"
                onClicked: mainMenu.exitClicked();
                height: column.buttonHeight
            }
        }
    }

    Image {

        x: parent.width * 0.5
        y: parent.height * 0.3

        width: 0.4 * parent.width

        fillMode: Image.PreserveAspectFit

        source: "main_menu_blok.png"
        smooth: true
    }
}
