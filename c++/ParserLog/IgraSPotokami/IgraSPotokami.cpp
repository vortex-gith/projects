#include <iostream>

#include <string>
#include <ctime>
#include <iomanip>
#include <cstdlib>

#include <filesystem>
#include <fstream>

#include <vector>
#include <map>

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


void generateSupporter(std::string log_type, std::string comment)
{
    static int counter = 0;
    std::time_t t = (rand() * 1000) + 1755689600;
    std::tm now;
    localtime_s(&now, &t);

    if (std::filesystem::exists("logs.txt") && std::filesystem::file_size("logs.txt") > 0 && counter == 0)
    {
        std::ofstream clear("logs.txt", std::ios::trunc);
        counter++;
    }

    std::ofstream inLogs("logs.txt", std::ios::app);

    if (!inLogs.is_open())
        std::cerr << "Файл не записывается!\n";
    else
    {
        inLogs << " ["
            << std::setfill('0') << std::setw(2) << now.tm_mday << "."
            << std::setfill('0') << std::setw(2) << (now.tm_mon + 1) << "."
            << (now.tm_year + 1900) << "] ["
            << std::setfill('0') << std::setw(2) << now.tm_hour << ":"
            << std::setfill('0') << std::setw(2) << now.tm_min << "] "
            << "[" << log_type << "] " << comment << std::endl;
        inLogs.close();
    }
}



void generateLogs(size_t& current)
{
    while (current != 0)
    {
        int random_number = rand() % warning_list.size();
        switch (random_number)
        {
        case 0:
        {
            int count = (rand() % 3) + 0;
            std::string comment = comment_list[count];
            generateSupporter("INFO", comment);
            break;
        }
        case 1:
        {
            int count = (rand() % 3) + 3;
            std::string comment = comment_list[count];
            generateSupporter("WARNING", comment);
            break;
        }
        case 2:
        {
            int count = (rand() % 3) + 6;
            std::string comment = comment_list[count];
            generateSupporter("ERROR", comment);
            break;
        }
        case 3:
        {
            int count = (rand() % 3) + 9;
            std::string comment = comment_list[count];
            generateSupporter("DEBUG", comment);
            break;
        }
        case 4:
        {
            int count = (rand() % 3) + 12;
            std::string comment = comment_list[count];
            generateSupporter("FATAL", comment);
            break;
        }
        case 5:
        {
            int count = (rand() % 3) + 15;
            std::string comment = comment_list[count];
            generateSupporter("TRACE", comment);
            break;
        }
        case 6:
        {
            int count = (rand() % 3) + 18;
            std::string comment = comment_list[count];
            generateSupporter("ALERT", comment);
            break;
        }
        case 7:
        {
            int count = (rand() % 3) + 21;
            std::string comment = comment_list[count];
            generateSupporter("NOTICE", comment);
            break;
        }
        }
        current--;
    }
}

void parseLogs()
{
    struct logInfo
    {
        int count = 0;
        std::map <std::string, int> logCommentCount;
        std::map <std::string, std::vector<std::string>> logCommentDate;
    };
    std::map<std::string, logInfo> log_vector;

    std::string inputString;
    std::ifstream outLogs("logs.txt");

    if (!outLogs.is_open())
        std::cerr << "Файл не читается!\n";
    else
    {
        while (std::getline(outLogs, inputString))
        {
            size_t pos1 = inputString.find("[");
            size_t pos2 = inputString.find("]");

            std::string newInputStringDate = inputString.substr(pos1, pos2 - pos1 + 1);

            size_t pos3 = inputString.find("[", pos2);
            size_t pos4 = inputString.find("]", pos3);

            std::string newInputStringTime = inputString.substr(pos3, pos4 - pos3 + 1);

            size_t pos5 = inputString.find("[", pos4);
            size_t pos6 = inputString.find("]", pos5);

            std::string newInputStringLvl = inputString.substr(pos5, pos6 - pos5 + 1);

            std::string newInputStringComment = inputString.substr(pos6 + 1);

            logInfo& info = log_vector[newInputStringLvl];

            info.count++;
            info.logCommentCount[newInputStringComment]++;
            info.logCommentDate[newInputStringComment].push_back(newInputStringDate);
        }
        outLogs.close();

        for (const auto& [lvl, info] : log_vector)
        {
            int counterI = 1;
            std::cout << "\nУровень логирования: " << lvl;
            std::cout << "\n\tИнформация о нем:";
            std::cout << "\n\t[ ] Сколько раз встретился этот уровень: " << info.count;

            std::cout << "\n\t[ ] Какие комментарии содержит:";
            for (const auto& [comment, count] : info.logCommentCount)
            {
                std::cout << "\n\t\t" << counterI++ << ")" << "Комментарий: " << comment;
                std::cout << "\n\t\tСколько раз встретился этот комментарий: " << count;

                std::cout << "\n";


                std::cout << "\n\t[ ] Комментарий: " << comment;
                const std::vector<std::string>& date = info.logCommentDate.at(comment);
                for (const std::string& d : date)
                {
                    std::cout << "\n\t\tДаты в которые появился комментарий: " << d;
                }

            }
            std::cout << "\n";
        }
        /*
        for (const auto& [lvl, info] : log_vector)
        {
            int counterI = 1;
            std::cout << "\nУровень логирования: " << lvl;
            std::cout << "\n\tИнформация о нем:";
            std::cout << "\n\t[ ] Сколько раз встретился этот уровень: " << info.count;

            std::cout << "\n\t[ ] Какие комментарии содержит:";
            for (const auto& [comment, count] : info.logCommentCount)
            {
                std::cout << "\n\t\t" << counterI++ << ")" << "Комментарий: " << comment;
                std::cout << "\n\t\tСколько раз встретился этот комментарий: " << count;

                std::cout << "\n";


                std::cout << "\n\t[ ] Комментарий: " << comment;
                const std::vector<std::string>& date = info.logCommentDate.at(comment);
                for (const std::string& d : date)
                {
                    std::cout << "\n\t\tДаты в которые появился комментарий: " << d;
                }

            }
            std::cout << "\n";
        }
        */
    }
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    setlocale(0, "");
    srand(time(0));
    size_t logs_number;
    std::cout << "Введите количество сгенерируесых логов: ";
    std::cin >> logs_number; std::cout << "\n";
    if (logs_number < 0)
    {
        std::cout << "Логов должно быть больше 0!\n";
    }
    else
    {
        generateLogs(logs_number);
        parseLogs();
        std::cout << "\n\nОткрываю файл с логами...\n";
        system("start logs.txt");
        system("pause");
    }
    return 0;
}