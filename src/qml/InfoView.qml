import QtQuick 1.0

Image {
    id: infoView

    signal backClicked

    width: 640; height: 360
    source: "info_view_menu_bg.jpg"

    Flickable {
        id: flickable

        anchors {
            top: parent.top; topMargin: 0.05 * parent.height
            bottom: parent.bottom; bottomMargin: 0.17 * parent.height
            horizontalCenter: parent.horizontalCenter
        }

        clip: true
        width: parent.width * 0.85
        flickableDirection: Flickable.VerticalFlick
        contentWidth: infoText.width
        contentHeight: infoText.height

        Text {
            id: infoText

            width: infoView.width * 0.8
            wrapMode: Text.WordWrap
            color: "white"
            smooth: true
            font.pixelSize: infoView.width * 0.03

            text: "Space Blok is a 1-4 player game where the goal is to " +
                  "gather as much points as possible by destroying the game " +
                  "bloks.\n\n" +
                  "This Nokia Developer example is hosted at " +
                  "http://projects.developer.nokia.com/spaceblokqt/\n\n" +
                  "To start a new game, select New game from the main menu. " +
                  "Game board is shown with four platforms on each corner " +
                  "of the view.\n\n" +
                  "Place the phone on the table so that each player are on " +
                  "each corner of the phone beside the colored ball. The " +
                  "ball can be shot by swiping from the top of the ball " +
                  "towards game structure in the center of the view. The " +
                  "direction and the speed of the swipe affects to the " +
                  "ball trajectory, use fast swipes to hit the nearest " +
                  "game bloks, and use slow swipes to hit the furthest " +
                  "bloks. The game level will begin to rotate when the " +
                  "bloks are hit. Some bloks requires several hits in " +
                  "order them to be destroyed.\n\n" +
                  "Good game strategy is to shoot the ball inside the game " +
                  "level from one of its endings and let the ball bounce " +
                  "inside the structure giving loads of points.\n\n" +
                  "Credits\n\n" +
                  "Antti Krats\n" +
                  "Kari Kantola\n" +
                  "Mika Myllymäki\n" +
                  "Tuomo Hirvonen\n\n" +
                  "The Bullet Physics Library comes with ZLib license:\n" +
                  "Copyright (c) 2003-2010 Erwin Coumans\n\n" +
                  "http://continuousphysics.com/Bullet/\n\n" +
                  "This software is provided 'as-is', without any " +
                  "express or implied warranty. In no event will the " +
                  "authors be held liable for any damages arising from " +
                  "the use of this software. Permission is granted to " +
                  "anyone to use this software for any purpose, " +
                  "including commercial applications, and to alter it " +
                  "and redistribute it freely, subject to the following " +
                  "restrictions:\n\n" +
                  "1. The origin of this software must not be " +
                  "misrepresented; you must not claim that you wrote " +
                  "the original software. If you use this software in a " +
                  "product, an acknowledgment in the product " +
                  "documentation would be appreciated but is not " +
                  "required.\n\n" +
                  "2. Altered source versions must be plainly marked " +
                  "as such, and must not be misrepresented as being the " +
                  "original software.\n\n" +
                  "3. This notice may not be removed or altered from " +
                  "any source distribution."
        }
    }

    Rectangle {
        id: scrollDecorator

        anchors.right: flickable.right
        y: flickable.visibleArea.yPosition * flickable.height +
           flickable.anchors.topMargin
        width: 4; height: flickable.visibleArea.heightRatio * flickable.height
        opacity: 0.5
        color: "lightsteelblue"
        radius: 2
    }

    Image {
        id: backButton

        anchors {
            bottom: parent.bottom; bottomMargin: parent.height * 0.02
            left: parent.left; leftMargin: parent.width * 0.04
        }

        width: parent.width * 0.08
        fillMode: Image.PreserveAspectFit
        source: "back_button.png"
        smooth: true

        MouseArea {
            anchors.fill: parent
            onPressed: parent.scale = 0.95
            onReleased: parent.scale = 1.0
            onClicked: infoView.backClicked()
        }
    }
}
