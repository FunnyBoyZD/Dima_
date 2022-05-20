#define _CRT_SECURE_NO_WARNINGS
#include "stdlib.h"
#include "stdio.h"
#include "locale.h"
#include "windows.h"
#include "time.h"

struct Student
{
    char* surname;
    int groupNum,
        mathMark,
        ukrainianMark,
        chemistryMark,
        biologyMark,
        PEMark,
        birthDay,
        birthMonth,
        birthYear;
};

/*Структура вузла списку*/
struct ListElement {
    Student data;
    ListElement* next; /*Покажчик на наступний елемент списку*/
    ListElement* previous; /*Покажчик на наступний елемент списку*/
};

/*Формат дескриптора списку*/
struct ListDescriptor {
    ListElement* begin; /*Покажчик на початок списку*/
    int size; /*Поточний розмір списку(кількість елементів)*/
};

/*Прототипи функцій*/
ListDescriptor* listCreation();
ListElement* utilitySE(ListElement* node, int position, const int& count);
ListElement* searchPreviousElement(ListElement* node, int count, int* i);
ListElement* utilityIEE(ListElement* node, int size, int* i);
int insertElementStart(ListDescriptor* sd, Student student);
int swapElements(const int& firstPosition, const int& secondPosition, ListDescriptor* ld);
int insertElementEnd(ListDescriptor* ld, const Student& number);
int elementCounter(FILE*& fd);
int numberScan(int& N);
int destroyList(ListDescriptor* sd);
int readFromFile(FILE* fp, int size, Student* array);
int insertAfterAnElement(ListDescriptor* ld, const Student& student, int count);
int utilityIAAE(ListElement* ln, ListElement* node, ListDescriptor* ld, int* i, const Student& student, int count);
void checkFilename(char filename[]);
void fopenCheck(FILE*& fp, char filename[], char d);
void printTheList(ListDescriptor* ld);
void printUtility(ListElement* node, int* i, int size);
void deleteElement(ListDescriptor* ld, int count);

int main() {
    FILE* fr = NULL; //для встановлення потоку з файлом
    int counter = 0; //для кількості елементів у файлі
    int check = 0;
    int count = 0; //для кількості елементів для генерації або для номеру елементу списку
    char filename[50]; //з назвою файлу
    Student* array = NULL; //на масив array

    SetConsoleCP(1251); //підключення кирилиці
    SetConsoleOutputCP(1251);

    srand((unsigned int)time(NULL));
    ListDescriptor* ld = listCreation(); //створюємо список

    if (ld == NULL) {
        printf("Список не створений!");
        exit(0);
    }

    printf("Список створено!\n");

    printf("Введіть назву файлу у вигляді name.txt, з якого бажаєте зчитати дані:\n");
    checkFilename(filename); //перевірка правильності введення назви файла
    fopenCheck(fr, filename, 'r'); //встановлення потоку для роботи з файлом у режимі зчитування та перевірка на помилки
    counter = elementCounter(fr); //підрахунок кількості елементів у файлі
    array = (Student*)malloc((counter) * sizeof(Student));
    if (!array)
    {
        printf("Не вдалося виділити пам'ять під масив!\n");
        return -1;
    }
    if (readFromFile(fr, counter, array) == -1) //перенесення елементів з файлу до масиву
    {
        printf("Не вдалося виділити пам'ять під рядок!\n");
        return -1;
    }
    for (int i = 0; i < counter; i++)
    {
        check = insertElementEnd(ld, array[i]);
        if (check == -2)
        {
            printf("Списку не існує!\n");
            return -1;
        }
        else if (check == -1)
        {
            printf("Не вдалося виділити пам'ять під новий елемент(список повний)!\n");
            return -1;
        }
        else if (check == -3)
        {
            printf("Не вдалося виділити пам'ять під рядок!\n");
            return -1;
        }
    }
    fclose(fr);
    printf("\n\nУтворений двохзв'язний список на основі даних із файлу:\n");
    printTheList(ld);
    Student newStudent;
    newStudent.surname = (char*)malloc(sizeof(char) * 256);
    if (!newStudent.surname)
    {
        printf("Не вдалося виділити пам'ять під рядок!\n");
        return -1;
    }
    strcpy(newStudent.surname, "Danchenko");
    newStudent.biologyMark = 80;
    newStudent.birthDay = 10;
    newStudent.birthMonth = 10;
    newStudent.birthYear = 2002;
    newStudent.chemistryMark = 85;
    newStudent.groupNum = 123;
    newStudent.mathMark = 90;
    newStudent.PEMark = 78;
    newStudent.ukrainianMark = 99;
    check = insertAfterAnElement(ld, newStudent, 4);
    if (check == -2)
    {
        printf("Списку не існує!\n");
        return -1;
    }
    else if (check == -1)
    {
        printf("Список пустий!\n");
        return -1;
    }
    else if (check == -3)
    {
        printf("Не вдалося виділити пам'ять під рядок!\n");
        return -1;
    }
    printf("\n\nСписок після додавання нового студента:\n");
    printTheList(ld);

    if (swapElements(1, 5, ld) == -2)
    {
        printf("Список не існує!\n");
        return -1;
    }

    printf("\n\nСписок після того, як 5 і 1 студентів поміняли місцями:\n");
    printTheList(ld);

    deleteElement(ld, ld->size);
    printf("\n\nСписок після видалення останнього студента:\n");
    printTheList(ld);

    destroyList(ld);
}

