#pragma once

#include "sequence.h"

// Интерфейс для алгоритмов сортировки
template <class T>
struct ISorter {
  // a > b: +1
  // a == b: 0
  // a < b: -1
  virtual Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) = 0;
};

// 1. Метод пузырька - 3
template <class T>
struct BubbleSort : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    for (int i = 0; i < seq->getLength(); i++) {
      for (int j = i + 1; j < seq->getLength(); j++) {
        if (cmp((*seq)[j], (*seq)[i]) < 0) {
          auto temp = (*seq)[i];
          (*seq)[i] = (*seq)[j];
          (*seq)[j] = temp;
        }
      }
    }

    return seq;
  }
};

// 2. Модификация метода пузырька – шейкерная сортировка - 5
template <class T>
struct ShakerSort : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    int n = seq->getLength();
    int control = n - 1;
    int left = 0;
    int right = n - 1;

    do {
      for (int i = left; i < right; i++) {
        if (cmp((*seq)[i], (*seq)[i + 1]) > 0) {
          std::swap((*seq)[i], (*seq)[i + 1]);
          control = i;
        }
      }
      right = control;
      for (int i = right; i > left; i--) {
        if (cmp((*seq)[i], (*seq)[i - 1]) < 0) {
          std::swap((*seq)[i], (*seq)[i - 1]);
          control = i;
        }
      }
      left = control;
    } while (left < right);

    return seq;
  }
};

// 3. Метод простых вставок - 5 *
template <class T>
struct InsertionSort : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    int n = seq->getLength();
    for (int i = 0; i < n; i++) {
      int j = i;
      while (j > 0 && cmp((*seq)[j - 1], (*seq)[j]) > 0)  // a[j - 1] > a[j]
      {
        std::swap((*seq)[j], (*seq)[j - 1]);
        j--;
      }
    }

    return seq;
  }
};

// 4. Сортировка с помощью простого выбора - 5

// 5. Сортировка подсчетом - 7 *

// 6. Метод двоичных вставок - 7 *

// 7. Квадратичная сортировка (усов. сортировка выбором) - 9

// 8. Сортировка с помощью выбора из дерева - 7

// 9. Сортировка слиянием - 7 *
template <class T>
struct MergeSort : public ISorter<T> {
  // слияние двух отсортированных частей массива: [l,med]+[med+1,r]
  void merge(Sequence<T> &a, int l, int med, int r, int (*cmp)(T, T)) {
    int i = l, j = med + 1, size = 0;
    T ans[r - l + 1];
    while (i <= med && j <= r) {
      if (cmp(a[i], a[j]) < 0)
        ans[size++] = a[i++];
      else
        ans[size++] = a[j++];
    }

    while (i <= med) ans[size++] = a[i++];
    while (j <= r) ans[size++] = a[j++];

    if (size != r - l + 1) {
      cerr << "Error in function merge\n" << size << " != " << r - l + 1 << "\n";
      if (r - l + 1 < 20) {
        cout << "\na = {";
        for (i = l; i <= r; i++) cout << a[i] << ", ";
        cout << "}\nans ={";
        for (i = 0; i < size; i++) cout << ans[i] << ", ";
        cout << "}\n";
      }
      throw runtime_error("Error in function merge: size != r-l+1");
    }

    size = 0;
    for (i = l; i <= r; i++) a[i] = ans[size++];
  }

  void mergeSort(Sequence<T> &a, int l, int r, int (*cmp)(T, T)) {
    if (l >= r) return;
    int med = (l + r) / 2;
    mergeSort(a, l, med, cmp);
    mergeSort(a, med + 1, r, cmp);
    merge(a, l, med, r, cmp);
  }

  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    mergeSort((*seq), 0, seq->getLength() - 1, cmp);
    return seq;
  }
};

// 10. Пирамидальная сортировка - 10

// 11. Быстрая сортировка - 10 *
template <class T>
struct HoarSort : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    hoarSort(*seq, 0, seq->getLength() - 1, cmp);
    return seq;
  }

  void hoarSort(Sequence<T> &a, int first, int last, int (*cmp)(T, T)) {
    int i = first, j = last;
    T med = a[(first + last) / 2];

    while (i <= j) {
      while (cmp(a[i], med) < 0) ++i;
      while (cmp(a[j], med) > 0) --j;

      if (i <= j) {
        swap(a[i], a[j]);
        ++i;
        --j;
      }
    }

    if (first < j) hoarSort(a, first, j, cmp);
    if (i < last) hoarSort(a, i, last, cmp);
  }
};

