import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import RecursiveCalculator

ApplicationWindow
{
    id: mainWindow
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

            ScrollView
            {
                id: scrollViewer
                Layout.fillWidth: true
                Layout.preferredHeight: Math.min(Math.max(55, inputField.contentHeight + 20), bottomRow.height * 0.4)
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                contentWidth: availableWidth

                background: Rectangle
                {
                    color: "#F9FAFB"
                    radius: 12
                    border.color: inputField.activeFocus ? "#3B82F6" : "#D1D5DB"
                    border.width: inputField.activeFocus ? 2 : 1
                    Behavior on border.color { ColorAnimation { duration: 150 } }
                }

                Behavior on Layout.preferredHeight
                {
                    NumberAnimation { duration: 120 }
                }

                TextArea
                {
                    id: inputField
                    font.pixelSize: 18 * (mainWindow.height * 0.002)
                    color: "#1F2937"
                    wrapMode: Text.Wrap
                    focus: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    background: null

                    Keys.onPressed: (event) =>
                    {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            if (event.modifiers & Qt.ControlModifier)
                            {
                                calculateButton.clicked();
                                event.accepted = true;
                            }
                    }

                    Keys.onPressed: (event) =>
                    {
                        if(event.key === Qt.Key_Backspace)
                            {
                                backspacebtn.clicked();
                                event.accepted = true;
                            }
                    }
                }

                ScrollBar.vertical: ScrollBar
                {
                    id: vsbar
                    policy: ScrollBar.AsNeeded

                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.topMargin: 4
                    anchors.bottomMargin: 4

                    contentItem: Rectangle
                    {
                        implicitWidth: 10
                        radius: 20
                        color: vsbar.pressed ? "#4B5563" : "#9CA3AF"

                        opacity: vsbar.active ? 1.0 : 0.4
                        Behavior on opacity { NumberAnimation { duration: 150 } }
                    }

                    background: null
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
                     font.pixelSize: 18
                     font.bold: true
                     color: "#111827"
                     anchors.fill: parent
                     anchors.margins: 10
                     horizontalAlignment: Text.AlignHCenter
                     verticalAlignment: Text.AlignVCenter

                     fontSizeMode: Text.Fit
                     minimumPixelSize: 12
                }
            }
            Item
            {
                Layout.fillHeight: true
            }

            RowLayout
            {
                ComboBox
                {
                    id: modeSelector
                    Layout.fillWidth: true
                    model: ["Базовые операции", "Тригонометрические функции", "Логарифмы"]

                }
            }

            RowLayout
            {
               id: bottomRow
               Layout.fillWidth: true
               Layout.fillHeight: true
               spacing: 20

                ColumnLayout
                {
                    StackLayout
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        currentIndex: modeSelector.currentIndex

                        GridLayout
                        {
                            uniformCellWidths: true
                            uniformCellHeights: true
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 5
                            columnSpacing: 5

                            ControlButton { text: "+"; onClicked: { inputField.insert(inputField.cursorPosition, "+") }}
                            ControlButton { text: "-"; onClicked: { inputField.insert(inputField.cursorPosition, "-") }}
                            ControlButton { text: "*"; onClicked: { inputField.insert(inputField.cursorPosition, "*") }}

                            ControlButton { text: "/"; onClicked: { inputField.insert(inputField.cursorPosition, "/") }}
                            ControlButton { text: "("; onClicked: { inputField.insert(inputField.cursorPosition, "(") }}
                            ControlButton { text: ")"; onClicked: { inputField.insert(inputField.cursorPosition, ")") }}

                            ControlButton { text: "sqrt"; onClicked: { inputField.insert(inputField.cursorPosition, "sqrt()") }}
                            ControlButton { text: "cbrt"; onClicked: { inputField.insert(inputField.cursorPosition, "cbrt()") }}
                            ControlButton { text: "inf"; onClicked: { inputField.insert(inputField.cursorPosition, "inf") }}
                        }

                        GridLayout
                        {
                            uniformCellWidths: true
                            uniformCellHeights: true
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 5
                            columnSpacing: 5

                            ControlButton { text: "sin"; onClicked: { inputField.insert(inputField.cursorPosition, "sin()") }}
                            ControlButton { text: "cos"; onClicked: { inputField.insert(inputField.cursorPosition, "cos()") }}
                            ControlButton { text: "tg";  onClicked: { inputField.insert(inputField.cursorPosition, "tg()") }}

                            ControlButton { text: "asin"; onClicked: { inputField.insert(inputField.cursorPosition, "asin()") }}
                            ControlButton { text: "acos"; onClicked: { inputField.insert(inputField.cursorPosition, "acos()") }}
                            ControlButton { text: "atg"; onClicked: { inputField.insert(inputField.cursorPosition, "atg()") }}

                            ControlButton { text: "pi"; onClicked: { inputField.insert(inputField.cursorPosition, "pi") }}
                            ControlButton { text: "phi"; onClicked: { inputField.insert(inputField.cursorPosition, "phi") }}
                            ControlButton { text: "tau"; onClicked: { inputField.insert(inputField.cursorPosition, "tau") }}
                        }

                        GridLayout
                        {
                            uniformCellWidths: true
                            uniformCellHeights: true
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            Layout.preferredHeight: bottomRow.height / 3
                            columns: 3
                            rowSpacing: 5
                            columnSpacing: 5

                            ControlButton { text: "log"; onClicked: { inputField.insert(inputField.cursorPosition, "log()") } }
                            ControlButton { text: "log10"; onClicked: { inputField.insert(inputField.cursorPosition, "log10()") } }
                            ControlButton { text: "log2";  onClicked: { inputField.insert(inputField.cursorPosition, "log2()") } }
                        }
                    }
                }

               ColumnLayout
               {
                   GridLayout
                   {
                       uniformCellHeights: true
                       uniformCellWidths: true
                       columns: 4
                       rowSpacing: 5
                       columnSpacing: 5

                       ControlButton { text: "7"; onClicked: { inputField.insert(inputField.cursorPosition, "7") } }
                       ControlButton { text: "8"; onClicked: { inputField.insert(inputField.cursorPosition, "8") } }
                       ControlButton { text: "9"; onClicked: { inputField.insert(inputField.cursorPosition, "9") } }
                       ControlButton { text: "Очистить все"; onClicked: { inputField.text = ""; outputField.text = "" }}

                       ControlButton { text: "4"; onClicked: { inputField.insert(inputField.cursorPosition, "4") } }
                       ControlButton { text: "5"; onClicked: { inputField.insert(inputField.cursorPosition, "5") } }
                       ControlButton { text: "6"; onClicked: { inputField.insert(inputField.cursorPosition, "6") } }
                       ControlButton { id: backspacebtn; text: "Удалить"; onClicked: { if(inputField.text.length > 0) {inputField.text = inputField.text.slice(0, -1)} } }

                       ControlButton { text: "1"; onClicked: { inputField.insert(inputField.cursorPosition, "1") } }
                       ControlButton { text: "2"; onClicked: { inputField.insert(inputField.cursorPosition, "2") } }
                       ControlButton { text: "3"; onClicked: { inputField.insert(inputField.cursorPosition, "3") } }
                       ControlButton { id: calculateButton; text: "Посчитать"; onClicked: { outputField.text = backendCalc.calculate(inputField.text); inputField.text = ""} }
                    }
                }
            }
        }
    }
}