/*Функція створення списку
Вихід:
Адреса на створений список
*/
ListDescriptor* listCreation() {
    ListDescriptor* ld; //оголошення дескриптору списку
    ld = (ListDescriptor*)malloc(sizeof(ListDescriptor)); //виділення пам'яті під дескриптор списку

    if (!ld)
        return NULL;/*список не створений*/

    else { /*список утворений*/
        ld->begin = NULL; /*ініціалізація покажчика на вершину списку*/
        ld->size = 0; /*ініціалізація розміру списку*/
        return ld;
    }
}

ListElement* utilitySE(ListElement* node, int position, const int& count)
{
    if (position + 1 == count)
    {
        return node;
    }
    else
    {
        position++;
        utilitySE(node->next, position, count);
    }
}

/*
Функція пошуку вузла, що стоїть перед тим, який буде видалений
Вхід:
Покажчик node на вузол списку
Змінна count з номером наступного елементу
Адреса змінної i з номером поточного елементу
Вихід:
Адреса на шуканий вузол списку
*/
ListElement* searchPreviousElement(ListElement* node, int count, int* i)
{
    ListElement* temp = NULL;
    temp = node;
    if (*i + 2 == count)
    {
        return temp;
    }
    else
    {
        *i = *i + 1;
        searchPreviousElement(node->next, count, i);
    }
}

/*
Функція пошуку останнього вузлу в списку
Вхід:
Покажчик node на вузол списку
Покажчик ln на елемент, який потрібно вставити до списку
Змінна size з номером останнього елементу
Адреса змінної i з номером поточного елементу
Вихід:
Адреса на останній вузол списку
*/
ListElement* utilityIEE(ListElement* node, int size, int* i)
{
    if (*i < size)
    {
        *i = *i + 1;
        utilityIEE(node->next, size, i);
    }
    else
    {
        return node;
    }
}

int insertElementStart(ListDescriptor* ld, Student student) {
    ListElement* ln; /*покажчик на новий елемент списку*/
    if (!ld) /*список не існує!*/
        return -2;
    ln = (ListElement*)malloc(sizeof(ListElement)); /*виділення пам'яті під новий елемент списку*/
    if (!ln) /*не вистачає пам'яті (список повний)*/
        return -1;
    ln->data.surname = (char*)malloc(sizeof(char) * 256);
    if (!ln->data.surname)
    {
        return -3;
    }
    strcpy(ln->data.surname, student.surname);
    ln->data.birthDay = student.birthDay;
    ln->data.birthMonth = student.birthMonth;
    ln->data.birthYear = student.birthYear;
    ln->data.chemistryMark = student.chemistryMark;
    ln->data.groupNum = student.groupNum;
    ln->data.mathMark = student.mathMark;
    ln->data.PEMark = student.PEMark;
    ln->data.ukrainianMark = student.ukrainianMark;
    ln->data.biologyMark = student.biologyMark;
    ln->next = ld->begin; /*встановлення покажчика на наступний елемент списку*/
    ln->previous = ld->begin->previous;
    ld->begin->previous->next = ln;
    ld->begin->previous = ln;
    ld->begin = ln;
    ld->size++; /*зміна поточного розміру списку*/
    return 0;
}

