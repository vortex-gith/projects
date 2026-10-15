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
                font.pixelSize: 18
                color: "#1F2937"
                wrapMode: Text.Wrap
                focus: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                Layout.fillWidth: true
                Layout.preferredHeight: Math.min(Math.max(55, contentHeight + 20), bottomRow.height * 0.4)

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
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                        if (event.modifiers & Qt.ControlModifier)
                        {
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

            // невидимый итем который занимает всё оставшееся пустое место
            // и выталкивает кнопку "Посчитать" в самый низ окна
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
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 10
                            columnSpacing: 10

                            Button { text: "+"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "+") }}
                            Button { text: "-"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "-") }}
                            Button { text: "*";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "*") }}

                            Button { text: "/"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "/") }}
                            Button { text: "("; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "(") }}
                            Button { text: ")"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, ")") }}

                            Button { text: "sqrt"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "sqrt()") }}
                            Button { text: "cbrt"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "cbrt()") }}
                            Button { text: "inf"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "inf") }}
                        }

                        GridLayout
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 10
                            columnSpacing: 10

                            Button { text: "sin"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "sin()") }}
                            Button { text: "cos"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "cos()") }}
                            Button { text: "tg";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "tg()") }}

                            Button { text: "asin"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "asin()") }}
                            Button { text: "acos"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "acos()") }}
                            Button { text: "atg"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "atg()") }}

                            Button { text: "pi"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "pi") }}
                            Button { text: "phi"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "phi") }}
                            Button { text: "tau"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "tau") }}
                        }

                        GridLayout
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: false
                            Layout.maximumHeight: 1000
                            columns: 3
                            rows: 3
                            rowSpacing: 10
                            columnSpacing: 10

                            Button { text: "log"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "log()") } }
                            Button { text: "log10"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "log10()") } }
                            Button { text: "log2";  Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "log2()") } }
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
                       rowSpacing: 10
                       columnSpacing: 10

                       Button { text: "7"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "7") } }
                       Button { text: "8"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "8") } }
                       Button { text: "9"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "9") } }
                       Button { text: "Очистить все"; Layout.fillWidth: true; Layout.fillHeight: true;
                           onClicked: { inputField.text = ""; outputField.text = "" }}

                       Button { text: "4"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "4") } }
                       Button { text: "5"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "5") } }
                       Button { text: "6"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "6") } }
                       Button { text: "Удалить"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { if(inputField.text.length > 0) {iputField.text.slice(0, -1)} } }

                       Button { text: "1"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "1") } }
                       Button { text: "2"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "2") } }
                       Button { text: "3"; Layout.fillWidth: true; Layout.fillHeight: true; onClicked: { inputField.insert(inputField.cursorPosition, "3") } }
                       Button { id: calculateButton; text: "Посчитать"; Layout.fillWidth: true; Layout.fillHeight: true;
                           onClicked: { outputField.text = backendCalc.calculate(inputField.text); inputField.text = ""} }
                    }
                }

            }
        }
    }
}