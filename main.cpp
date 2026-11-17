#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cctype>

struct Question {
    std::string stage;
    std::string title;
    std::string prompt;
    std::string code;
    std::array<std::string, 3> options;
    int correct;
    std::string explanation;
};

int readChoice(int minimum, int maximum) {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::size_t first = line.find_first_not_of(" \t");
        if (first != std::string::npos) {
            std::size_t last = line.find_last_not_of(" \t");
            if (first == last && line[first] >= '0' && line[first] <= '9') {
                int choice = line[first] - '0';
                if (choice >= minimum && choice <= maximum) return choice;
            }
        }
        std::cout << "Некорректный ввод. Введите число от " << minimum
                  << " до " << maximum << ": " << std::flush;
    }
    return -1;
}

bool waitEnter() {
    std::string line;
    while (true) {
        std::cout << "Нажмите Enter для продолжения: " << std::flush;
        if (!std::getline(std::cin, line)) return false;
        if (line.find_first_not_of(" \t") == std::string::npos) return true;
        std::cout << "Для продолжения нужна пустая строка.\n";
    }
}

void showMenu() {
    std::cout << "\nМастерская памяти\n"
              << "1. Начать новую игру\n2. Правила\n0. Выход\nВаш выбор: " << std::flush;
}

void showRules() {
    std::cout << "\nПравила: 4 этапа, 10 вопросов, 3 варианта ответа.\n"
              << "За верный ответ +10 баллов, за неверный +0. Максимум 100.\n"
              << "90-100: Отличное понимание темы\n"
              << "70-89: Уверенное понимание материала\n"
              << "50-69: Базовые знания, рекомендуется повторение\n"
              << "0-49: Стоит повторить материал\n"
              << "После пояснения нажмите Enter.\n";
}

bool askQuestion(const Question& q, int number, int& correctCount) {
    std::cout << "\n" << q.stage << "\n"
              << "Вопрос " << number << "/10. " << q.title << "\n"
              << q.prompt << "\n";
    if (!q.code.empty()) std::cout << "\n" << q.code << "\n";
    for (int i = 0; i < 3; ++i)
        std::cout << i + 1 << ") " << q.options[i] << "\n";
    std::cout << "Ответ (1-3): " << std::flush;
    int answer = readChoice(1, 3);
    if (answer == -1) return false;
    if (answer == q.correct) {
        ++correctCount;
        std::cout << "Верно\n";
    } else {
        std::cout << "Неверно\n";
    }
    std::cout << "Правильный ответ: " << q.correct << ") "
              << q.options[q.correct - 1] << "\n"
              << "Пояснение: " << q.explanation << "\n";
    return waitEnter();
}

void showResult(int correctCount) {
    int score = correctCount * 10;
    std::cout << "\nРезультат\nПравильно: " << correctCount
              << " из 10\nБаллы: " << score << " из 100\n";
    if (score >= 90) std::cout << "Отличное понимание темы\n";
    else if (score >= 70) std::cout << "Уверенное понимание материала\n";
    else if (score >= 50) std::cout << "Базовые знания, рекомендуется повторение\n";
    else std::cout << "Стоит повторить материал\n";
}

bool playGame(const std::vector<Question>& questions) {
    int correctCount = 0;
    for (std::size_t i = 0; i < questions.size(); ++i) {
        if (!askQuestion(questions[i], static_cast<int>(i) + 1, correctCount)) {
            std::cout << "\nИгра прервана. Итоговая оценка не присваивается.\n";
            return false;
        }
    }
    showResult(correctCount);
    return true;
}