int swapElements(const int& firstPosition, const int& secondPosition, ListDescriptor* ld)
{
    if (!ld) /*список не існує!*/
        return -2;
    ListElement* firstElement = NULL, * secondElement = NULL;
    int i = 0;
    firstElement = utilityIEE(ld->begin, firstPosition - 1, &i);
    i = 0;
    secondElement = utilityIEE(ld->begin, secondPosition - 1, &i);
    if (firstPosition == 1)
    {
        insertElementStart(ld, secondElement->data);
    }
    else
    {
        insertAfterAnElement(ld, secondElement->data, firstPosition - 1);
    }
    deleteElement(ld, secondPosition + 1);

    insertAfterAnElement(ld, firstElement->data, secondPosition);
    deleteElement(ld, firstPosition + 1);
    return 0;
}

int insertElementEnd(ListDescriptor* ld, const Student& student) {
    ListElement* ln; /*покажчик на новий елемент списку*/
    ListElement* temp;
    int i = 0;
    if (!ld) /*список не існує!*/
        return -2;
    ln = (ListElement*)malloc(sizeof(ListElement)); /*виділення пам'яті під новий елемент списку*/
    if (!ln) /*не вистачає пам'яті (список повний)*/
        return -1;
    ln->data.surname = (char*)malloc(sizeof(char) * 256);
    if (!ln->data.surname)
    {
        return -3;
    }
    strcpy(ln->data.surname, student.surname);
    ln->data.birthDay = student.birthDay;
    ln->data.birthMonth = student.birthMonth;
    ln->data.birthYear = student.birthYear;
    ln->data.chemistryMark = student.chemistryMark;
    ln->data.groupNum = student.groupNum;
    ln->data.mathMark = student.mathMark;
    ln->data.PEMark = student.PEMark;
    ln->data.ukrainianMark = student.ukrainianMark;
    ln->data.biologyMark = student.biologyMark;
    if (!ld->begin)
    {
        ld->begin = ln;
        ld->begin->next = ld->begin;
        ld->begin->previous = ld->begin;
        ld->size++; /*зміна поточного розміру списку*/
        return 0;
    }
    temp = utilityIEE(ld->begin, ld->size - 1, &i); //пошук останнього елементу списку
    ln->next = ld->begin;
    ld->begin->previous = ln;
    temp->next = ln;
    ln->previous = temp;
    ld->size++; /*зміна поточного розміру списку*/
    return 0;
}

/*Функція для підрахунку кількості елементів у файлі
Значення, що повертається функцією:
Кількість елементів у файлі count
Зміст параметрів, що передаються:
Файловий дескриптор fd*/
int elementCounter(FILE*& fd) {
    int count = 0; //змінна для підрахунку кількості елементів, що зберігається у файлі

    while (!feof(fd)) {
        fscanf(fd, "%*s");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        fscanf(fd, "%*d");
        count++;
    }
    rewind(fd);
    if (ferror(fd)) {
        printf("Не переміщено показник в початок файлу\n");
        exit(0);
    }
    return count;
}

/*Функція введення значень змінних
Вхід:
Змінна для заповнення, передача за посиланням
Вихід:
Змінна з записаним числом
*/
int numberScan(int& N) {
    int entered = 0; //змінна для контролю введених користувачем з клавіатури символів
    do {
        do {
            entered = scanf_s("%d", &N);
            while (getchar() != '\n');
            while (entered < 1) {
                printf("Ви ввели некоректне значення, повторіть введення: ");
                entered = scanf_s("%d", &N);
                while (getchar() != '\n');
            }
        } while (entered < 1);
    } while (entered != 1);
    return N;
}

/*Функція видалення списку
Вхід:
Дескриптор списку, який буде видалений
Вихід:
-2-у випадку, якщо списку не існує
0-при нормальному завершення роботи функції
*/
int destroyList(ListDescriptor* ld) {
    ListElement* pDel; /*адреса тимчасового елемента списку*/

    if (!ld) /*список не існує*/
        return -2;

    /*спершу слід видалити всі елементи!*/
    for (int i = 0; i < ld->size; i++)
    {
        pDel = ld->begin; /*збереження адреси вершини списку для видалення*/
        ld->begin = ld->begin->next; /*задання верхівкою списку елемент, який іде після неї*/
        free(pDel->data.surname);
        free(pDel); /*видалення колишньої верхівки списку*/
    }
    free(ld); /*звільняється пам'ять дескриптора списку*/
    return 0;
}

