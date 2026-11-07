# password-gen

Мини-утилита на C++17 для генерации паролей с оценкой энтропии
и проверкой на слабые паттерны.

## Сборка

```bash
cmake -S . -B build
cmake --build build

## Использование
./build/password-gen                 # пароль 16 символов (буквы + цифры)
./build/password-gen -l 24 -S        # 24 символа + спецсимволы
./build/password-gen -l 20 -a -S     # без неоднозначных символов