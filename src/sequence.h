#pragma once
#include <vector>
template <class T>
class Sequence {
 public:
  // == Декомпозиция ==
  // Получить первый элемент в списке
  virtual T getFirst() const = 0;
  // Получить последний элемент в списке
  virtual T getLast() const = 0;

  // Получить элемент по индексу
  virtual T get(int index) const = 0;

  virtual T operator[](int i) const = 0;

  virtual T &operator[](int i) = 0;

  // Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
  virtual Sequence<T> *getSubsequence(int startIndex, int endIndex) const = 0;

  // Получить длину списка
  virtual int getLength() const = 0;

  // == Операции ==
  // Добавляет элемент в конец списка
  virtual void append(T item) = 0;

  // Добавляет элемент в начало списка
  virtual void prepend(T item) = 0;

  // Вставляет элемент в заданную позицию index
  virtual void insertAt(T item, int index) = 0;

  // Удаление элемента по индексу
  virtual void removeAt(int index) = 0;

  // Сцепляет два списка: к текущему списку присоединяем list
  virtual Sequence<T> *concat(Sequence<T> *list) = 0;

  // Вывод последовательности на печать
  virtual void print() = 0;

  // Поиск подпоследовательности
  // Возвращает стартовый индекс или -1 - если не найдена
  virtual int findSubsequence(Sequence<T> &subSequence) {
    int subSeqLen = subSequence.getLength();
    for (int start = 0; start <= getLength() - subSeqLen; start++) {
      bool match = true;
      for (int i = 0; i < subSeqLen; i++) {  // Проверяем подпоследовательность поэлементно
        if (get(start + i) != subSequence.get(i)) {
          match = false;
          break;
        }
      }
      if (match) {  // Нашли подпоследовательность
        return start;
      }
    }
    return -1;
  };

  // map - применение функции f(T x) к каждому элементу стека
  virtual Sequence<T> *map(T (*f)(T)) const = 0;

  // where фильтрует значения из списка l с помощью функции-фильтра h
  virtual Sequence<T> *where(bool (*h)(T)) const = 0;

  // reduce - применяем к каждой паре значений функцию f пока не получим одно значение
  virtual T reduce(T (*f)(T, T)) const = 0;

  virtual std::vector<T> asVector() const = 0;

  virtual void updateFromVector(const std::vector<T> &vec) = 0;

  // == Виртуальный деструктор ==
  virtual ~Sequence<T>() = default;
};