/*Функція для зчитування даних з файлу і запису їх до комірок масиву array[]
Зміст параметрів, що передаються:
Файловий дескриптор fp
Змінна, яка зберігає значення кількості елементів у файлі size
Вказівник на масив array[]*/
int readFromFile(FILE* fp, int size, Student* array)
{
    for (int i = 0; i < size; i++) {
        array[i].surname = (char*)malloc(sizeof(char) * 256);
        if (!array[i].surname)
        {
            return -1;
        }
        fscanf(fp, "%s", array[i].surname);
        fscanf(fp, "%d", &array[i].groupNum);
        fscanf(fp, "%d", &array[i].mathMark);
        fscanf(fp, "%d", &array[i].ukrainianMark);
        fscanf(fp, "%d", &array[i].chemistryMark);
        fscanf(fp, "%d", &array[i].biologyMark);
        fscanf(fp, "%d", &array[i].PEMark);
        fscanf(fp, "%d", &array[i].birthDay);
        fscanf(fp, "%d", &array[i].birthMonth);
        fscanf(fp, "%d", &array[i].birthYear);
    }
    return 0;
}

int insertAfterAnElement(ListDescriptor* ld, const Student& student, int count)
{
    int i = 0;
    if (!ld)
    {
        printf("Списку не існує!");
        return -2;
    }
    if (!ld->begin)
    {
        printf("Список пустий!\n");
        return -1;
    }
    ListElement* ln = NULL;
    ln = (ListElement*)malloc(sizeof(ListElement)); /*виділення пам'яті під новий елемент списку*/
    if (utilityIAAE(ln, ld->begin, ld, &i, student, count) == -1) //вставлення елементу після заданого вузла списку
    {
        return -3;
    }
    return 0;
}

int utilityIAAE(ListElement* ln, ListElement* node, ListDescriptor* ld, int* i, const Student& student, int count)
{
    if (*i + 1 == count)
    {
        ln->data.surname = (char*)malloc(sizeof(char) * 256);
        if (!ln->data.surname)
        {
            return -1;
        }
        strcpy(ln->data.surname, student.surname);
        ln->data.birthDay = student.birthDay;
        ln->data.birthMonth = student.birthMonth;
        ln->data.birthYear = student.birthYear;
        ln->data.chemistryMark = student.chemistryMark;
        ln->data.groupNum = student.groupNum;
        ln->data.mathMark = student.mathMark;
        ln->data.PEMark = student.PEMark;
        ln->data.ukrainianMark = student.ukrainianMark;
        ln->data.biologyMark = student.biologyMark;
        ln->next = node->next;
        ln->next->previous = ln;
        ln->previous = node;
        node->next = ln;
        ld->size++;
        return 0;
    }
    else
    {
        *i = *i + 1;
        utilityIAAE(ln, node->next, ld, i, student, count);
    }
    return 0;
}

/*Функція для перевірки правильності введення назви файлів
Вхід:
Масив, у якому записана назва файлу, яку ввів користувач filename[]
*/
void checkFilename(char filename[]) {
    int fcheck = 0; //змінна для контролю правильності вводу назви файлу користувачем
    int scheck = 0; //змінна для контролю правильності вводу назви файлу користувачем
    do {
        fgets(filename, 1024, stdin);
        fcheck = 1;
        scheck = 0;
        for (int i = 0; i < (int)strlen(filename); i++)
        {
            if (filename[i] == '.' && i != 0)
            {
                scheck = 1;
                if (filename[i + 1] != 't' || filename[i + 2] != 'x' || filename[i + 3] != 't' || filename[i + 4] != '\n')
                {
                    printf("Ви ввели розширення не вірно, спробуйте ще раз!\n");
                    fcheck = 0;
                }
            }
        }
        if (filename[0] == '\n')
        {
            printf("Ви нічого не ввели, спробуйте ще раз!\n");
        }

        if (scheck == 0 && filename[0] != '\n')
        {
            printf("Ви не ввели розширення, спробуйте ще раз!\n");
        }
    } while (filename[0] == '\n' || fcheck == 0 || scheck == 0);
    filename[strlen(filename) - 1] = '\0'; //змінює \n на \0
}

