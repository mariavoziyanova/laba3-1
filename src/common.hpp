#pragma once


#include <string>

using namespace std;


#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#endif



// Выход индекса за допустимые границы: отрицательный, >= размера массива
// Индекс должен быть в пределах: 0..количество_элементов-1
class IndexOutOfRange : public std::exception {  // наследуем наш класс-исключение от общего
  std::string message_;                          // Детали об ошибке в читаемом формате

 public:
  explicit IndexOutOfRange(const std::string &message) : message_(message) {}

  explicit IndexOutOfRange(const char *message) : message_(message) {}

  const char *what() const noexcept override {
    return message_.c_str();
  }
};
