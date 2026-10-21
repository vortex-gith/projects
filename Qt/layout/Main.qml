import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: app
    visible: true
    width: 1440
    height: 940
    minimumWidth: 760
    minimumHeight: 620
    title: ""
    flags: Qt.Window | Qt.FramelessWindowHint
    color: palette.background

    property bool wideLayout: width >= 1120
    property int targetLogs: 25000
    property int processedLogs: 0
    property int workerCount: 8
    property bool isRunning: false
    property bool isPaused: false
    property real progress: targetLogs > 0 ? Math.min(1.0, processedLogs / targetLogs) : 0
    property int demoSequence: 12
    property int demoTicks: 0
    property string selectedSeverity: "INFO"
    property var filteredDetailLogs: []

    function formatNumber(value) {
        return String(value).replace(/\B(?=(\d{3})+(?!\d))/g, " ")
    }

    function pad2(value) {
        return value < 10 ? "0" + value : String(value)
    }

    function currentTimestamp() {
        var d = new Date()
        var ms = String(d.getMilliseconds())
        while (ms.length < 3) ms = "0" + ms
        return pad2(d.getHours()) + ":" + pad2(d.getMinutes()) + ":" + pad2(d.getSeconds()) + "." + ms
    }

    function levelColor(level) {
        if (level === "INFO") return palette.lime
        if (level === "WARNING") return palette.amber
        if (level === "ERROR" || level === "FATAL") return palette.red
        if (level === "DEBUG") return palette.blue
        if (level === "TRACE") return palette.violet
        if (level === "ALERT") return palette.orange
        if (level === "NOTICE") return palette.cyan
        return palette.text
    }

    function openSeverity(level) {
        selectedSeverity = level
        filteredDetailLogs = detailLogs.filter(function(entry) { return entry.level === level })
        detailDialog.open()
    }

    function appendTerminalEvent() {
        var levels = ["INFO", "DEBUG", "TRACE", "WARNING", "INFO", "NOTICE", "ERROR"]
        var messages = [
            "decode.record           status=ok bytes=384",
            "worker.merge            batch_ready=true queue=17",
            "normalize.timestamp     format=iso8601 delta=0.4ms",
            "schema.mismatch         field=request_id record=0412",
            "index.write             shard=03 committed=true",
            "source.rotate          file=generated_05.log",
            "parse.exception        token=\\x00 offset=812"
        ]
        var index = demoSequence % levels.length
        demoSequence += 1
        var seq = String(demoSequence)
        while (seq.length < 4) seq = "0" + seq
        logModel.append({
            seq: seq,
            timestamp: currentTimestamp(),
            level: levels[index],
            message: messages[index],
            thread: "t-" + pad2((demoSequence % Math.max(1, workerCount)) + 1)
        })
        if (logModel.count > 70)
            logModel.remove(0)
        Qt.callLater(function() { logList.positionViewAtEnd() })
    }

    QtObject {
        id: palette
        readonly property color background: "#11140F"
        readonly property color panel: "#171B14"
        readonly property color panelDark: "#090C08"
        readonly property color panelRaised: "#1D2319"
        readonly property color hover: "#252D1E"
        readonly property color edge: "#333B2D"
        readonly property color edgeSoft: "#262D22"
        readonly property color text: "#D9DFD0"
        readonly property color muted: "#929B87"
        readonly property color dim: "#66705D"
        readonly property color lime: "#B8D977"
        readonly property color amber: "#D9AC65"
        readonly property color red: "#E07168"
        readonly property color blue: "#8BAED1"
        readonly property color violet: "#B19ACB"
        readonly property color cyan: "#83C2B8"
        readonly property color orange: "#D99061"
    }

    ListModel {
        id: logModel
        ListElement { seq: "0001"; timestamp: "12:04:17.021"; level: "INFO";    message: "parser.boot             mode=stream source=generated_04.log"; thread: "t-01" }
        ListElement { seq: "0002"; timestamp: "12:04:17.026"; level: "DEBUG";   message: "pool.spawn              workers=8 queue=0"; thread: "t-01" }
        ListElement { seq: "0003"; timestamp: "12:04:17.104"; level: "INFO";    message: "source.open             encoding=UTF-8 size=18.4MB"; thread: "t-02" }
        ListElement { seq: "0004"; timestamp: "12:04:17.128"; level: "NOTICE";  message: "schema.detect           pattern=nginx_combined confidence=0.98"; thread: "t-02" }
        ListElement { seq: "0005"; timestamp: "12:04:17.219"; level: "WARNING"; message: "schema.mismatch         field=request_id record=0412"; thread: "t-04" }
        ListElement { seq: "0006"; timestamp: "12:04:17.244"; level: "TRACE";   message: "queue.pop               task=000128 latency=0.8ms"; thread: "t-07" }
        ListElement { seq: "0007"; timestamp: "12:04:17.307"; level: "ERROR";   message: "timestamp.parse        record=0418 token=2026/99/77"; thread: "t-03" }
        ListElement { seq: "0008"; timestamp: "12:04:17.332"; level: "DEBUG";   message: "worker.retry            task=000128 attempt=2"; thread: "t-07" }
        ListElement { seq: "0009"; timestamp: "12:04:17.401"; level: "INFO";    message: "record.commit           offset=00000812 parsed=true"; thread: "t-05" }
        ListElement { seq: "0010"; timestamp: "12:04:17.443"; level: "ALERT";   message: "io.backpressure         stream=stdin pending=8192"; thread: "t-01" }
        ListElement { seq: "0011"; timestamp: "12:04:17.501"; level: "FATAL";   message: "worker.exit             worker=06 code=139"; thread: "t-06" }
        ListElement { seq: "0012"; timestamp: "12:04:17.539"; level: "INFO";    message: "worker.recover          worker=06 respawn=true"; thread: "t-01" }
    }

    ListModel {
        id: severityModel
        ListElement { label: "INFO";    total: 8421; share: 42.1; accent: "#B8D977" }
        ListElement { label: "WARNING"; total: 2184; share: 10.9; accent: "#D9AC65" }
        ListElement { label: "ERROR";   total: 1032; share: 5.2;  accent: "#E07168" }
        ListElement { label: "DEBUG";   total: 4508; share: 22.5; accent: "#8BAED1" }
        ListElement { label: "FATAL";   total: 86;   share: 0.4;  accent: "#E07168" }
        ListElement { label: "TRACE";   total: 1946; share: 9.7;  accent: "#B19ACB" }
        ListElement { label: "ALERT";   total: 527;  share: 2.6;  accent: "#D99061" }
        ListElement { label: "NOTICE";  total: 1296; share: 6.5;  accent: "#83C2B8" }
    }

    // Representative demo rows shown when a level card is opened.
    property var detailLogs: [
        { level: "INFO", date: "2026-10-21", time: "12:04:17.021", thread: "t-01", source: "generated_04.log:1", comment: "Парсер запущен в потоковом режиме; источник успешно открыт.", raw: "parser.boot mode=stream source=generated_04.log" },
        { level: "INFO", date: "2026-10-21", time: "12:04:17.104", thread: "t-02", source: "generated_04.log:3", comment: "Определена кодировка UTF-8, размер входного файла 18,4 МБ.", raw: "source.open encoding=UTF-8 size=18.4MB" },
        { level: "INFO", date: "2026-10-21", time: "12:04:17.401", thread: "t-05", source: "generated_04.log:9", comment: "Запись прошла разбор и была отправлена в индекс.", raw: "record.commit offset=00000812 parsed=true" },
        { level: "WARNING", date: "2026-10-21", time: "12:04:17.219", thread: "t-04", source: "generated_04.log:5", comment: "В записи отсутствует поле request_id. Запись сохранена с пометкой.", raw: "schema.mismatch field=request_id record=0412" },
        { level: "WARNING", date: "2026-10-21", time: "12:05:02.663", thread: "t-03", source: "generated_04.log:612", comment: "Неизвестное поле upstream_time; значение вынесено в дополнительные данные.", raw: "schema.extra_field field=upstream_time record=0612" },
        { level: "WARNING", date: "2026-10-21", time: "12:05:11.008", thread: "t-08", source: "generated_04.log:781", comment: "Размер строки превышает целевой порог 16 КБ.", raw: "record.large bytes=19288 threshold=16384" },
        { level: "ERROR", date: "2026-10-21", time: "12:04:17.307", thread: "t-03", source: "generated_04.log:7", comment: "Не удалось распознать дату: месяц указан как 99. Применён режим пропуска поля.", raw: "timestamp.parse record=0418 token=2026/99/77" },
        { level: "ERROR", date: "2026-10-21", time: "12:06:23.451", thread: "t-02", source: "generated_04.log:1804", comment: "Нарушен ожидаемый разделитель между полями 4 и 5.", raw: "tokenize.fail record=1804 column=5 expected=TAB" },
        { level: "ERROR", date: "2026-10-21", time: "12:07:01.110", thread: "t-04", source: "generated_04.log:2291", comment: "Преобразование числового поля duration_ms завершилось ошибкой.", raw: "cast.error field=duration_ms value=\"n/a\"" },
        { level: "DEBUG", date: "2026-10-21", time: "12:04:17.026", thread: "t-01", source: "worker-pool:2", comment: "Создан пул из восьми рабочих потоков; очередь пока пуста.", raw: "pool.spawn workers=8 queue=0" },
        { level: "DEBUG", date: "2026-10-21", time: "12:04:17.332", thread: "t-07", source: "worker-pool:8", comment: "Задача 128 запущена повторно после временной блокировки.", raw: "worker.retry task=000128 attempt=2" },
        { level: "DEBUG", date: "2026-10-21", time: "12:08:44.920", thread: "t-05", source: "worker-pool:6", comment: "Пакет из 512 записей обработан за 14,6 мс.", raw: "batch.done size=512 elapsed=14.6ms" },
        { level: "FATAL", date: "2026-10-21", time: "12:04:17.501", thread: "t-06", source: "worker-pool:7", comment: "Рабочий поток завершился с кодом 139. Менеджер потоков инициировал восстановление.", raw: "worker.exit worker=06 code=139" },
        { level: "FATAL", date: "2026-10-21", time: "12:10:09.221", thread: "t-02", source: "worker-pool:3", comment: "Критическая ошибка выделения памяти во время декодирования блока.", raw: "memory.alloc_failed block=0x03B1 size=8388608" },
        { level: "FATAL", date: "2026-10-21", time: "12:11:54.013", thread: "t-01", source: "parser-core:1", comment: "Не удалось восстановить целостность текущего блока; обработка блока остановлена.", raw: "block.abort offset=00491882 reason=checksum" },
        { level: "TRACE", date: "2026-10-21", time: "12:04:17.244", thread: "t-07", source: "worker-pool:8", comment: "Задача 128 извлечена из очереди; измеренная задержка 0,8 мс.", raw: "queue.pop task=000128 latency=0.8ms" },
        { level: "TRACE", date: "2026-10-21", time: "12:04:17.246", thread: "t-07", source: "parser-core:8", comment: "Начало разбора поля status_code на смещении 812.", raw: "field.begin name=status_code offset=812" },
        { level: "TRACE", date: "2026-10-21", time: "12:04:17.247", thread: "t-07", source: "parser-core:8", comment: "Токен завершён; найдено 6 полей в текущей записи.", raw: "token.end count=6 cursor=834" },
        { level: "ALERT", date: "2026-10-21", time: "12:04:17.443", thread: "t-01", source: "stdin:1", comment: "Очередь ввода превысила порог ожидания; новые блоки временно задержаны.", raw: "io.backpressure stream=stdin pending=8192" },
        { level: "ALERT", date: "2026-10-21", time: "12:12:27.095", thread: "t-03", source: "worker-pool:4", comment: "Доля ошибок за последнее окно достигла 5%. Зафиксировано предупреждение оператору.", raw: "error.rate window=1000 value=5.0%" },
        { level: "ALERT", date: "2026-10-21", time: "12:13:04.500", thread: "t-01", source: "parser-core:1", comment: "Обнаружен резкий рост входного потока: 3,2× от среднего значения.", raw: "input.rate baseline=1200/s current=3840/s" },
        { level: "NOTICE", date: "2026-10-21", time: "12:04:17.128", thread: "t-02", source: "schema:1", comment: "Формат записи определён автоматически с высокой уверенностью.", raw: "schema.detect pattern=nginx_combined confidence=0.98" },
        { level: "NOTICE", date: "2026-10-21", time: "12:14:15.331", thread: "t-01", source: "source:1", comment: "Переключение на следующий файл журнала выполнено без остановки чтения.", raw: "source.rotate file=generated_05.log" },
        { level: "NOTICE", date: "2026-10-21", time: "12:14:48.906", thread: "t-04", source: "parser-core:5", comment: "Необязательное поле отсутствует; применено значение по умолчанию.", raw: "field.default name=region value=unknown" }
    ]

    Timer {
        id: demoTimer
        interval: 230
        repeat: true
        running: app.isRunning && !app.isPaused && app.processedLogs < app.targetLogs
        onTriggered: {
            var step = Math.max(1, Math.ceil(app.targetLogs / 90))
            app.processedLogs = Math.min(app.targetLogs, app.processedLogs + step)
            app.demoTicks += 1
            if (app.demoTicks % 2 === 0)
                app.appendTerminalEvent()
            if (app.processedLogs >= app.targetLogs) {
                app.isRunning = false
                app.isPaused = false
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Frameless, draggable window strip. It intentionally contains no app name or logo.
        Rectangle {
            id: chromeBar
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: palette.background

            MouseArea {
                anchors.fill: parent
                onPressed: app.startSystemMove()
            }

            Row {
                anchors.right: parent.right
                anchors.top: parent.top
                spacing: 0

                Rectangle {
                    width: 42; height: 30
                    color: minMouse.containsMouse ? palette.hover : palette.background
                    Text { anchors.centerIn: parent; text: "—"; color: palette.muted; font.pixelSize: 13 }
                    MouseArea { id: minMouse; anchors.fill: parent; hoverEnabled: true; onClicked: app.showMinimized() }
                }
                Rectangle {
                    width: 42; height: 30
                    color: maxMouse.containsMouse ? palette.hover : palette.background
                    Text { anchors.centerIn: parent; text: app.visibility === Window.Maximized ? "❐" : "□"; color: palette.muted; font.pixelSize: 11 }
                    MouseArea {
                        id: maxMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if (app.visibility === Window.Maximized) app.showNormal()
                            else app.showMaximized()
                        }
                    }
                }
                Rectangle {
                    width: 46; height: 30
                    color: closeMouse.containsMouse ? "#9F3F38" : palette.background
                    Text { anchors.centerIn: parent; text: "×"; color: closeMouse.containsMouse ? "#FFFFFF" : palette.muted; font.pixelSize: 17 }
                    MouseArea { id: closeMouse; anchors.fill: parent; hoverEnabled: true; onClicked: app.close() }
                }
            }
            Rectangle { anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 1; color: palette.edgeSoft }
        }

        ScrollView {
            id: pageScroll
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: availableWidth
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ColumnLayout {
                id: pageContent
                width: pageScroll.availableWidth
                spacing: 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.topMargin: 25
                    Layout.bottomMargin: 16
                    spacing: 16

                    ColumnLayout {
                        spacing: 5
                        Text {
                            text: "Обработка журналов"
                            color: palette.text
                            font.pixelSize: 22
                            font.weight: Font.DemiBold
                        }
                        Text {
                            text: "SOURCE  /  ./logs/generated_04.log"
                            color: palette.muted
                            font.family: "Consolas"
                            font.pixelSize: 11
                        }
                    }
                    Item { Layout.fillWidth: true }
                    ColumnLayout {
                        visible: app.isRunning || app.isPaused
                        spacing: 4
                        RowLayout {
                            spacing: 7
                            Rectangle { width: 6; height: 6; radius: 3; color: app.isPaused ? palette.amber : palette.lime }
                            Text {
                                text: app.isPaused ? "PAUSED" : "RUNNING"
                                color: app.isPaused ? palette.amber : palette.lime
                                font.family: "Consolas"
                                font.pixelSize: 10
                                font.bold: true
                            }
                        }
                        Text { text: "SESSION / 004"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                    }
                }

                // A low-profile metrics strip instead of floating summary cards.
                GridLayout {
                    id: metricsGrid
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.bottomMargin: 20
                    columns: app.width >= 1040 ? 4 : 2
                    columnSpacing: 0
                    rowSpacing: 0

                    Repeater {
                        model: [
                            { label: "ОБРАБОТАНО", value: app.formatNumber(app.processedLogs) + " / " + app.formatNumber(app.targetLogs), note: "records" },
                            { label: "WORKER POOL", value: String(app.workerCount), note: "threads" },
                            { label: "ERROR EVENTS", value: "12", note: "demo counter" },
                            { label: "INPUT MODE", value: "STREAM", note: "UTF-8 · auto schema" }
                        ]
                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 66
                            color: "transparent"
                            border.color: "transparent"
                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: 1
                                color: palette.edgeSoft
                                visible: index > 0 && (metricsGrid.columns === 4 || index % 2 === 1)
                            }
                            ColumnLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 14
                                anchors.rightMargin: 12
                                spacing: 5
                                Text { text: modelData.label; color: palette.dim; font.pixelSize: 9; font.bold: true; font.letterSpacing: 0.8 }
                                Text { text: modelData.value; color: palette.text; font.family: "Consolas"; font.pixelSize: 16; font.bold: true; elide: Text.ElideRight; Layout.fillWidth: true }
                                Text { text: modelData.note; color: palette.muted; font.family: "Consolas"; font.pixelSize: 9 }
                            }
                        }
                    }
                    Rectangle { Layout.fillWidth: true; Layout.columnSpan: metricsGrid.columns; Layout.preferredHeight: 1; color: palette.edgeSoft }
                }

                GridLayout {
                    id: dashboardGrid
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.bottomMargin: 18
                    columns: app.wideLayout ? 2 : 1
                    columnSpacing: 18
                    rowSpacing: 18

                    // LEFT: terminal output and controls.
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: app.wideLayout ? (dashboardGrid.width - dashboardGrid.columnSpacing) / 2 : dashboardGrid.width
                        Layout.alignment: Qt.AlignTop
                        spacing: 18

                        Rectangle {
                            id: terminalPanel
                            Layout.fillWidth: true
                            Layout.preferredHeight: 390
                            color: palette.panelDark
                            border.color: palette.edge
                            radius: 3

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 9

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Text { text: "user@node-04"; color: palette.lime; font.family: "Consolas"; font.pixelSize: 11; font.bold: true }
                                    Text { text: ":~/session$"; color: palette.muted; font.family: "Consolas"; font.pixelSize: 11 }
                                    Text { text: "tail -n 12 -f ./logs/generated_04.log"; color: palette.text; font.family: "Consolas"; font.pixelSize: 11; elide: Text.ElideRight; Layout.fillWidth: true }
                                    Text { text: app.isPaused ? "HOLD" : (app.isRunning ? "LIVE" : "LOCAL"); color: app.isRunning && !app.isPaused ? palette.lime : palette.dim; font.family: "Consolas"; font.pixelSize: 9; font.bold: true }
                                }
                                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: palette.edgeSoft }
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Text { text: "SEQ"; Layout.preferredWidth: 37; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                    Text { text: "TIME"; Layout.preferredWidth: 91; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                    Text { text: "LEVEL"; Layout.preferredWidth: 58; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                    Text { text: "STDOUT / RAW PAYLOAD"; Layout.fillWidth: true; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                    Text { text: "THR"; Layout.preferredWidth: 30; horizontalAlignment: Text.AlignRight; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                }
                                ListView {
                                    id: logList
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    clip: true
                                    spacing: 1
                                    model: logModel
                                    boundsBehavior: Flickable.StopAtBounds
                                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                                    delegate: Item {
                                        width: ListView.view.width
                                        height: 24
                                        Rectangle {
                                            anchors.fill: parent
                                            color: level === "ERROR" || level === "FATAL" ? "#1C110F" : (index % 2 === 0 ? "#0D100B" : "#0A0D09")
                                        }
                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: 2
                                            anchors.rightMargin: 2
                                            spacing: 8
                                            Text { text: seq; Layout.preferredWidth: 35; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9; verticalAlignment: Text.AlignVCenter }
                                            Text { text: timestamp; Layout.preferredWidth: 91; color: "#98A18F"; font.family: "Consolas"; font.pixelSize: 9; verticalAlignment: Text.AlignVCenter }
                                            Text { text: level; Layout.preferredWidth: 58; color: app.levelColor(level); font.family: "Consolas"; font.pixelSize: 9; font.bold: true; verticalAlignment: Text.AlignVCenter }
                                            Text { text: message; Layout.fillWidth: true; color: palette.text; font.family: "Consolas"; font.pixelSize: 9; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                                            Text { text: thread; Layout.preferredWidth: 30; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                                        }
                                    }
                                }
                                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: palette.edgeSoft }
                                RowLayout {
                                    Layout.fillWidth: true
                                    Text { text: "^C  detach stream"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                    Item { Layout.fillWidth: true }
                                    Text { text: logModel.count + " lines buffered"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: app.width >= 900 ? 255 : 322
                            color: palette.panel
                            border.color: palette.edge
                            radius: 3

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 15
                                spacing: 12

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text { text: "Параметры запуска"; color: palette.text; font.pixelSize: 14; font.bold: true }
                                    Item { Layout.fillWidth: true }
                                    Text { text: "[ CONFIG ]"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                }

                                GridLayout {
                                    Layout.fillWidth: true
                                    columns: app.width >= 900 ? 2 : 1
                                    columnSpacing: 12
                                    rowSpacing: 9

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 5
                                        Text { text: "Количество записей"; color: palette.muted; font.pixelSize: 10 }
                                        TextField {
                                            id: targetInput
                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 35
                                            text: "25000"
                                            selectByMouse: true
                                            placeholderText: "25000"
                                            color: palette.text
                                            font.family: "Consolas"
                                            font.pixelSize: 12
                                            validator: IntValidator { bottom: 1; top: 1000000000 }
                                            background: Rectangle {
                                                color: palette.panelDark
                                                border.color: targetInput.activeFocus ? palette.lime : palette.edge
                                                radius: 2
                                            }
                                            onAccepted: focus = false
                                        }
                                    }

                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 5
                                        Text { text: "Рабочие потоки"; color: palette.muted; font.pixelSize: 10 }
                                        SpinBox {
                                            id: workerInput
                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 35
                                            from: 1
                                            to: 64
                                            value: app.workerCount
                                            editable: true
                                            onValueChanged: app.workerCount = value
                                            contentItem: TextInput {
                                                text: workerInput.textFromValue(workerInput.value, workerInput.locale)
                                                color: palette.text
                                                font.family: "Consolas"
                                                font.pixelSize: 12
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                readOnly: !workerInput.editable
                                                validator: workerInput.validator
                                                selectByMouse: true
                                            }
                                            background: Rectangle {
                                                color: palette.panelDark
                                                border.color: workerInput.activeFocus ? palette.lime : palette.edge
                                                radius: 2
                                            }
                                        }
                                    }
                                }

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 6
                                    RowLayout {
                                        Layout.fillWidth: true
                                        Text { text: "Ход обработки"; color: palette.muted; font.pixelSize: 10 }
                                        Item { Layout.fillWidth: true }
                                        Text { text: Math.round(app.progress * 100) + "%  ·  " + app.formatNumber(app.processedLogs); color: palette.lime; font.family: "Consolas"; font.pixelSize: 10; font.bold: true }
                                    }
                                    Rectangle {
                                        id: progressTrack
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 5
                                        color: "#30382A"
                                        radius: 1
                                        clip: true
                                        Rectangle {
                                            width: progressTrack.width * app.progress
                                            height: parent.height
                                            color: palette.lime
                                            radius: 1
                                            Behavior on width { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                                        }
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 7
                                    Button {
                                        id: startButton
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 34
                                        text: app.isRunning ? "Выполняется…" : "Запустить"
                                        enabled: !app.isRunning
                                        onClicked: {
                                            var requested = parseInt(targetInput.text)
                                            if (!isNaN(requested) && requested > 0) app.targetLogs = requested
                                            else targetInput.text = String(app.targetLogs)
                                            if (app.processedLogs >= app.targetLogs) app.processedLogs = 0
                                            app.isRunning = true
                                            app.isPaused = false
                                        }
                                        background: Rectangle {
                                            color: !startButton.enabled ? "#424A38" : (startButton.down ? "#A4C565" : palette.lime)
                                            radius: 2
                                        }
                                        contentItem: Text {
                                            text: startButton.text
                                            color: "#18200F"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    Button {
                                        id: pauseButton
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 34
                                        text: app.isPaused ? "Продолжить" : "Пауза"
                                        enabled: app.isRunning
                                        onClicked: app.isPaused = !app.isPaused
                                        background: Rectangle {
                                            color: pauseButton.down ? palette.hover : palette.panelRaised
                                            border.color: palette.edge
                                            radius: 2
                                        }
                                        contentItem: Text {
                                            text: pauseButton.text
                                            color: pauseButton.enabled ? palette.text : palette.dim
                                            font.pixelSize: 10
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    Button {
                                        id: resetButton
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 34
                                        text: "Сброс"
                                        onClicked: {
                                            app.isRunning = false
                                            app.isPaused = false
                                            app.processedLogs = 0
                                            targetInput.text = String(app.targetLogs)
                                        }
                                        background: Rectangle {
                                            color: resetButton.down ? palette.hover : palette.panelRaised
                                            border.color: palette.edge
                                            radius: 2
                                        }
                                        contentItem: Text {
                                            text: resetButton.text
                                            color: palette.muted
                                            font.pixelSize: 10
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // RIGHT: compact distribution chart and clickable levels.
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.preferredWidth: app.wideLayout ? (dashboardGrid.width - dashboardGrid.columnSpacing) / 2 : dashboardGrid.width
                        Layout.alignment: Qt.AlignTop
                        spacing: 18

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 302
                            color: palette.panel
                            border.color: palette.edge
                            radius: 3

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 15
                                spacing: 8

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text { text: "Распределение по уровням"; color: palette.text; font.pixelSize: 14; font.bold: true }
                                    Item { Layout.fillWidth: true }
                                    Text { text: "N = 20 000"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                }
                                Text { text: "Количество записей / демонстрационная выборка"; color: palette.muted; font.pixelSize: 10 }

                                Canvas {
                                    id: distributionChart
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    antialiasing: true
                                    onWidthChanged: requestPaint()
                                    onHeightChanged: requestPaint()
                                    onPaint: {
                                        var ctx = getContext("2d")
                                        ctx.clearRect(0, 0, width, height)
                                        var left = 8
                                        var right = 7
                                        var top = 20
                                        var bottom = 30
                                        var plotW = Math.max(0, width - left - right)
                                        var plotH = Math.max(0, height - top - bottom)
                                        var maxValue = 8421
                                        ctx.lineWidth = 1
                                        ctx.strokeStyle = "#2A3225"
                                        for (var g = 0; g < 4; g++) {
                                            var gy = top + plotH * g / 3
                                            ctx.beginPath()
                                            ctx.moveTo(left, gy)
                                            ctx.lineTo(width - right, gy)
                                            ctx.stroke()
                                        }
                                        var slot = plotW / severityModel.count
                                        var barW = Math.min(30, slot * 0.5)
                                        for (var i = 0; i < severityModel.count; i++) {
                                            var entry = severityModel.get(i)
                                            var barH = plotH * entry.total / maxValue
                                            var bx = left + i * slot + (slot - barW) / 2
                                            var by = top + plotH - barH
                                            ctx.fillStyle = entry.accent
                                            ctx.fillRect(bx, by, barW, barH)
                                            ctx.fillStyle = "#929B87"
                                            ctx.font = "9px monospace"
                                            ctx.textAlign = "center"
                                            ctx.fillText(app.formatNumber(entry.total), bx + barW / 2, Math.max(11, by - 5))
                                            ctx.fillStyle = "#929B87"
                                            ctx.font = "9px monospace"
                                            var shortLabel = ["INFO", "WARN", "ERR", "DEBUG", "FATAL", "TRACE", "ALERT", "NOTE"][i]
                                            ctx.fillText(shortLabel, left + i * slot + slot / 2, height - 9)
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: app.width >= 1180 ? 260 : 430
                            color: palette.panel
                            border.color: palette.edge
                            radius: 3

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 15
                                spacing: 11

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text { text: "Уровни логирования"; color: palette.text; font.pixelSize: 14; font.bold: true }
                                    Item { Layout.fillWidth: true }
                                    Text { text: "CLICK TO INSPECT"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                                }
                                Text { text: "Выбери уровень, чтобы открыть список событий и комментарии."; color: palette.muted; font.pixelSize: 10; wrapMode: Text.WordWrap; Layout.fillWidth: true }

                                GridLayout {
                                    id: severityGrid
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    columns: app.width >= 1180 ? 4 : 2
                                    columnSpacing: 7
                                    rowSpacing: 7

                                    Repeater {
                                        model: severityModel
                                        delegate: Rectangle {
                                            id: levelTile
                                            Layout.fillWidth: true
                                            Layout.fillHeight: true
                                            Layout.minimumHeight: 76
                                            Layout.preferredHeight: 80
                                            color: tileMouse.containsMouse ? palette.hover : palette.panelDark
                                            border.color: tileMouse.containsMouse ? accent : palette.edgeSoft
                                            radius: 2

                                            ColumnLayout {
                                                anchors.fill: parent
                                                anchors.margins: 10
                                                spacing: 5
                                                RowLayout {
                                                    Layout.fillWidth: true
                                                    spacing: 5
                                                    Rectangle { Layout.preferredWidth: 4; Layout.preferredHeight: 4; color: accent }
                                                    Text { text: label; color: palette.muted; font.family: "Consolas"; font.pixelSize: 9; font.bold: true; Layout.fillWidth: true; elide: Text.ElideRight }
                                                    Text { text: "↗"; color: tileMouse.containsMouse ? accent : palette.dim; font.pixelSize: 11 }
                                                }
                                                RowLayout {
                                                    Layout.fillWidth: true
                                                    spacing: 5
                                                    Text { text: app.formatNumber(total); color: palette.text; font.family: "Consolas"; font.pixelSize: 17; font.bold: true; Layout.fillWidth: true }
                                                    Text { text: Number(share).toFixed(1) + "%"; color: accent; font.family: "Consolas"; font.pixelSize: 9 }
                                                }
                                                Rectangle {
                                                    Layout.fillWidth: true
                                                    Layout.preferredHeight: 2
                                                    color: "#30382A"
                                                    Rectangle { width: parent.width * share / 100; height: parent.height; color: accent }
                                                }
                                            }
                                            MouseArea {
                                                id: tileMouse
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                cursorShape: Qt.PointingHandCursor
                                                onClicked: app.openSeverity(label)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.leftMargin: 24
                    Layout.rightMargin: 24
                    Layout.bottomMargin: 18
                    spacing: 8
                    Text { text: "DEMO DATA"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                    Text { text: "·"; color: palette.dim; font.pixelSize: 9 }
                    Text { text: "counts and comments are synthetic"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                    Item { Layout.fillWidth: true }
                    Text { text: "QML / Qt Quick"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                }
            }
        }
    }

    Dialog {
        id: detailDialog
        modal: true
        focus: true
        width: Math.min(app.width - 34, 800)
        height: Math.min(app.height - 56, 650)
        x: Math.round((app.width - width) / 2)
        y: Math.round((app.height - height) / 2)
        padding: 0
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: palette.panel
            border.color: palette.edge
            border.width: 1
            radius: 3
        }

        contentItem: ColumnLayout {
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 72
                color: palette.panelRaised
                Rectangle { anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; width: 3; color: app.levelColor(app.selectedSeverity) }
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 18
                    anchors.rightMargin: 12
                    spacing: 12
                    ColumnLayout {
                        spacing: 4
                        RowLayout {
                            spacing: 8
                            Text { text: app.selectedSeverity; color: app.levelColor(app.selectedSeverity); font.family: "Consolas"; font.pixelSize: 15; font.bold: true }
                            Text { text: "EVENT INSPECTOR"; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                        }
                        Text { text: "Список событий выбранного уровня · дата, источник и комментарий"; color: palette.muted; font.pixelSize: 10; wrapMode: Text.WordWrap }
                    }
                    Item { Layout.fillWidth: true }
                    Rectangle {
                        Layout.preferredWidth: 30; Layout.preferredHeight: 30
                        color: closeDetailMouse.containsMouse ? palette.hover : "transparent"
                        border.color: palette.edgeSoft
                        radius: 2
                        Text { anchors.centerIn: parent; text: "×"; color: palette.muted; font.pixelSize: 18 }
                        MouseArea { id: closeDetailMouse; anchors.fill: parent; hoverEnabled: true; onClicked: detailDialog.close() }
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: palette.edge }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 29
                Layout.leftMargin: 14
                Layout.rightMargin: 14
                spacing: 12
                Text { text: "DATE / TIME"; Layout.preferredWidth: 132; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
                Text { text: "COMMENT / RAW EVENT"; Layout.fillWidth: true; color: palette.dim; font.family: "Consolas"; font.pixelSize: 9 }
            }

            ListView {
                id: detailList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 1
                model: app.filteredDetailLogs
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                delegate: Rectangle {
                    width: ListView.view.width
                    height: 100
                    color: index % 2 === 0 ? palette.panelDark : "#10140E"
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 5
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 12
                            ColumnLayout {
                                Layout.preferredWidth: 132
                                spacing: 4
                                Text { text: modelData.date; color: palette.text; font.family: "Consolas"; font.pixelSize: 10 }
                                Text { text: modelData.time; color: palette.lime; font.family: "Consolas"; font.pixelSize: 10 }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                Text { text: modelData.comment; color: palette.text; font.pixelSize: 11; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                                Text { text: modelData.source + "   ·   worker " + modelData.thread; color: palette.muted; font.family: "Consolas"; font.pixelSize: 9; elide: Text.ElideRight; Layout.fillWidth: true }
                            }
                        }
                        Text {
                            text: "> " + modelData.raw
                            color: app.levelColor(modelData.level)
                            font.family: "Consolas"
                            font.pixelSize: 9
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }
                    Rectangle { anchors.left: parent.left; anchors.right: parent.right; anchors.bottom: parent.bottom; height: 1; color: palette.edgeSoft }
                }
            }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: palette.edge }
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 38
                Layout.leftMargin: 14
                Layout.rightMargin: 14
                Text { text: app.filteredDetailLogs.length + " sample rows"; color: palette.muted; font.family: "Consolas"; font.pixelSize: 9 }
                Item { Layout.fillWidth: true }
                Text { text: "Данные демонстрационные"; color: palette.dim; font.pixelSize: 9 }
            }
        }
    }

    // Native resize gestures for the frameless window (Qt 5.15+ / Qt 6).
    MouseArea {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 4
        cursorShape: Qt.SizeVerCursor
        onPressed: app.startSystemResize(Qt.TopEdge)
    }
    MouseArea {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 4
        cursorShape: Qt.SizeVerCursor
        onPressed: app.startSystemResize(Qt.BottomEdge)
    }
    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 4
        cursorShape: Qt.SizeHorCursor
        onPressed: app.startSystemResize(Qt.LeftEdge)
    }
    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 4
        cursorShape: Qt.SizeHorCursor
        onPressed: app.startSystemResize(Qt.RightEdge)
    }
}
