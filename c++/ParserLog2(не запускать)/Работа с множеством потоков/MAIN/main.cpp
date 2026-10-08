#include <iostream>
#include <filesystem>
#include <fstream>

#include <thread>
#include <atomic>
#include <mutex>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>

#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <functional> // std::ref

struct SafeBuffer
{
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::vector<std::string>> safe_vector;
    bool is_done = false;
};

class LogGenerator
{
private:
    std::vector <std::string> warning_list =
    {
        "INFO", "WARNING", "ERROR", "DEBUG", "FATAL", "TRACE", "ALERT", "NOTICE"
    };
    std::vector <std::string> comment_list =
    {
        "Сервер успешно запущен на порту 8080", "Пользователь admin авторизовался в системе", "Завершено плановое резервное копирование данных",
         "Свободное место на диске менее 15%", "SSL сертификат истекает через 7 дней", "Медленный ответ от базы данных (3.2 сек)",
          "Не удалось подключиться к базе данных: timeout", "Файл конфигурации не найден по пути /etc/app/config.json", "Превышена квота памяти для процесса (лимит 2 ГБ)",
           "Запрос: GET /api/v1/users/123", "Ответ: код 200, тело ответа 1456 байт", "Кэш очищен, количество элементов: 42",
            "Невосстановимая ошибка ядра: segmentation fault", "Аварийное завершение: таймаут watchdog", "Критическая утечка памяти: потребление превысило 90%",
             "Вызов функции calculatePrice с аргументами (100, 'USD')", "Выход из функции calculatePrice, результат: 8500", "Вход в цикл рендеринга, итерация 42",
              "Обнаружено подозрительное количество запросов от IP 192.168.1.100", "Попытка несанкционированного доступа к панели администратора", "Служба мониторинга не отвечает в течение 5 минут",
               "Установлено новое подключение от клиента 10.0.0.45", "Очередь задач пуста, система переходит в режим ожидания", "Смена временного слота: день, активен основной сервер"
    };

public:
    LogGenerator(int log_count, SafeBuffer& SafeVector)
    {
        srand(time(0));
    }

    std::string generateSupporter(const std::string& log_type, const std::string& comment)
    {
        std::time_t t = (static_cast<std::time_t>(rand()) * 1000) + 1755689600;
        std::tm now;
        localtime_s(&now, &t);

        std::string day_str = (now.tm_mday < 10 ? "0" : "") + std::to_string(now.tm_mday);
        std::string mon_str = ((now.tm_mon + 1) < 10 ? "0" : "") + std::to_string(now.tm_mon + 1);
        std::string year_str = std::to_string(now.tm_year + 1900);
        std::string hour_str = (now.tm_hour < 10 ? "0" : "") + std::to_string(now.tm_hour);
        std::string min_str = (now.tm_min < 10 ? "0" : "") + std::to_string(now.tm_min);

        return " [" + day_str + "." + mon_str + "." + year_str + "] ["
            + hour_str + ":" + min_str + "] [" + log_type + "] " + comment + "\n";

    }

    void generate(int log_count, SafeBuffer& SafeVectorForSimpleWrite)
    {
        std::vector<std::string> raw_vector_logs;
        std::string currentLog;
        
        int counter_supporter = log_count;

        for (int i = 0; i < log_count; i++)                             
        {
            int random_number = rand() % warning_list.size();
            int count = 0;

            count = (rand() % 3) + random_number * 3;
            std::string comment = comment_list[count];

            currentLog = generateSupporter(warning_list[random_number], comment);
            raw_vector_logs.push_back(currentLog);

            counter_supporter--;
 
            if (raw_vector_logs.size() == 3000)
            {
                SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
                raw_vector_logs.clear();
            }
        }
        if (!raw_vector_logs.empty())
        {
            SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
            raw_vector_logs.clear();
        }

    }

    ~LogGenerator() {}

