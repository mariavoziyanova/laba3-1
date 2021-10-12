

#include <fcntl.h>

#include <chrono>
#include <complex>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <fstream>
#include <random>

#include "array_generator.h"
#include "arraysequence.h"
#include "common.hpp"
#include "isorter.h"
#include "linkedlistsequence.h"
#include "menu.h"

using namespace std::chrono;


template <class T>
Sequence<T> *generateArray() {}

template <int>
Sequence<int> *generateArray() {
  std::random_device rd;
  std::default_random_engine e1(rd());
  std::uniform_int_distribution<int> uniform_dist(-1000000, 1000000);

  wcout << L"Генерация массива из случайных целых чисел " << endl;
  Sequence<int> *sequence = new ArraySequence<int>();
  for (int i = 0; i < 100; i++) sequence->append(rand() % INT32_MAX);
}

Sequence<int> *generateSequence(Sequence<int> *a, int n){
  random_device rd;
  mt19937 mersenne(rd());
  for (int i = 0; i < n; ++i) {
    int value = int(mersenne())%1000;
    if (value % 2 == 0)
      value = -value;
    a->append(value);
  }
  return a;
}

template <typename T>
void TestSort(string sortName, Sequence<T> *sequence) {}

// Основные операции с выбранным типом данных
template <class T>
void main_menu() {
  Sequence<T> *sequence = generateArray<T>();
  TestSort("", sequence);

  // MenuItem menu[] = {
  //{L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
  //    {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
  //    {L"Конкатенация двух стеков", stack_concat<T>},
  //    {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
  //    {L"Поиск на вхождение подпоследовательности", stack_findSubSequence<T>},
  //    {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}
  //};
  // menuLoop(L"Возможные операции", _countof(menu), menu);
}

void graph() {
  //    {L"Применение функции map, where, reduce - ручной ввод данных", apply_map_where_reduce<T>},
  //    {L"Конкатенация двух стеков", stack_concat<T>},
  //    {L"Извлечение подпоследовательности (по заданным индексам)", stack_getSubSequence<T>},
  //    {L"Поиск на вхождение подпоследовательности", stack_findSubSequence<T>},
  //    {L"Сравнение времени добавления элементов в стек на основе LinkedList и DynamicArray", stack_addElementSpeed<T>}
  int n = 10000;

  /*
  // push
  ofstream push_list("push_list.txt", ios_base::trunc);
  Stack<int> push_stack = Stack<int>(new LinkedListSequence<int>());
  int elem = 10;
  int k = 1;
  for (int i = n / 100; i < n; i++) {
    auto start_push = chrono::steady_clock::now();
    push_stack.push(elem);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - start_push).count();
    if (i == k * n / 100) {
      k++;
      push_list << i << " " << time << "\n";
    }

  }
  push_list.close();

  ofstream push_array("push_array.txt", ios_base::trunc);
  Stack<int> push_stack_array = Stack<int>(new ArraySequence<int>());
  int elem_array = 10;
  int k1 = 1;
  for (int i = n / 100; i < n; i++) {
    auto start_push = chrono::steady_clock::now();
    push_stack.push(elem_array);
    auto endTime = chrono::steady_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(endTime - start_push).count();
    if (i == k1 * n / 100) {
      k1++;
      push_array << i << " " << time << "\n";
    }
  }
  push_array.close();
  */
}

inline int cmp(int a, int b) { return (a < b) ? -1 : (a == b) ? 0 : 1; }

wstring toWS(string s) { return wstring(s.begin(), s.end()); }
string toS(wstring s) { return string(s.begin(), s.end()); }

// MAX_SIZE - максимальное количество элементов в массиве
template <typename T>
void TestSortFunc(Sequence<T> *s, string sortName, ISorter<T> *sorter, string genName, string structName,
                  int MAX_SIZE) {
  string fileName = "../graphics/" + genName + "/" + sortName + "_" + structName + ".txt";
  wcout << toWS(fileName) << endl;
  ofstream file(fileName, ios_base::trunc);
  const int POINTS = 100;
  const int STEP = MAX_SIZE / POINTS;

  for (int n = STEP; n <= MAX_SIZE; n += STEP) {
    // Генерация последовательности
    Sequence<T> *seq = s->getSubsequence(0, n - 1);

    auto begin = steady_clock::now();
    sorter->Sort(seq, cmp);
    auto end = steady_clock::now();
    auto time = duration_cast<microseconds>(end - begin);

    // Проверяем что массив отсортирован
    /*
    for (int i = 0; i < seq->getLength() - 1; i++) {
      if ((*seq)[i] > (*seq)[i + 1]) {
        wprintf(L"Ошибка\n");
        string path = "../";
        path.append("Error.txt");
        ofstream err(path, ios_base::trunc);
        err << sortName << "\n";
        err << "i = " << i << ", (*seq)[i]=" << (*seq)[i] << ", (*seq)[i+1] = " << (*seq)[i + 1] << "\n";
        cout << "size = " << i << "\n";
        for (i = 0; i <= seq->getLength() - 1; i++) err << (*seq)[i] << " ";
        err.close();
        cout << sortName << " Error\n";
        return;
      }
    }
    */
    delete seq;
    file << n << " " << time.count() / 1e3 << "\n";
    // wcout << n << " " << time.count() / 1e3 << "\n";
  }

  file.close();
}

