import QtQuick 1.0
import DataElements 1.0

Image {
    id: winningScreen

    signal backClicked

    width: 640; height: 360
    source: "winning_screen_bg.jpg"

    ScoreModel {
        id: scoreModel
        objectName: "scoreModel"
    }

    Image {
        id: resultsText

        source: "text_results.png"

        anchors {
            top: parent.top; topMargin: parent.height * 0.1
            horizontalCenter: parent.horizontalCenter
        }

        fillMode: Image.PreserveAspectFit
        width: parent.width * 0.3
        smooth: true
    }


    ListView {
        id: scoreList

        property real delegateWidth: scoreList.width * 0.8 / 4
        property real delegateHeight: scoreList.height * 0.7 / 4

        anchors {
            top: resultsText.bottom; topMargin: parent.height * 0.1
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }

        width: parent.width * 0.15
        model: scoreModel
        interactive: false

        delegate: Item {
            width: scoreList.delegateWidth
            height: scoreList.delegateHeight

            // Base on enum in the Platform.h, the model.ballId is the
            // ball_id representing the color of the ball.
            //
            // enum BALL_ID {
            //    UNKNOWN = 0,
            //    RED_BALL,
            //    YELLOW_BALL,
            //    GREEN_BALL,
            //    BLUE_BALL
            // };

            Image {
                id: ball

                anchors.verticalCenter: parent.verticalCenter

                width: parent.height * 0.8; height: width
                source: "ball_" + model.ballId + ".png"
            }

            Text {
                id: scoreText

                anchors {
                    left: ball.right; leftMargin: parent.width * 0.8
                    verticalCenter: parent.verticalCenter
                }

                font.pixelSize: parent.height * 0.5
                text: model.score
                color: "white"
                smooth: true
            }
        }
    }

    Image {
        id: backButton

        anchors {
            bottom: parent.bottom; bottomMargin: parent.height * 0.1
            left: parent.left; leftMargin: parent.width * 0.22
        }

        width: parent.width * 0.08
        fillMode: Image.PreserveAspectFit
        source: "back_button.png"
        smooth: true

        MouseArea {
            anchors.fill: parent
            onPressed: parent.scale = 0.95
            onReleased: parent.scale = 1.0
            onClicked: winningScreen.backClicked()
        }
    }
}