/*Функція вiдкриття файлу та перевірки на помилки
Вхід:
Файловий дескриптор fp
Масив, у якому записана назва файлу, яку ввів користувач filename[]
Змінна для визначення того, у якому режимі користувач бажає відкрити файл
*/
void fopenCheck(FILE*& fp, char filename[], char d) {
    if (d == 'r') {
        if ((fp = fopen(filename, "r")) == NULL)
        {
            printf("Виникла помилка при відкритті файлу %s! \n", filename);
            exit(0);
        }
    }
    else if (d == 'w') {
        if ((fp = fopen(filename, "w")) == NULL)
        {
            printf("Виникла помилка при відкритті файлу %s! \n", filename);
            exit(0);
        }
    }
    else if (d == 'a') {
        if ((fp = fopen(filename, "a")) == NULL)
        {
            printf("Виникла помилка при відкритті файлу %s! \n", filename);
            exit(0);
        }
    }
}

/*
Функція виводу списку на екран
Вхід:
Дескриптор списку ld
*/
void printTheList(ListDescriptor* ld)
{
    if (!ld) /*стек не існує!*/
    {
        printf("Списку не існує!");
        return;
    }
    if (!ld->begin)
    {
        printf("Список пустий!\n");
        return;
    }
    else
    {
        int i = 0;
        printUtility(ld->begin, &i, ld->size); //вивід списку на екран
    }
}

/*
Функція виводу списку на екран
Вхід:
Адреса node на вузол списку
Адреса змінної i з номером поточного елементу
Змінна size з розміром списку
*/
void printUtility(ListElement* node, int* i, int size)
{
    if (*i < size)
    {
        if (node->data.birthDay < 10)
        {
            if (node->data.birthMonth < 10)
            {
                printf("Студент №%d:\nПрізвище-%s\nНомер групи-%d\nБал з математики-%d\nБал з української мови-%d\nБал з хімії-%d\nБал з біології-%d\nБал з фізкультури-%d\nДата народження-0%d.0%d.%d\n", *i + 1, node->data.surname, node->data.groupNum, node->data.mathMark, node->data.ukrainianMark, node->data.chemistryMark, node->data.biologyMark, node->data.PEMark, node->data.birthDay, node->data.birthMonth, node->data.birthYear);
            }
            else
            {
                printf("\nСтудент №%d:\nПрізвище-%s\nНомер групи-%d\nБал з математики-%d\nБал з української мови-%d\nБал з хімії-%d\nБал з біології-%d\nБал з фізкультури-%d\nДата народження-0%d.%d.%d\n", *i + 1, node->data.surname, node->data.groupNum, node->data.mathMark, node->data.ukrainianMark, node->data.chemistryMark, node->data.biologyMark, node->data.PEMark, node->data.birthDay, node->data.birthMonth, node->data.birthYear);
            }
        }
        else
        {
            if (node->data.birthMonth < 10)
            {
                printf("\nСтудент №%d:\nПрізвище-%s\nНомер групи-%d\nБал з математики-%d\nБал з української мови-%d\nБал з хімії-%d\nБал з біології-%d\nБал з фізкультури-%d\nДата народження-%d.0%d.%d\n", *i + 1, node->data.surname, node->data.groupNum, node->data.mathMark, node->data.ukrainianMark, node->data.chemistryMark, node->data.biologyMark, node->data.PEMark, node->data.birthDay, node->data.birthMonth, node->data.birthYear);
            }
            else
            {
                printf("\nСтудент №%d:\nПрізвище-%s\nНомер групи-%d\nБал з математики-%d\nБал з української мови-%d\nБал з хімії-%d\nБал з біології-%d\nБал з фізкультури-%d\nДата народження-%d.%d.%d\n", *i + 1, node->data.surname, node->data.groupNum, node->data.mathMark, node->data.ukrainianMark, node->data.chemistryMark, node->data.biologyMark, node->data.PEMark, node->data.birthDay, node->data.birthMonth, node->data.birthYear);
            }
        }
        *i = *i + 1;
        printUtility(node->next, i, size);
    }
    else
    {
        return;
    }
}

/*
Функція видалення вузла списку
Вхід:
Дескриптор списку ld
Номер елементу count, який потрібно видалити
*/
void deleteElement(ListDescriptor* ld, int count)
{
    ListElement* toDelete;
    int i = 0;
    if (!ld)
    {
        printf("Списку не існує!");
        return;
    }
    if (!ld->begin)
    {
        printf("Список пустий!\n");
        return;
    }
    ListElement* temp = searchPreviousElement(ld->begin, count, &i); //пошук попереднього елементу до того, який потрібно видалити
    toDelete = temp->next;
    temp->next = temp->next->next;
    temp->next->previous = temp;
    free(toDelete);
    ld->size--; /*зміна поточного розміру списку*/
}