template <class S>
void Bench(IGenerator *gen, string structName, int MAX_SIZE) {
  Sequence<int> *seq = gen->generate(new S(), MAX_SIZE);
  TestSortFunc(seq, "BubbleSort", new BubbleSort<int>(), gen->name(), structName, MAX_SIZE / 50);
  TestSortFunc(seq, "ShellSort", new ShellSort<int>(), gen->name(), structName, MAX_SIZE / 50);
  TestSortFunc(seq, "ShellSort2", new ShellSort2<int>(), gen->name(), structName, MAX_SIZE / 50);
  TestSortFunc(seq, "ShakerSort", new ShakerSort<int>(), gen->name(), structName, MAX_SIZE / 50);
  TestSortFunc(seq, "InsertionSort", new InsertionSort<int>(), gen->name(), structName, MAX_SIZE / 50);
  if (gen->name() != "sorted" and gen->name() != "decreasing")
    TestSortFunc(seq, "QuickSort", new QuickSort<int>(), gen->name(), structName, MAX_SIZE);
  TestSortFunc(seq, "MergeSort", new MergeSort<int>(), gen->name(), structName, MAX_SIZE);
  TestSortFunc(seq, "HoarSort", new HoarSort<int>(), gen->name(), structName, MAX_SIZE);
  TestSortFunc(seq, "RandomizedQuickSort", new RandomizedQuickSort<int>(), gen->name(), structName, MAX_SIZE);
}

template <class S>
void benchAll(string structName) {
  IGenerator *gen = new RandomArrayGenerator();
  int MAX_SIZE = 100000;  // 500000;
  Bench<S>(gen, structName, MAX_SIZE);

  IGenerator *gen2 = new SortedArrayGenerator();
  Bench<S>(gen2, structName, MAX_SIZE);

  IGenerator *gen3 = new DecreasingArrayGenerator();
  Bench<S>(gen3, structName, MAX_SIZE);

  IGenerator *gen4 = new SwapArrayGenerator();
  Bench<S>(gen4, structName, MAX_SIZE);
}

// функция для демонстрации работы dynamic_cast<>()
template <class T>
void who_are_you(Sequence<T> *seq) {
  if (dynamic_cast<ArraySequence<T> *>(seq))
    wprintf(L"== Массив ==\n");
  else
    wprintf(L"== Лист ==\n");
}

// Основная программа
int main() {
  // Задаём кодировку UTF-16 для всего вывода в программе
  // Все символы и строки будут wchar_t
#if WIN32 || WIN64
  _setmode(_fileno(stdout), _O_U16TEXT);
  _setmode(_fileno(stdin), _O_U16TEXT);
  _setmode(_fileno(stderr), _O_U16TEXT);
#endif
  wprintf(L"== Тестирование операций ==\n");
  auto list = new LinkedListSequence<int>();
  auto array = new ArraySequence<int>();
  //  who_are_you(list);
  //  who_are_you(array);
//  benchAll<ArraySequence<int>>("array");
// benchAll<LinkedListSequence<int>>("list");

  //  MenuItem menu[] = {{L"Целые числа (int)", main_menu<int>},
  //                     {L"Вещественные числа (double)", main_menu<double>},
  //                     {L"Строки/символы (string)", main_menu<wstring>}};
  //  try {
  //    menuLoop(L"Выберите тип элементов", _countof(menu), menu);
  //  } catch (IndexOutOfRange &ex) {
  //    wcout << L"Exception: " << ex.what() << endl << endl;
  //  }
  //  graph();
  Sequence<int> *seq;
  while (true)
  {
    wprintf(L"Создать array(1) или создать list(2)\n");

    int c;
    wcin>>c;
    if (c < 1 || c > 2)
      wprintf(L"Нет такой операции\n");
    wprintf(L"Введите размер\n");
    int n;
    wcin>>n;
    wprintf(L"1 -заполнить случайными числами, 2 - ввод с клавиатуры\n");
    int h;
    wcin>>h;
    if (h< 1 ||  h> 2)
      wprintf(L"Нет такой операции\n");
    if(h==1)
    {
      if(c==1)
      {
        seq = generateSequence(new ArraySequence<int>, n);
        seq->print();
      }
      else {
        seq = generateSequence(new LinkedListSequence<int>, n);
        seq->print();
      }
    }else
    {
      int items[n];
      for(int i=0;i<n;i++)
        wcin>>items[i];
      if(c==1)
      {
        seq = new ArraySequence<int>(items, n);
        seq->print();
      }
      else
      {
        seq = new LinkedListSequence<int>(items,n);
        seq->print();
      }
    }
    wprintf(L"Выберите сортировку:\n1 - HoarSort\n2 - QuickSort\n3 - InsertSort\n4 - BubbleSort\n5 - ShellSort\n6 - ShellSort2\n7 - RandomizedQuickSort\n8 - ShakerSort\n9 - MergeSort\n");
    wcin>>c;
    if (c> 9 ||  c<=0)
      wprintf(L"Нет такой сортировки\n");
    switch (c)
    {
      case 1:
      {
        HoarSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 2:
      {
        QuickSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 3:
      {
        InsertionSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 4:
      {
        BubbleSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 5:
      {
        ShellSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 6:
      {
        ShellSort2<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 7:
      {
        RandomizedQuickSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 8:
      {
        ShakerSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }
      case 9:
      {
        MergeSort<int> sort;
        sort.Sort(seq, cmp)->print();
        break;
      }

      default:
        break;
    }
  }
}
