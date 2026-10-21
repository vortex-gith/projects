#ifndef LOGGENERATOR_H
#define LOGGENERATOR_H

#include <mutex>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <utility>

#include "safebuffer.h"

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
                {
                    std::unique_lock<std::mutex> lock(SafeVectorForSimpleWrite.mtx);
                    SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
                    raw_vector_logs.clear();
                }
                SafeVectorForSimpleWrite.cv.notify_one();
            }
        }
        if (!raw_vector_logs.empty())
        {
            {
                std::unique_lock<std::mutex> lock(SafeVectorForSimpleWrite.mtx);
                SafeVectorForSimpleWrite.safe_vector.push_back(std::move(raw_vector_logs));
                raw_vector_logs.clear();
            }
            SafeVectorForSimpleWrite.cv.notify_one();
        }

        {
            std::unique_lock <std::mutex> lock(SafeVectorForSimpleWrite.mtx);
            SafeVectorForSimpleWrite.is_done = true;
        }

        SafeVectorForSimpleWrite.cv.notify_all();
    }

    ~LogGenerator() {}

};

#endif // LOGGENERATOR_H