template <class T>
struct QuickSort : public ISorter<T> {
  /* This function takes last element as pivot, places
     the pivot element at its correct position in sorted
      array, and places all smaller (smaller than pivot)
     to left of pivot and all greater elements to right
     of pivot */
  int partition(Sequence<T> &a, int low, int high, int (*cmp)(T, T)) {
    T pivot = a[high];  // Разделяющий элемент массива
    int i = low;  // Индекс наименьшего элемента (слева массива, с которым будет обмен)
    // Все элементы левее уже отсортированы
    for (int j = low; j <= high - 1; j++) {
      // if (a[j] <= pivot) // Если текущий элемент меньше либо равен разделяющему
      if (cmp(a[j], pivot) <= 0) {  // Вызов функции сравнения
        // swap(&a[i], &a[j]);
        std::swap(a[i], a[j]);
        i++;
      }
    }
    std::swap(a[i], a[high]);
    return i;
  }

  // Основная функция сортировки (рекурсивная реализация)
  // arr[] --> массив (последовательность) для сортировки
  // low   --> начиная с какого индекса сортировать
  // high  --> заканчивая каким индексом сортировать
  void quickSort(Sequence<T> &arr, int low, int high, int (*cmp)(T, T)) {
    if (low < high) {
      //  pi - индекс разделяющего элемента
      int pi = partition(arr, low, high, cmp);
      // Separately sort elements before
      // partition and after partition
      quickSort(arr, low, pi - 1, cmp);
      quickSort(arr, pi + 1, high, cmp);
    }
  }

  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    quickSort((*seq), 0, seq->getLength() - 1, cmp);
    return seq;
  }
};

template <class T>
struct RandomizedQuickSort : public ISorter<T> {
  /* This function takes last element as pivot, places
     the pivot element at its correct position in sorted
      array, and places all smaller (smaller than pivot)
     to left of pivot and all greater elements to right
     of pivot */
  int partition(Sequence<T> &arr, int low, int high, int (*cmp)(T, T)) {
    int pi = rand() % (high - low + 1) + low;  // Случайный индекс low..high
    std::swap(arr[pi], arr[high]);  // Меняем местами случайный элемент с крайним правым
    T pivot = arr[high];  // Разделяющий элемент массива
    int i = low;  // Индекс наименьшего элемента (слева массива, с которым будет обмен)
    // Все элементы левее уже отсортированы
    for (int j = low; j <= high - 1; j++) {
      // if (arr[j] <= pivot) // Если текущий элемент меньше либо равен разделяющему
      if (cmp(arr[j], pivot) <= 0) {  // Вызов функции сравнения
        // swap(&arr[i], &arr[j]);
        std::swap(arr[i], arr[j]);
        i++;
      }
    }
    std::swap(arr[i], arr[high]);
    return i;
  }

  // Основная функция сортировки (рекурсивная реализация)
  // arr[] --> массив (последовательность) для сортировки
  // low   --> начиная с какого индекса сортировать
  // high  --> заканчивая каким индексом сортировать
  void quickSort(Sequence<T> &arr, int low, int high, int (*cmp)(T, T)) {
    if (low < high) {
      //  pi - индекс разделяющего элемента
      int pi = partition(arr, low, high, cmp);
      // Separately sort elements before
      // partition and after partition
      quickSort(arr, low, pi - 1, cmp);
      quickSort(arr, pi + 1, high, cmp);
    }
  }

  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    quickSort((*seq), 0, seq->getLength() - 1, cmp);
    return seq;
  }
};

// 12. Сортировка Шелла - 10
template <class T>
struct ShellSort : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    int n = seq->getLength();
    for (int step = n / 2; step >= 1; step /= 2)
      for (int i = 0; i < n; i++)
        for (int j = i + step; j < n; j += step)
          if (cmp((*seq)[i], (*seq)[j]) > 0) std::swap((*seq)[i], (*seq)[j]);

    return seq;
  }
};

// 13. Сортировка Шелла (с выбором смещения) - 11
template <class T>
struct ShellSort2 : public ISorter<T> {
  Sequence<T> *Sort(Sequence<T> *seq, int (*cmp)(T, T)) override {
    int n = seq->getLength();
    int initialStep = n / 3;
    for (int step = initialStep; step >= 1; step /= 2)  // Смещение
      for (int i = 0; i < n; i++)
        for (int j = i + step; j < n; j += step)
          if (cmp((*seq)[i], (*seq)[j]) > 0) std::swap((*seq)[i], (*seq)[j]);

    return seq;
  }
};

// 14. Схема Бэтчера - 11
