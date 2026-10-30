import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import MultithreadedParserForGeneratedLogs

ApplicationWindow
{
        id: mainWindow
        visible: true

        MainController
        {
                id:backend
        }

        RowLayout
        {
                ColumnLayout
                {
                        id: leftmaincolumn

                        Rectangle
                        {
                                id: rawlogsTextArea


                                TextField
                                {
                                        id: countField
                                        text: ""

                                        validator: IntValidator
                                        {
                                                top: 100000000
                                                bottom: 0
                                        }
                                }

                        }
                }
                ColumnLayout
                {
                        id:rightmaincolumn

                        Button
                        {
                                text: "Запустить"

                                onClicked: if(countField.text !== "")
                                           {
                                                   backend.initialization(3000);
                                                   backend.start_threads();
                                           }

                        }
                }
        }
}
