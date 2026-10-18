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
    color: "#FFFFFF"

    Calculator
    {
        id: backendCalc
    }


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
                    else if(event.key === Qt.Key_Backspace)
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

                Component.onCompleted:
                {
                    inputField.forceActiveFocus()
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

                palette.button: "#F3F4F6"
                palette.buttonText: "#111827"
                palette.highlight: "#E5E7EB"

                popup.palette.highlight: "#E5E7EB"
                popup.palette.highlightedText: "#111827"

                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: 18 * (mainWindow.height * 0.0015)

                focusPolicy: Qt.NoFocus

                model: ["Базовые операции", "Тригонометрические функции", "Элементарные функции"]

                contentItem: Text
                {
                    text: modeSelector.displayText
                    font: modeSelector.font
                    color: "#111827" // Цвет текста выбранного элемента (твой темно-серый)
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    leftPadding: 12 // Отступ текста от левого края кнопки
                    rightPadding: 30 // Чтобы текст не налезал на стрелочку справа
                    elide: Text.ElideRight
                }

                background: Rectangle
                {
                    color: modeSelector.pressed ? "#E5E7EB" : "#F3F4F6" // Слегка темнеет при нажатии
                    border.color: modeSelector.activeFocus ? "#3B82F6" : "#D1D5DB" // Синяя рамка, если в фокусе
                    border.width: modeSelector.activeFocus ? 2 : 1
                    radius: 8 // Симпатичное закругление углов кнопки
                }

                delegate: ItemDelegate
                {
                    width: modeSelector.width
                    highlighted: modeSelector.highlightedIndex === index

                           // Настройка фона элемента при наведении
                    background: Rectangle
                    {
                               // #E5E7EB при наведении (ваш highlight), иначе прозрачный/белый
                        color: highlighted ? "#E5E7EB" : "#F3F4F6"
                    }

                           // Настройка текста элемента
                    contentItem: Text
                    {
                        text: modelData
                        font: modeSelector.font
                        color: "#111827" // Цвет текста всегда темный, как вы и хотели
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }
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

                        ControlButton { text: "."; onClicked: { inputField.insert(inputField.cursorPosition, ".") }}
                        ControlButton { text: "^"; onClicked: { inputField.insert(inputField.cursorPosition, "^") }}
                        ControlButton { text: "!"; onClicked: { inputField.insert(inputField.cursorPosition, "!") }}
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

                        ControlButton { text: "sin"; onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "sin()"); inputField.cursorPosition = currentPos + 4; }}
                        ControlButton { text: "cos"; onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "cos()"); inputField.cursorPosition = currentPos + 4; }}
                        ControlButton { text: "tg";  onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "tg()"); inputField.cursorPosition = currentPos + 3; }}

                        ControlButton { text: "asin"; onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "arcsin()"); inputField.cursorPosition = currentPos + 7; }}
                        ControlButton { text: "acos"; onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "arccos()"); inputField.cursorPosition = currentPos + 7; }}
                        ControlButton { text: "atg"; onClicked: { let currentPos = inputField.cursorPosition; inputField.insert(currentPos, "arctg()"); inputField.cursorPosition = currentPos + 6; }}

                        ControlButton { text: "pi"; onClicked: { inputField.insert(inputField.cursorPosition, "pi") }}
                        ControlButton { text: "phi"; onClicked: { inputField.insert(inputField.cursorPosition, "phi") }}
                        ControlButton { text: "tau"; onClicked: { inputField.insert(inputField.cursorPosition, "tau") }}
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

                        ControlButton { text: "log"; onClicked: { inputField.insert(inputField.cursorPosition, "log()") } }
                        ControlButton { text: "log10"; onClicked: { inputField.insert(inputField.cursorPosition, "log10()") } }
                        ControlButton { text: "log2";  onClicked: { inputField.insert(inputField.cursorPosition, "log2()") } }

                        ControlButton { text: "sqrt"; onClicked: { inputField.insert(inputField.cursorPosition, "sqrt()") }}
                        ControlButton { text: "cbrt"; onClicked: { inputField.insert(inputField.cursorPosition, "cbrt()") }}
                        ControlButton { text: "inf"; onClicked: { inputField.insert(inputField.cursorPosition, "inf") }}

                        ControlButton { text: "abs"; onClicked: { inputField.insert(inputField.cursorPosition, "abs()") }}
                        ControlButton { text: "deg"; onClicked: { inputField.insert(inputField.cursorPosition, "deg()") }}
                        ControlButton { text: "e"; onClicked: { inputField.insert(inputField.cursorPosition, "e") }}

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
                   ControlButton { text: "C"; onClicked: { inputField.text = ""; outputField.text = "" }}

                   ControlButton { text: "4"; onClicked: { inputField.insert(inputField.cursorPosition, "4") } }
                   ControlButton { text: "5"; onClicked: { inputField.insert(inputField.cursorPosition, "5") } }
                   ControlButton { text: "6"; onClicked: { inputField.insert(inputField.cursorPosition, "6") } }
                   ControlButton { id: backspacebtn; text: "⌫"; onClicked: { if(inputField.text.length > 0) {inputField.text = inputField.text.slice(0, -1)} } }

                   ControlButton { text: "1"; onClicked: { inputField.insert(inputField.cursorPosition, "1") } }
                   ControlButton { text: "2"; onClicked: { inputField.insert(inputField.cursorPosition, "2") } }
                   ControlButton { text: "3"; onClicked: { inputField.insert(inputField.cursorPosition, "3") } }
                   ControlButton { id: calculateButton; text: "="; font.pixelSize: 22 * (mainWindow.height * 0.0015); onClicked: { outputField.text = backendCalc.calculate(inputField.text); inputField.text = ""} }
                }
            }
        }
    }
}
