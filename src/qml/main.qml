import QtQuick 1.0

Item  {
    id: main

    function toMainMenu() {
        main.state = "MainMenu";
    }

    function toNewGame() {
        main.state = "Game";
        main.newGame();
    }

    function toWinningScreen() {
        main.state = "WinningScreen";
    }

    function toInfoView() {
        main.state = "Info";
    }

    function toInGameMenu() {
        main.state = "InGameMenu";
    }

    signal newGame
    signal resume
    signal exit

    width: 640; height: 360

    //Component.onCompleted: toInfoView();

    MainMenu {
        id: mainMenu

        anchors.fill: parent
        opacity: 0

        onNewGameClicked: main.newGame();
        onInfoClicked: toInfoView();
        onExitClicked: exit();
    }

    InfoView {
        id: infoView

        anchors.fill: parent
        opacity: 0

        onBackClicked: toMainMenu();
    }

    WinningScreen {
        id: winningScreen

        anchors.fill: parent
        opacity: 0

        onBackClicked: toMainMenu();
    }

    InGameMenu {
        id: inGameMenu

        anchors.fill: parent
        opacity: 0

        onMainMenuClicked: toMainMenu();
        onResumeClicked: main.resume();
    }

    states: [ State {
            name: "MainMenu"
            PropertyChanges {
                target: mainMenu
                opacity: 1.0
            }
        },
        State {
            name: "Info"
            PropertyChanges {
                target: infoView
                opacity: 1.0
            }
        },
        State {
            name: "WinningScreen"
            PropertyChanges {
                target: winningScreen
                opacity: 1.0
            }
        },
        State {
            name: "InGameMenu"
            PropertyChanges {
                target: inGameMenu
                opacity: 1.0
            }
        },
        State {
            name: "Game"
        }
    ]
}