int main() {
    const std::vector<Question> questions = {
    {R"QUIZ(Опасная копия)QUIZ", R"QUIZ(Копирование указателя)QUIZ", R"QUIZ(Выполняется RawArray b = a. Что делает неявный конструктор копирования с полем data?)QUIZ", R"QUIZ(struct RawArray {
    int* data = new int[3]{10, 20, 30};
    ~RawArray() { delete[] data; }
};

RawArray a;
RawArray b = a;
b.data[0] = 99;)QUIZ", {R"QUIZ(Создаёт новый буфер и копирует в него элементы.)QUIZ", R"QUIZ(Копирует значение указателя: оба объекта ссылаются на один буфер.)QUIZ", R"QUIZ(Не компилируется, потому что объявлен деструктор.)QUIZ"}, 2, R"QUIZ(Неявное копирование копирует значение указателя, а не выполняет поэлементную копию. Оба объекта ошибочно считают себя владельцами одного буфера.)QUIZ"},
    {R"QUIZ(Опасная копия)QUIZ", R"QUIZ(Общие данные)QUIZ", R"QUIZ(Сразу после записи b.data[0] = 99, до разрушения объектов, какое значение хранится в a.data[0]?)QUIZ", R"QUIZ(struct RawArray {
    int* data = new int[3]{10, 20, 30};
    ~RawArray() { delete[] data; }
};

RawArray a;
RawArray b = a;
b.data[0] = 99;)QUIZ", {R"QUIZ(99, потому что оба указателя ведут к одному элементу.)QUIZ", R"QUIZ(10, потому что b всегда имеет независимый буфер.)QUIZ", R"QUIZ(Такая запись не компилируется.)QUIZ"}, 1, R"QUIZ(Оба указателя обращаются к одному буферу, поэтому изменение через b видно и через a до разрушения объектов.)QUIZ"},
    {R"QUIZ(Опасная копия)QUIZ", R"QUIZ(Разрушение объектов)QUIZ", R"QUIZ(Что неправильно при уничтожении a и b, если их указатели всё ещё содержат один адрес?)QUIZ", R"QUIZ(struct RawArray {
    int* data = new int[3]{10, 20, 30};
    ~RawArray() { delete[] data; }
};

