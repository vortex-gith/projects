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

                            Button { text: "+"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "-"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "*";  Layout.fillWidth: true; Layout.fillHeight: true }

                            Button { text: "/"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "("; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: ")"; Layout.fillWidth: true; Layout.fillHeight: true }

                            Button { text: "sqrt"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "cbrt"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "inf"; Layout.fillWidth: true; Layout.fillHeight: true }
                        }

                        GridLayout
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 10
                            columnSpacing: 10

                            Button { text: "sin"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "cos"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "tg";  Layout.fillWidth: true; Layout.fillHeight: true }

                            Button { text: "asin"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "acos"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "atg"; Layout.fillWidth: true; Layout.fillHeight: true }

                            Button { text: "pi"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "phi"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "tau"; Layout.fillWidth: true; Layout.fillHeight: true }
                        }

                        GridLayout
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 3
                            rowSpacing: 10
                            columnSpacing: 10

                            Button { text: "log"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "log10"; Layout.fillWidth: true; Layout.fillHeight: true }
                            Button { text: "log2";  Layout.fillWidth: true; Layout.fillHeight: true }
                        }
                    }
                }

               ColumnLayout
               {
                   GridLayout
                   {
                       Layout.fillWidth: true
                       Layout.fillHeight: true
                       columns: 4
                       rowSpacing: 10
                       columnSpacing: 10

                       Button { text: "7"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "8"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "9"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "Очистить все"; Layout.fillWidth: true; Layout.fillHeight: true }

                       Button { text: "4"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "5"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "6"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "Удалить"; Layout.fillWidth: true; Layout.fillHeight: true }

                       Button { text: "1"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "2"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "3"; Layout.fillWidth: true; Layout.fillHeight: true }
                       Button { text: "Посчитать"; Layout.fillWidth: true; Layout.fillHeight: true }
                    }
                }

            }
        }
    }
}