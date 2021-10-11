#pragma once

#include "linkedlist.h"
#include "sequence.h"

template <class T>
class LinkedListSequence : public Sequence<T> {
  LinkedList<T> data;  // Массив данных
  void checkIndex(int index) const {
    if (index < 0)
      throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
    if (index >= getLength())
      throw IndexOutOfRange(string("Index ") + to_string(index) + " out of range 0.." + to_string(getLength() - 1));
  }

 public:
  // Копировать элементы из переданного массива
  LinkedListSequence(T *items, int count) : data(items, count) {}
  // Создать пустой список
  LinkedListSequence() : data(){};
  // Копирующий конструктор
  explicit LinkedListSequence(const LinkedList<T> &list) : data(list) {};
  // == Декомпозиция ==
  // Получить первый элемент в списке
  T getFirst() const override { return data.getFirst(); };
  // Получить последний элемент в списке
  T getLast() const override { return data.getLast(); };
  // Получить элемент по индексу
  T get(int index) const override { return data.get(index); };
  // Перегруженные операторы
  T operator[](int i) const { return data[i]; }

  T &operator[](int i) { return data[i]; };
  // Получить список из всех элементов, начиная с startIndex и заканчивая endIndex
  Sequence<T> *getSubsequence(int startIndex, int endIndex) const override {
    checkIndex(startIndex);
    checkIndex(endIndex);
//    auto *res = new LinkedListSequence<T>();
//    for (int index = startIndex; index <= endIndex; index++) {
//      res->append(data.get(index));
//    }
    LinkedList<T> res = data.getSubList(startIndex,endIndex);
    return new LinkedListSequence<T>(res);
  }

  // Получить длину списка
  int getLength() const override { return data.getLength(); }

  // == Операции ==
  // Добавляет элемент в конец списка
  void append(T item) override { data.append(item); };
  // Добавляет элемент в начало списка
  void prepend(T item) override { data.prepend(item); };
  // Вставляет элемент в заданную позицию
  void insertAt(T item, int index) override { data.insertAt(item, index); };
  // Сцепляет два списка
  Sequence<T> *concat(Sequence<T> *list) override {
    Sequence<T> *res = new LinkedListSequence<T>(*this);
    for (int i = 0; i < list->getLength(); i++) {
      res->append(list->get(i));
    }
    return res;
  };
  // Удаление элемента по индексу
  void removeAt(int index) override { data.removeAt(index); }
  // Печать на экран всех элементов
  void print() override { data.print(); }
  // == Деструктор ==
  virtual ~LinkedListSequence() = default;
  Sequence<T> *map(T (*f)(T)) const override {
    auto *res = new LinkedListSequence<T>();
    for (int i = 0; i < getLength(); i++) {
      res->append(f(data.get(i)));
    }
    return res;
  }
  Sequence<T> *where(bool (*h)(T)) const override {
    auto *res = new LinkedListSequence<T>;
    for (int i = 0; i < getLength(); i++) {
      T item = data.get(i);
      if (h(item)) {
        res->append(item);
      }
    }
    return res;
  }
  T reduce(T (*f)(T, T)) const override {
    T result = data.getFirst();
    for (int i = 1; i < data.getLength(); i++) {
      result = f(result, data.get(i));
    }
    return result;
  }

  vector<T> asVector() const override { return data.asVector(); }

  void updateFromVector(const vector<T> &vec) override { data.updateFromVector(vec); }
};