RawArray a;
RawArray b = a;
b.data[0] = 99;)QUIZ", {R"QUIZ(Ничего: delete[] автоматически считает владельцев.)QUIZ", R"QUIZ(Второй delete[] обязан бросить std::bad_alloc.)QUIZ", R"QUIZ(Один буфер освобождается повторно, возникает undefined behavior.)QUIZ"}, 3, R"QUIZ(Оба деструктора пытаются освободить один буфер. Повторный delete[] вызывает undefined behavior, но не гарантирует конкретного результата.)QUIZ"},
    {R"QUIZ(Правильное владение)QUIZ", R"QUIZ(Порядок присваивания)QUIZ", R"QUIZ(В этом операторе копируются int. Какое утверждение верно?)QUIZ", R"QUIZ(IntArray& operator=(const IntArray& other) {
    int* fresh = other.size ? new int[other.size] : nullptr;
    for (std::size_t i = 0; i < other.size; ++i)
        fresh[i] = other.data[i];
    delete[] data;
    data = fresh;
    size = other.size;
    return *this;
})QUIZ", {R"QUIZ(До new надо выполнить delete[] data, иначе старый буфер утечёт.)QUIZ", R"QUIZ(Если new бросит, объект сохранится; a = a здесь также безопасно.)QUIZ", R"QUIZ(Любое a = a вызывает double free, если нет отдельного if.)QUIZ"}, 2, R"QUIZ(Сначала выделяется новый буфер и выполняется поэлементная копия, поэтому при исключении new прежний объект сохраняется; самоприсваивание безопасно. Конструктор копирования создаёт новый объект, а копирующее присваивание заменяет значение существующего.)QUIZ"},
    {R"QUIZ(Правильное владение)QUIZ", R"QUIZ(Правила трёх, пяти и нуля)QUIZ", R"QUIZ(Какой подход к специальным членам класса корректен?)QUIZ", R"QUIZ()QUIZ", {R"QUIZ(Согласовать деструктор, copy и при необходимости move; при владении через RAII-поля предпочесть правило нуля.)QUIZ", R"QUIZ(Деструктор автоматически запрещает copy и реализует move.)QUIZ", R"QUIZ(Правило нуля запрещает классу владеть std::vector.)QUIZ"}, 1, R"QUIZ(При ручном владении ресурсом нужно согласованно определить деструктор, копирование и при необходимости перемещение. RAII-поля позволяют следовать правилу нуля.)QUIZ"},
    {R"QUIZ(Передача ресурсов)QUIZ", R"QUIZ(Смысл std::move)QUIZ", R"QUIZ(Что делает само выражение std::move(a)?)QUIZ", R"QUIZ()QUIZ", {R"QUIZ(Немедленно обнуляет все поля a.)QUIZ", R"QUIZ(Безусловно вызывает перемещающий конструктор.)QUIZ", R"QUIZ(Даёт xvalue; изменение ресурса выполняет выбранная затем операция.)QUIZ"}, 3, R"QUIZ(std::move(a) преобразует выражение в xvalue, но само не переносит ресурс. Дальнейшая выбранная операция может выполнить перемещение или копирование.)QUIZ"},
    {R"QUIZ(Передача ресурсов)QUIZ", R"QUIZ(Перемещающее присваивание)QUIZ", R"QUIZ(Предположим, data - единолично владеющий int*, size - размер. Какой план подходит для operator=(IntArray&& other) с контрактом «self-move сохраняет значение»?)QUIZ", R"QUIZ()QUIZ", {R"QUIZ(Если this == &other, вернуть *this. Иначе освободить свой буфер, принять поля other, обнулить other и вернуть *this.)QUIZ", R"QUIZ(Скопировать поля other, оставив источник без изменений, чтобы оба объекта владели буфером.)QUIZ", R"QUIZ(Вызвать delete[] other.data, затем скопировать этот адрес в data.)QUIZ"}, 1, R"QUIZ(Проверка this == &other сохраняет значение при self-move. В остальных случаях свой буфер освобождается, владение передаётся, а источник обнуляется.)QUIZ"},
    {R"QUIZ(Передача ресурсов)QUIZ", R"QUIZ(Состояние источника)QUIZ", R"QUIZ(Контракт учебного IntArray: после перемещения в другой объект источник имеет size == 0 и data == nullptr. Какие действия допустимы?)QUIZ", R"QUIZ()QUIZ", {R"QUIZ(Объект уже уничтожен; использовать его нельзя никак.)QUIZ", R"QUIZ(Прочитать size, проверить data, присвоить новое значение или уничтожить объект; a[0] читать нельзя.)QUIZ", R"QUIZ(Прочитать a[0]: прежний элемент обязательно сохранился в источнике.)QUIZ"}, 2, R"QUIZ(После перемещения источник остаётся живым объектом: его можно проверять, присваивать и уничтожать. По контракту этого IntArray он пуст, поэтому предусловие индексации a[0] не выполнено; для других типов гарантии могут отличаться.)QUIZ"},
    {R"QUIZ(Операторы под контролем)QUIZ", R"QUIZ(Сравнение содержимого)QUIZ", R"QUIZ(a и b независимо владеют буферами {10, 20, 30}. Их адреса различны. operator== сравнивает размер и элементы. Чему равно a == b?)QUIZ", R"QUIZ()QUIZ", {R"QUIZ(false, потому что адреса различаются.)QUIZ", R"QUIZ(Выражение нельзя вычислить для двух объектов.)QUIZ", R"QUIZ(true, потому что размеры и значения элементов совпадают.)QUIZ"}, 3, R"QUIZ(Оператор сравнивает содержимое: одинаковый размер и одинаковые элементы дают true, даже если буферы расположены по разным адресам.)QUIZ"},
    {R"QUIZ(Операторы под контролем)QUIZ", R"QUIZ(Константная индексация)QUIZ", R"QUIZ(Есть две перегрузки ниже. a - обычный массив, c - const-массив, оба размера 3. Какое утверждение верно?)QUIZ", R"QUIZ(int& operator[](std::size_t i) { return data[i]; }
const int& operator[](std::size_t i) const { return data[i]; })QUIZ", {R"QUIZ(c[0] = 5 допустимо, потому что метод const.)QUIZ", R"QUIZ(a[0] = 5 и int x = c[0] допустимы; c[0] = 5 не компилируется.)QUIZ", R"QUIZ(a[10] = 5 обязательно бросит исключение.)QUIZ"}, 2, R"QUIZ(Неконстантная перегрузка возвращает int&, а константная const int&, поэтому c[0] изменять нельзя. Индексация требует допустимого индекса; operator[] сам по себе не обязан бросать исключение.)QUIZ"}
    };
    while (true) {
        showMenu();
        int choice = readChoice(0, 2);
        if (choice == -1 || choice == 0) break;
        if (choice == 2) showRules();
        else if (!playGame(questions)) break;
    }
    return 0;
}
