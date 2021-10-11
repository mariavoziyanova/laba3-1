#pragma once

#include <cassert>
#include <cwchar>
#include <iostream>

struct MenuItem {
  const wchar_t *name;

  void (*f)();
};

// Вывод меню и выбор из меню + вызов функций
// На входе:
//  title - заголовок меню
//  N - количество пунктов меню (вариантов для выбора)
//  ... (varargs) - пункты меню (массив строк)
// На выходе: выбор пользователя
void menuLoop(const wchar_t *title, int N, MenuItem menu[]) {
  assert(N >= 2);
  while (true) {
    wprintf(L"\n%ls (0 - выход):\n", title);
    // Печатаем меню
    for (int no = 1; no <= N; no++) {
      wprintf(L"  %d. %ls\n", no, menu[no - 1].name);
    }
    wprintf(L"Выберите (1..%d): ", N);
    int choice;  // Выбор пользователя
    wcin >> choice;
    if (choice >= 1 && choice <= N) {
      int index = choice - 1;
      std::wcout << L"Вы выбрали: " << menu[index].name << std::endl;
      menu[index].f();  // Вызываем функцию
      std::wcout << std::endl;
    } else {
      wprintf(L"Пункта меню %d нет => выход\n", choice);
      return;
    }
  }
}
