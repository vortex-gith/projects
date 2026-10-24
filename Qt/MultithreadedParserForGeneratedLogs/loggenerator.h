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
    LogGenerator();

    std::string generateSupporter(const std::string& log_type, const std::string& comment);

    void generate(int log_count, SafeBuffer& SafeVectorForSimpleWrite);
};

#endif // LOGGENERATOR_H