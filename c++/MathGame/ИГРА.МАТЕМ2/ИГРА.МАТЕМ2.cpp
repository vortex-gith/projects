#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>

const int MIN_VALUE = 0;
const int MAX_VALUE = 10;
const int TOTAL_QUESTIONS = 12;

enum GameState {
    QUESTION,
    RESULT,
    GAME_OVER
};

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Функция для центрирования текста
void centerText(sf::Text& text, float windowWidth, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
    text.setPosition(windowWidth / 2.0f, y);
}

int main() {
    srand(time(0));

    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        L"Математическая игра",
        sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        if (!font.loadFromFile("arial.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                return 1;
            }
        }
    }

    // Градиентный фон
    sf::RectangleShape bgTop(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2));
    bgTop.setFillColor(sf::Color(138, 43, 226)); // Фиолетовый
    bgTop.setPosition(0, 0);

    sf::RectangleShape bgBottom(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT / 2));
    bgBottom.setFillColor(sf::Color(75, 0, 130)); // Индиго
    bgBottom.setPosition(0, WINDOW_HEIGHT / 2);

    // Главная карточка
    sf::RectangleShape card(sf::Vector2f(700, 600));
    card.setFillColor(sf::Color(255, 255, 255, 250));
    card.setPosition((WINDOW_WIDTH - 700) / 2, 100);

    // Тень карточки
    sf::RectangleShape cardShadow(sf::Vector2f(700, 600));
    cardShadow.setFillColor(sf::Color(0, 0, 0, 30));
    cardShadow.setPosition((WINDOW_WIDTH - 700) / 2 + 8, 108);

    // Заголовок
    sf::Text title(sf::String(L"Математическая игра"), font, 42);
    title.setFillColor(sf::Color(75, 0, 130));
    title.setStyle(sf::Text::Bold);
    centerText(title, WINDOW_WIDTH, 160);

    // Прогресс бар фон
    sf::RectangleShape progressBg(sf::Vector2f(600, 12));
    progressBg.setFillColor(sf::Color(230, 230, 230));
    progressBg.setPosition((WINDOW_WIDTH - 600) / 2, 220);

    // Прогресс бар заполнение
    sf::RectangleShape progressFill(sf::Vector2f(0, 12));
    progressFill.setFillColor(sf::Color(76, 175, 80)); // Зелёный
    progressFill.setPosition((WINDOW_WIDTH - 600) / 2, 220);

    // Счётчик вопросов
    sf::Text counterText("", font, 22);
    counterText.setFillColor(sf::Color(120, 120, 120));

    // Карточка с вопросом
    sf::RectangleShape questionCard(sf::Vector2f(500, 140));
    questionCard.setFillColor(sf::Color(248, 249, 250));
    questionCard.setPosition((WINDOW_WIDTH - 500) / 2, 280);

    // Текст вопроса
    sf::Text questionText("", font, 72);
    questionText.setFillColor(sf::Color(33, 33, 33));
    questionText.setStyle(sf::Text::Bold);

    // Поле ввода
    sf::RectangleShape inputBox(sf::Vector2f(200, 70));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(3);
    inputBox.setOutlineColor(sf::Color(138, 43, 226));
    inputBox.setPosition((WINDOW_WIDTH - 200) / 2, 450);

    // Текст ввода
    sf::Text inputText("", font, 48);
    inputText.setFillColor(sf::Color(138, 43, 226));
    inputText.setStyle(sf::Text::Bold);

    // Сообщение о результате
    sf::Text messageText("", font, 32);

    // Подсказка
    sf::Text hintText("", font, 20);
    hintText.setFillColor(sf::Color(150, 150, 150));
    hintText.setStyle(sf::Text::Italic);

    // Финальный экран
    sf::Text finalTitle("", font, 52);
    finalTitle.setStyle(sf::Text::Bold);

    sf::Text finalScore("", font, 36);

    sf::Text finalPercentage("", font, 28);
    finalPercentage.setFillColor(sf::Color(100, 100, 100));

    sf::Text finalHint("", font, 24);
    finalHint.setFillColor(sf::Color(120, 120, 120));

    //// Индикатор результата (галочка или крестик)
    //sf::CircleShape resultCircle(40);
    //resultCircle.setFillColor(sf::Color::Transparent);
    //resultCircle.setOutlineThickness(5);
    //resultCircle.setPosition((WINDOW_WIDTH - 80) / 2, 465);

    //// Линии для галочки/крестика
    //sf::RectangleShape line1(sf::Vector2f(0, 5));
    //sf::RectangleShape line2(sf::Vector2f(0, 5));

    int a, b, correctAnswer;
    char op;
    int solved = 0;
    int correct = 0;
    std::string input;
    GameState state = QUESTION;
    bool isCorrectAnswer = false;

    auto newQuestion = [&]() {
        a = getRandom(MIN_VALUE, MAX_VALUE);
        b = getRandom(MIN_VALUE, MAX_VALUE);
        op = (rand() % 2 == 0) ? '+' : '-';

        if (op == '+') {
            while (a + b > MAX_VALUE) {
                a = getRandom(MIN_VALUE, MAX_VALUE);
                b = getRandom(MIN_VALUE, MAX_VALUE);
            }
            correctAnswer = a + b;
        }
        else {
            if (a < b) std::swap(a, b);
            correctAnswer = a - b;
        }

        questionText.setString(
            sf::String(std::to_string(a) + "  " + op + "  " + std::to_string(b) + "  =  ?")
        );
        centerText(questionText, WINDOW_WIDTH, 350);

        input.clear();
        inputText.setString("");
        messageText.setString("");

        counterText.setString(
            std::to_string(solved + 1) + " / " + std::to_string(TOTAL_QUESTIONS)
        );
        centerText(counterText, WINDOW_WIDTH, 245);

        hintText.setString(sf::String(L"Введите ответ и нажмите Enter"));
        centerText(hintText, WINDOW_WIDTH, 555);

        // Обновление прогресс бара
        float progress = (float)solved / TOTAL_QUESTIONS;
        progressFill.setSize(sf::Vector2f(600 * progress, 12));
        };

    newQuestion();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == QUESTION && event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    if (input.length() < 2) {
                        input += static_cast<char>(event.text.unicode);
                        inputText.setString(input);
                        centerText(inputText, WINDOW_WIDTH, 485);
                    }
                }
                else if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                    inputText.setString(input);
                    centerText(inputText, WINDOW_WIDTH, 485);
                }
                else if (event.text.unicode == '\r' && !input.empty()) {
                    int answer = std::stoi(input);
                    isCorrectAnswer = (answer == correctAnswer);

                    if (isCorrectAnswer) {
                        correct++;
                        messageText.setString(sf::String(L"Правильно!"));
                        messageText.setFillColor(sf::Color(76, 175, 80));
                        /*resultCircle.setOutlineColor(sf::Color(76, 175, 80));*/
                        inputBox.setOutlineColor(sf::Color(76, 175, 80));
                    }
                    else {
                        messageText.setString(
                            sf::String(L"Неверно. Правильный ответ: ")
                            + sf::String(std::to_string(correctAnswer))
                        );
                        messageText.setFillColor(sf::Color(244, 67, 54));
                    /*    resultCircle.setOutlineColor(sf::Color(244, 67, 54));*/
                        inputBox.setOutlineColor(sf::Color(244, 67, 54));
                    }
                    centerText(messageText, WINDOW_WIDTH, 555);

                    hintText.setString(sf::String(L"Нажмите Enter для продолжения"));
                    centerText(hintText, WINDOW_WIDTH, 600);

                    state = RESULT;
                }
            }

            if (state == RESULT && event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {

                solved++;
                inputBox.setOutlineColor(sf::Color(138, 43, 226));

                if (solved == TOTAL_QUESTIONS) {
                    state = GAME_OVER;
                    double percentage = (correct * 100.0) / TOTAL_QUESTIONS;

                    if (percentage >= 90) {
                        finalTitle.setString(sf::String(L"ПРЕВОСХОДНО!"));
                        finalTitle.setFillColor(sf::Color(255, 193, 7));
                    }
                    else if (percentage >= 70) {
                        finalTitle.setString(sf::String(L"ОТЛИЧНО!"));
                        finalTitle.setFillColor(sf::Color(76, 175, 80));
                    }
                    else if (percentage >= 50) {
                        finalTitle.setString(sf::String(L"ХОРОШО!"));
                        finalTitle.setFillColor(sf::Color(33, 150, 243));
                    }
                    else {
                        finalTitle.setString(sf::String(L"ПРОДОЛЖАЙ ПРАКТИКОВАТЬСЯ!"));
                        finalTitle.setFillColor(sf::Color(255, 152, 0));
                    }
                    centerText(finalTitle, WINDOW_WIDTH, 240);

                    finalScore.setString(
                        sf::String(L"Правильных ответов: ")
                        + sf::String(std::to_string(correct))
                        + sf::String(L" из ")
                        + sf::String(std::to_string(TOTAL_QUESTIONS))
                    );
                    finalScore.setFillColor(sf::Color(33, 33, 33));
                    centerText(finalScore, WINDOW_WIDTH, 330);

                    std::ostringstream percentStream;
                    percentStream << std::fixed << std::setprecision(0) << percentage;
                    finalPercentage.setString(
                        sf::String(L"Точность: ") +
                        sf::String(percentStream.str()) +
                        sf::String(L"%")
                    );
                    centerText(finalPercentage, WINDOW_WIDTH, 390);

                    finalHint.setString(
                        sf::String(L"\n\nНажми R - сыграть ещё раз\n")
                        + sf::String(L"Нажми Esc - выход")
                    );
                    centerText(finalHint, WINDOW_WIDTH, 480);
                }
                else {
                    newQuestion();
                    state = QUESTION;
                }
            }

            if (state == GAME_OVER && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    solved = 0;
                    correct = 0;
                    newQuestion();
                    state = QUESTION;
                }
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        // Отрисовка
        window.clear();

        // Фон
        window.draw(bgTop);
        window.draw(bgBottom);

        if (state != GAME_OVER) {
            // Тень и карточка
            window.draw(cardShadow);
            window.draw(card);

            // Заголовок
            window.draw(title);

            // Прогресс бар
            window.draw(progressBg);
            window.draw(progressFill);
            window.draw(counterText);

            // Карточка вопроса
            window.draw(questionCard);
            window.draw(questionText);

            // Поле ввода
            window.draw(inputBox);

            if (state == QUESTION) {
                window.draw(inputText);
            }
            else {
                // Рисуем круг с галочкой или крестиком
        /*        window.draw(resultCircle);*/

                //if (isCorrectAnswer) {
                //    // Галочка (две линии)
                //    line1.setSize(sf::Vector2f(25, 5));
                //    line1.setFillColor(sf::Color(76, 175, 80));
                //    line1.setPosition(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT / 2 + 10);
                //    line1.setRotation(45);

                //    line2.setSize(sf::Vector2f(40, 5));
                //    line2.setFillColor(sf::Color(76, 175, 80));
                //    line2.setPosition(WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2 + 15);
                //    line2.setRotation(-45);

                //    window.draw(line1);
                //    window.draw(line2);
                //}
                //else {
                //    // Крестик (две линии)
                //    line1.setSize(sf::Vector2f(50, 5));
                //    line1.setFillColor(sf::Color(244, 67, 54));
                //    line1.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT / 2 + 5);
                //    line1.setRotation(45);

                //    line2.setSize(sf::Vector2f(50, 5));
                //    line2.setFillColor(sf::Color(244, 67, 54));
                //    line2.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT / 2 + 5);
                //    line2.setRotation(-45);

                //    window.draw(line1);
                //    window.draw(line2);
                //}

                window.draw(messageText);
            }

            window.draw(hintText);
        }
        else {
            // Финальный экран
            window.draw(cardShadow);
            window.draw(card);
            window.draw(finalTitle);
            window.draw(finalScore);
            window.draw(finalPercentage);
            window.draw(finalHint);
        }

        window.display();
    }

    return 0;
}