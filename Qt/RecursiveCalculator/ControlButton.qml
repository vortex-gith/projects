import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button
{
    id: controlbtn
    Layout.fillWidth: true
    Layout.fillHeight: true
    font.pixelSize: 18 * (mainWindow.height * 0.0015)

    focusPolicy: Qt.NoFocus

    property color baseColor: "#F3F4F6"
    property color hoverColor: "#E5E7EB"
    property color pressColor: "#D1D5DB"
    property color textColor: "#111827"

    contentItem: Text
    {
        text: controlbtn.text
        font: controlbtn.font
        color: controlbtn.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle
    {
        radius: controlbtn.cornerRadius
        implicitWidth: 40
        implicitHeight: 40

        color: controlbtn.down ? controlbtn.pressColor : (controlbtn.hovered ? controlbtn.hoverColor : controlbtn.baseColor)

        Behavior on color { ColorAnimation { duration: 70 } }
    }
}