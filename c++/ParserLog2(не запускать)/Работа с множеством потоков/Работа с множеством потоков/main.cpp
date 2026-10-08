#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

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

std::mutex mtx;
bool EndOfStream = false;
bool globalAccesFlagThread1 = false;
bool globalAccesFlagThread2 = false;

std::string generateSupporter(const std::string& log_type, const std::string& comment, std::string& currentLog);
void accesToVector(std::vector<std::string>& queueLogs, int flag, std::ofstream& outLogs);
void generateLogs(long long current, const std::vector <std::string>& warning_list, const std::vector <std::string>& comment_list, std::vector<std::string>& queueLogs, std::vector<std::string>& intermediateQueue);
void putInTheMainQueue(std::vector<std::string>& intermediateQueue, std::vector<std::string>& queueLogs, int flag);
void coutNotFormattedLogs(std::vector<std::string>& queueLogs);
void parseLogs(std::vector<std::string>& queueLogs);
void coutParsedLogs(std::vector<std::string>& queueLogs);

std::string generateSupporter(const std::string& log_type, const std::string& comment, std::string& currentLog)
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

void accesToVector(std::vector<std::string>& queueLogs, int flag, std::ofstream& outLogs)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (queueLogs.size() > 999)
    {
        std::vector<std::string> copyQueue = std::move(queueLogs);

        switch (flag)
        {
          case 0:
          {
              // вывожу данные в txt
              for (std::string currentString : copyQueue)
              {
                  outLogs << currentString;
              }



              globalAccesFlagThread1 = true;
              break;
          }
          case 1:
          {
              //вывожу паршенные логи в txt


              globalAccesFlagThread2 = true;
              break;
          }
        }

        queueLogs = std::move(copyQueue);
        if (globalAccesFlagThread1 == true && globalAccesFlagThread2 == true)
        {
            queueLogs.clear();
        }

    }
        // не надо?? queueLogs = std::move(copyQueue);
}

void generateLogs(long long current, const std::vector <std::string>& warning_list, const std::vector <std::string>& comment_list, std::vector<std::string>& queueLogs, std::vector<std::string>& intermediateQueue)
{
    std::string currentLog;
    int supporter = current / 1000;

    while (supporter != 0)
    {
        int subSupporter = 1000;
        while (subSupporter != 0)
        {
            int random_number = rand() % warning_list.size();
            int count = 0;

            count = (rand() % 3) + random_number * 3;
            std::string comment = comment_list[count];

            currentLog = generateSupporter(warning_list[random_number], comment, currentLog);
            intermediateQueue.push_back(currentLog);
            subSupporter--;
        }
        putInTheMainQueue(intermediateQueue, queueLogs, 0);
        supporter--;
        intermediateQueue.clear();
    }
    EndOfStream = true;
}

void putInTheMainQueue(std::vector<std::string>& intermediateQueue, std::vector<std::string>& queueLogs, int flag)
{
    // заполняю основную очередь по циклу
    std::lock_guard<std::mutex> lock(mtx);
    static int counter = 0;
    if ((globalAccesFlagThread1 == true && globalAccesFlagThread2 == true) || counter == 0)
    for (std::string currentString : intermediateQueue)
    {
        queueLogs.push_back(currentString);
    }
    counter++;
    globalAccesFlagThread1 = false;
    globalAccesFlagThread2 = false;
}


void coutNotFormattedLogs(std::vector<std::string>& queueLogs)
{
    std::ofstream outLogs("Logs.txt");
 //  if (!std::filesystem::exists("Logs.txt"))
 //  {
 //      outLogs.open("Logs.txt", std::ios::app);
 //  }
 //  else outLogs.open("Logs.txt", std::ios::trunc);
 
 
    while (EndOfStream == false)
    {
        accesToVector(queueLogs, 0, outLogs);
    
    
    }
    
    outLogs.close();
}

void parseLogs(std::vector<std::string>& queueLogs)
{
    while (EndOfStream == false)
    {


    }
    //парсинг




}

void coutParsedLogs(std::vector<std::string>& queueLogs)
{
    std::ofstream outLogs("parsedLogs.txt");
 // if (!std::filesystem::exists("parsedLogs.txt"))
 // {
 //     outLogs.open("parsedLogs.txt", std::ios::app);
 // }
 // else outLogs.open("parsedLogs.txt", std::ios::trunc);
  
    while (EndOfStream == false)
    {
        accesToVector(queueLogs, 1, outLogs);
    }

    outLogs.close();
}

int main()
{
    std::ios::sync_with_stdio(false);
    setlocale(0, "");
    srand(time(0));

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

    long long logs_number;
    static int counter = 0;
    std::vector<std::string> queueLogs;
    std::vector<std::string> intermediateQueue;

    std::cout << "Введите количество сгенерируесых логов: ";
    std::cin >> logs_number; std::cout << "\n";

	std::thread generateLogs(generateLogs, logs_number, warning_list, comment_list, queueLogs, intermediateQueue);
    std::thread coutNotFormattedLogs(coutNotFormattedLogs, queueLogs);
    std::thread parseLogs(parseLogs, queueLogs);
    std::thread coutParsedLogs(coutParsedLogs, queueLogs);

    generateLogs.join();
    coutNotFormattedLogs.join();
    parseLogs.join();
    coutParsedLogs.join();
}


//"короче код у меня говно полное. мне надо структурировать свою работу и подвергнуть ее четкой структуризации. Как я могу это сделать что бы не держать все в голове?"