import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RecursiveCalculator

ApplicationWindow {
    width: 400
    height: 500
    minimumWidth: 400
    minimumHeight: 500
    visible: true
    title: "Академический калькулятор"
    color: "#F3F4F6"

    Calculator
    {
        id: backendCalc
    }

    Rectangle
    {
        anchors.fill: parent
        anchors.margins: 20
        color: "#FFFFFF"
        radius: 20
        border.color: "#E5E7EB"
        border.width: 1

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 25
            spacing: 20

            TextArea
            {
                id: inputField
                placeholderText: "Например: 2 + 2 * sin(pi)"
                font.pixelSize: 18
                color: "#1F2937"
                wrapMode: Text.Wrap

                Layout.fillWidth: true
                Layout.preferredHeight: Math.min(Math.max(55, contentHeight + 20), 220)

                Behavior on Layout.preferredHeight
                {
                    NumberAnimation { duration: 120 }
                }

                background: Rectangle
                {
                    color: "#F9FAFB"
                    radius: 12
                    border.color: inputField.activeFocus ? "#3B82F6" : "#D1D5DB"
                    border.width: inputField.activeFocus ? 2 : 1
                    Behavior on border.color { ColorAnimation { duration: 150 } }
                }

                Keys.onPressed: (event) =>
                {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                        calculateButton.clicked();
                        event.accepted = true;
                    }
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 55
                color: "#FFFFFF"
                radius: 12
                border.color: "#E5E7EB"
                border.width: 1

                Text
                {
                    id: outputField
                    text: ""
                    font.pixelSize: 22
                    font.bold: true
                    color: "#111827"
                    anchors.fill: parent
                    anchors.margins: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    // чтобы длинные ответы или ошибки умещались визуально
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 12
                }
            }

            // невидимая итем который занимает всё оставшееся пустое место
            // и выталкивает кнопку "Посчитать" в самый низ окна
            Item
            {
                Layout.fillHeight: true
            }

            Item
            {
                id: calculateButton
                Layout.fillWidth: true
                Layout.preferredHeight: 55

                signal clicked()

                // свойства hovered и down на основе мыши
                readonly property bool hovered: mouseArea.containsMouse
                readonly property bool down: mouseArea.pressed

                Rectangle
                {
                    anchors.fill: parent
                    radius: 12

                    color: calculateButton.down ? "#1e2947" : (calculateButton.hovered ? "#2e3a65" : "#455076")

                    Behavior on color { ColorAnimation { duration: 120 } }

                    Text
                    {
                        text: "Посчитать"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#FFFFFF"
                        anchors.centerIn: parent
                    }
                }

                MouseArea
                {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: calculateButton.clicked()
                }

                onClicked:
                {
                    if (inputField.text !== "")
                    {
                        outputField.text = backendCalc.calculate(inputField.text);
                    }
                }
            }
        }
    }
}