    /*void generate(int log_count)
    {
        std::vector<std::string> raw_vector_logs;
        std::string currentLog;
        
        int log_count_counter = log_count;
        int log_count_counter_supporter;

        if (log_count > 3000)
        {
            if (log_count % 3000 != 0)
                log_count_counter_supporter = std::ceil( log_count / 3000);
            else if (log_count % 3000 == 0)
                log_count_counter_supporter = log_count / 3000;
                
            while (log_count_counter_supporter != 0)
            {
                int counter = 0;
                while (counter != 3000 && log_count_counter != 0)
                {
                    int random_number = rand() % warning_list.size();
                    int count = 0;

                    count = (rand() % 3) + random_number * 3;
                    std::string comment = comment_list[count];

                    currentLog = generateSupporter(warning_list[random_number], comment);
                    raw_vector_logs.push_back(currentLog);

                    counter++;
                    log_count_counter--;
                }
                log_count_counter_supporter--;
            }
        }

        else
        {
            int counter = 0;
            while (counter != 3000 && log_count_counter != 0)
            {
                int random_number = rand() % warning_list.size();
                int count = 0;

                count = (rand() % 3) + random_number * 3;
                std::string comment = comment_list[count];

                currentLog = generateSupporter(warning_list[random_number], comment);
                raw_vector_logs.push_back(currentLog);

                counter++;
                log_count_counter--;
            }
        }
        SafeVector = std::move(raw_vector_logs);
    }*/
    /*
    std::string generateSupporter(const std::string& log_type, const std::string& comment)
    {
        std::time_t t = (static_cast<std::time_t>(rand()) * 1000) + 1755689600;
        std::tm now;
        localtime_s(&now, &t);

        std::stringstream inputString;

        inputString << " ["
            << std::setfill('0') << std::setw(2) << now.tm_mday << "."
            << std::setfill('0') << std::setw(2) << (now.tm_mon + 1) << "."
            << (now.tm_year + 1900) << "] ["
            << std::setfill('0') << std::setw(2) << now.tm_hour << ":"
            << std::setfill('0') << std::setw(2) << now.tm_min << "] "
            << "[" << log_type << "] " << comment << "\n";

        return inputString.str();
    }
    */
};

class RawLogsWriter
{
private:
    SafeBuffer currentSafeVector;
public:
    RawLogsWriter(SafeBuffer& SafeVectorForSimpleWriter, SafeBuffer& SafeVectorForParse) {}

    void write(SafeBuffer& SafeVectorForParse, SafeBuffer& SafeVectorForSimpleWriter)
    {
        currentSafeVector.safe_vector = std::move(SafeVectorForSimpleWriter.safe_vector);
        std::ofstream rawLogWriter("RawLogs.txt");
        for (std::vector<std::string>& vector : currentSafeVector.safe_vector)
        {
            for (std::string& string : vector)
            {
                rawLogWriter << string;
            }
        }

        SafeVectorForParse.safe_vector = std::move(currentSafeVector.safe_vector);
    }

    ~RawLogsWriter() {}
};

class ParseLogs
{
private:
    SafeBuffer currentSafeVector;

    struct LogDateTime { std::string date; std::string time; };
    struct logInfo
    {
        int count = 0;
        std::unordered_map <std::string, int> logCommentCount;
        std::unordered_map <std::string, std::vector<LogDateTime>> logCommentDateTime;
    };
    std::unordered_map<std::string, logInfo> current_log_vector;
public:
    ParseLogs(SafeBuffer& SafeVectorForParse, std::unordered_map<std::string, logInfo>& log_vector) {}
    
    void parse(SafeBuffer& SafeVectorForParsedWriter, std::unordered_map<std::string, logInfo>& log_vector, SafeBuffer& SafeVectorForParse)
    {
        SafeVectorForParse.mtx.lock();
        currentSafeVector.safe_vector = std::move(SafeVectorForParse.safe_vector);
        while (!currentSafeVector.safe_vector.empty())
        {
            for (std::vector<std::string>& vector : currentSafeVector.safe_vector)
            {
                for (std::string& inputString : vector)
                {
                    size_t pos1 = inputString.find("[");
                    size_t pos2 = inputString.find("]");

                    std::string newInputStringDate = "";
                    if (pos1 == std::string::npos || pos2 == std::string::npos) continue;
                    newInputStringDate = inputString.substr(pos1, pos2 - pos1 + 1);

                    size_t pos3 = inputString.find("[", pos2);
                    size_t pos4 = inputString.find("]", pos3);

                    std::string newInputStringTime = "";
                    if (pos3 == std::string::npos || pos4 == std::string::npos) continue;
                    newInputStringTime = inputString.substr(pos3, pos4 - pos3 + 1);

                    size_t pos5 = inputString.find("[", pos4);
                    size_t pos6 = inputString.find("]", pos5);

                    std::string newInputStringLvl = "";
                    if (pos5 == std::string::npos || pos6 == std::string::npos) continue;
                    newInputStringLvl = inputString.substr(pos5, pos6 - pos5 + 1);

                    std::string newInputStringComment = "";
                    if (pos6 + 1 == std::string::npos || pos6 + 1 >= inputString.size()) continue;
                    newInputStringComment = inputString.substr(pos6 + 1);

                    logInfo& info = log_vector[newInputStringLvl];

                    info.count++;
                    info.logCommentCount[newInputStringComment]++;
                    info.logCommentDateTime[newInputStringComment].push_back({ newInputStringDate, newInputStringTime });
                }
            }
        }

        log_vector = std::move(current_log_vector);
    }

