import QtQuick 1.0

Item {
    id: container

    property alias text: text.text

    signal clicked

    width: 100; height: 50

    Text {
        id: text

        anchors.centerIn: parent
        color: "white"
        font.pixelSize: parent.height * 0.6
        smooth: true
    }

    MouseArea {
        anchors.fill: parent

        onPressed: text.color = "lightblue"
        onReleased: text.color = "white"
        onClicked: container.clicked();
    }
}
