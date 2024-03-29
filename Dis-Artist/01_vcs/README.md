# Информатика 2023 🔥 | Система контроля версий

Система контроля версий — это программа, позволяющая работать с несколькими версиями одного и того же файла. Известной системой контроля версий является git. Этот прототип написан ChatGPT. Ваша цель — довести прототип до рабочего состояния. В коде могут быть ошибки, их надо исправить. Программа должна создавать репозиторий в рабочем каталоге и предоставлять базовые функции системы контроля версий. Вы должны выбрать одну из функций из списка задач ниже и добавить её в код проетка. Вы также можете предложить свою функцию.

## Информация
- [Задачи](#задачи)
- [Библиотеки](#библиотеки)
- [Сборка](#сборка)
- [Использование](#использование)
- [Тесты](#тесты)

## Задачи<a name = "задачи"></a>

- [x] Написать заготовку приложения 🤓
- [ ] Реализовать возможность откатывать изменения 🔵
- [x] Реализовать вывод состояния репозитория 🔵
- [ ] Реализовать сравнение двух состояний репозитория 💀
- [ ] Реализовать слияние двух состояний репозитория 💀
- [x] Реализовать вывод справки (ключ `-h`) 🟢
- [ ] Реализовать аналог механизма `stage` у git 🔵
- [ ] Реализовать вывод истории изменений файла 🔵
- [ ] Реализовать синхронизацию с другим репозиторием (другой каталог) 💀
- [ ] Реализовать синхронизацию с другим репозиторием (по сети) 💀
- [ ] Реализовать поиск по состояниям 🔵
- [ ] Реализовать механизм ветвления 🔵
- [ ] Реализовать метки (теги) 🟢
- [ ] Реализовать поддержку нескольких пользователей 🟢
- [x] Добавить тесты 🔵
- [ ] Предложить свою функцию

## Используемые библиотеки и зависимости<a name = "библиотеки"></a>

- Компилятор - `g++-9`
- `Sqlite3`

## Как собрать и запустить проект<a name = "сборка"></a>

Запустить виртуальную машину linux

Клонировать проект:

```console
git clone https://github.com/bogdanova21/2023-spring-computer-science.git
```
В консоли ввести следующее:
```console
cd 2023-spring-computer-science/Dis-Artist/01_vcs/  
g++ -std=c++17 cli.cpp vcs.cpp vcs.h cli.h main.cpp -lsqlite3 -o main.out
```
## Использование<a name = "использование"></a>
Запускайте команды, как обычно с git.
```console
./main.out < status / commit <message> / push(requires server to be running) / log >
```
## Тесты 1<a name = "тесты"></a>


:white_check_mark: Функция --help

:white_check_mark: Функция status, commit, push

```
(Проверка происходила при удалении файла txt и папки. Все изменения сохраняются в нашей базе данных SQLite) 
```
![Screenshot](https://github.com/bogdanova21/2023-spring-computer-science/blob/main/Dis-Artist/01_vcs/tests_program/Screenshot_1.png)


## Тест 2

:white_check_mark: Функция log
```
(Каждое изменение фиксируется и добавляется в БД. При выведении логов правильно выводится время и дата изменения)
```
![Screenshot](https://github.com//bogdanova21/2023-spring-computer-science/blob/main/Dis-Artist/01_vcs/tests_program/Screenshot_2.png)