    ~ParseLogs() {}
};

class ParsedLogWriter
{
private:
    struct LogDateTime { std::string date; std::string time; };
    struct logInfo
    {
        int count = 0;
        std::unordered_map <std::string, int> logCommentCount;
        std::unordered_map <std::string, std::vector<LogDateTime>> logCommentDateTime;
    };
    std::unordered_map<std::string, logInfo> current_log_vector;
public:
    ParsedLogWriter(std::unordered_map<std::string, logInfo>& log_vector) {}

    void write(std::unordered_map<std::string, logInfo>& log_vector)
    {
        current_log_vector = std::move(log_vector);
        std::ofstream parsedLogWriter("ParsedLogs.txt");

        for (const auto& [lvl, info] : current_log_vector)
        {
            int counterI = 1;
            parsedLogWriter << "\nУровень логирования: " << lvl;
            parsedLogWriter << "\n\tИнформация о нем:";
            parsedLogWriter << "\n\t[ ] Сколько раз встретился этот уровень: " << info.count;

            parsedLogWriter << "\n\t[ ] Какие комментарии содержит:";
            for (const auto& [comment, count] : info.logCommentCount)
            {
                parsedLogWriter << "\n\t\t" << counterI++ << ")" << "Комментарий: " << comment;
                parsedLogWriter << "\n\t\tСколько раз встретился этот комментарий: " << count;

                const std::vector<LogDateTime>& keyDateTime = info.logCommentDateTime.at(comment);

                for (int i = 0; i < keyDateTime.size(); i++)
                {
                    parsedLogWriter << "\n\t\tДаты в которые появился комментарий: " << keyDateTime[i].date << " " << keyDateTime[i].time;
                }
                parsedLogWriter << "\n";
            }
        }
    }

    ~ParsedLogWriter() {}
};

int main()
{
    std::ios::sync_with_stdio(false);
	setlocale(0, "");

    size_t log_count = 0;
    bool is_done = false;

    try
    {
        std::cout << "Введите количество сгенерируемых логов: ";
        std::cin >> log_count;
    }
    catch (std::invalid_argument& error)
    {
        std::cerr << "Недопустимое значение!";
        return 0;
    }
    catch (...)
    {
        std::cerr << "Ошибка ввода.";
        return 0;
    }

    struct LogDateTime { std::string date; std::string time; };
    struct logInfo
    {
        int count = 0;
        std::unordered_map <std::string, int> logCommentCount;
        std::unordered_map <std::string, std::vector<LogDateTime>> logCommentDateTime;
    };

    std::unordered_map<std::string, logInfo> log_vector;

    SafeBuffer SafeVectorForSimpleWriter;
    SafeBuffer SafeVectorForParse;

    LogGenerator Generator(log_count, SafeVectorForSimpleWriter);
    RawLogsWriter RawWriter(SafeVectorForSimpleWriter, SafeVectorForParse);
    ParseLogs Parser(SafeVectorForParse, log_vector);
    ParsedLogWriter ParsedWriter(log_vector);


    std::thread Generate(&LogGenerator::generate, &Generator, log_count, std::ref(SafeVectorForSimpleWriter));
    std::thread PutRawLogs(&RawLogsWriter::write, &RawWriter, std::ref(SafeVectorForParse), std::ref(SafeVectorForSimpleWriter));
    std::thread ParseLogs(&ParseLogs::parse, &Parser, std::ref(SafeVectorForSimpleWriter), std::ref(log_vector), std::ref(SafeVectorForParse));
    std::thread PutParseLogs(&ParsedLogWriter::write, &ParsedWriter, std::ref(log_vector));

    Generate.join();
    PutRawLogs.join();
    ParseLogs.join();
    PutParseLogs.join();
}