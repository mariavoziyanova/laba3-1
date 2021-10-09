#include <chrono>
#include <complex>
#include <cstdlib>

#include "array_generator.h"
#include "arraysequence.h"
#include "gtest/gtest.h"
#include "isorter.h"
#include "sequence.h"

// Базовые операции с динамическим массивом
TEST(DynamicArray, basic_operations) {
  // Создаём массив на 3 элемента
  DynamicArray<int> da(3);
  ASSERT_EQ(3, da.getSize());  // Проверяем что размер 3
  try {
    da.get(0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 0 not defined", ex.what());
  }
  try {
    da.get(2);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 2 not defined", ex.what());
  }
  // Задаём значение всех элементов
  da.set(0, 11);
  da.set(1, 22);
  da.set(2, 33);
  // Проверяем значение всех элементов массива
  ASSERT_EQ(11, da.get(0));
  ASSERT_EQ(22, da.get(1));
  ASSERT_EQ(33, da.get(2));
  // Обращение по индексу через оператор
  ASSERT_EQ(11, da[0]);
  ASSERT_EQ(22, da[1]);
  ASSERT_EQ(33, da[2]);
  // Проверяем что на отрицательный индекс генерируется исключение для операции get
  try {
    da.get(-1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..2", ex.what());
  }
  // Проверяем что на отрицательный индекс генерируется исключение для операции set
  try {
    da.set(-1, 122);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..2", ex.what());
  }
  // Слишком большой индекс - за границами массива => тоже исключение
  try {
    da.get(10);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 10 out of range 0..2", ex.what());
  }
  try {
    int x = da[11];
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 11 out of range 0..2", ex.what());
  }
  // Меняем значения по индексу
  da[0] = 101;
  ASSERT_EQ(3, da.getSize());
  ASSERT_EQ(101, da[0]);
  ASSERT_EQ(22, da[1]);
  ASSERT_EQ(33, da[2]);
  // Вставка элемента
  da.insertAt(1, 0);
  ASSERT_EQ(4, da.getSize());
  ASSERT_EQ(1, da[0]);
  ASSERT_EQ(101, da[1]);
  ASSERT_EQ(22, da[2]);
  ASSERT_EQ(33, da[3]);
  // Перегрузка операторов
  da[0] = 2 * da[1] + da[2];             // Более наглядна
  da.set(0, 2 * da.get(1) + da.get(2));  // чем такая
}

TEST(DynamicArray, undefined_elements) {
  DynamicArray<int> da(4);
  try {
    int x = da.get(0);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 0 not defined", ex.what());
  }
  try {
    int x = da.get(2);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Element with index 2 not defined", ex.what());
  }
  try {
    int x = da.get(-1);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index -1 out of range 0..3", ex.what());
  }
  try {
    int x = da.get(11);
    FAIL() << "Should be IndexOutOfRange";
  } catch (IndexOutOfRange &ex) {
    ASSERT_STREQ("Index 11 out of range 0..3", ex.what());
  }
  da[1] = 20;
  da.print();
}

TEST(DynamicArray, initializer_list) {
  DynamicArray<int> da{6, 32, 87};
  ASSERT_EQ(3, da.getSize());
  ASSERT_EQ(6, da[0]);
  ASSERT_EQ(32, da[1]);
  ASSERT_EQ(87, da[2]);
}

TEST(DynamicArray, swap) {
  int data[] = {11, 22, 33};
  DynamicArray<int> da(data, _countof(data));
  // Меняем местами 2 элемента
  da.swap(0, 1);
  ASSERT_EQ(22, da[0]);
  ASSERT_EQ(11, da[1]);
  ASSERT_EQ(33, da[2]);
}

TEST(DynamicArray, removeAt) {
  int data[] = {11, 22, 33};
  DynamicArray<int> da(data, _countof(data));
  da.removeAt(1);
  ASSERT_EQ(2, da.getSize());
  ASSERT_EQ(11, da[0]);
  ASSERT_EQ(33, da[1]);
  da.removeAt(1);
  ASSERT_EQ(1, da.getSize());
  ASSERT_EQ(11, da[0]);
  da.removeAt(0);
  ASSERT_EQ(0, da.getSize());
}

TEST(DynamicArray, resize) {
  DynamicArray<int> da(0);
  ASSERT_EQ(0, da.getSize());  // Проверяем что размер 0
  for (int size = 1; size <= 10; size++) {
    da.resize(size);
    ASSERT_EQ(size, da.getSize());  // Проверяем размер
  }
  da[4] = 4;
  da[5] = 5;
  da[9] = 9;
  da.print();
}

TEST(DynamicArray, array_initialization) {
  // Инициализация динамического массива из массива значений
  int data[] = {111, 222};
  DynamicArray<int> da(data, _countof(data));
  ASSERT_EQ(_countof(data), da.getSize());
  da.set(0, 111);
  da.set(1, 222);
  ASSERT_EQ(111, da.get(0));
  ASSERT_EQ(222, da.get(1));

  // Копирующий конструктор
  DynamicArray<int> d2(da);
  ASSERT_EQ(da.getSize(), d2.getSize());
  ASSERT_EQ(111, d2.get(0));
  ASSERT_EQ(222, d2.get(1));

  // Уменьшаем размер
  d2.resize(1);
  ASSERT_EQ(1, d2.getSize());
  ASSERT_EQ(111, d2.get(0));

  // Увеличиваем размер
  d2.resize(3);
  ASSERT_EQ(3, d2.getSize());
  ASSERT_EQ(111, d2.get(0));
  d2.set(1, 221);
  d2.set(2, 331);
  ASSERT_EQ(221, d2.get(1));
  ASSERT_EQ(331, d2.get(2));
}

TEST(ArraySequence, constructor) {
  Sequence<int> *s = new ArraySequence<int>();
  ASSERT_EQ(0, s->getLength());
  for (int x = 1; x <= 10; x++) {
    s->append(x);
    ASSERT_EQ(x, s->getLength());
    ASSERT_EQ(x, s->getLast());
  }
  delete s;
}

TEST(ArraySequence, ArraySequence_Append_Prepend_element) {
  int data[] = {111, 222};
  Sequence<int> *s = new ArraySequence<int>(data, _countof(data));  // Список целых чисел
  ASSERT_EQ(2, s->getLength());
  ASSERT_EQ(111, s->get(0));
  ASSERT_EQ(222, s->get(1));
  ASSERT_EQ(111, s->getFirst());
  ASSERT_EQ(222, s->getLast());
  s->append(333);
  ASSERT_EQ(3, s->getLength());
  ASSERT_EQ(333, s->get(2));
  s->prepend(10);
  ASSERT_EQ(4, s->getLength());
  ASSERT_EQ(10, s->get(0));
  ASSERT_EQ(111, s->get(1));
  ASSERT_EQ(222, s->get(2));
  ASSERT_EQ(333, s->get(3));
  delete s;
}

// Создание второго динамического массива как копии первого
TEST(ArraySequence, create_from_DynamicArray) {
  int data[] = {123, 234, 345};
  DynamicArray<int> da(data, _countof(data));     // Список целых чисел
  Sequence<int> *s = new ArraySequence<int>(da);  // Список целых чисел
  ASSERT_EQ(3, s->getLength());
  ASSERT_EQ(data[0], s->get(0));
  ASSERT_EQ(data[1], s->get(1));
  ASSERT_EQ(data[2], s->get(2));
  ASSERT_EQ(data[0], s->getFirst());
  ASSERT_EQ(data[2], s->getLast());
  s->append(333);
  ASSERT_EQ(4, s->getLength());
  ASSERT_EQ(333, s->getLast());
  s->prepend(10);
  ASSERT_EQ(5, s->getLength());
  // 123, 234, 345
  ASSERT_EQ(10, s->get(0));
  ASSERT_EQ(data[0], s->get(1));
  ASSERT_EQ(data[1], s->get(2));
  ASSERT_EQ(data[2], s->get(3));
  ASSERT_EQ(333, s->get(4));
  Sequence<int> *sub = s->getSubsequence(1, 3);
  ASSERT_EQ(3, sub->getLength());
  ASSERT_EQ(123, sub->get(0));
  ASSERT_EQ(234, sub->get(1));
  ASSERT_EQ(345, sub->get(2));
  // Вставим ещё один элемент посередине
  sub->insertAt(4444, 1);
  ASSERT_EQ(4, sub->getLength());
  ASSERT_EQ(123, sub->get(0));
  ASSERT_EQ(4444, sub->get(1));
  ASSERT_EQ(234, sub->get(2));
  ASSERT_EQ(345, sub->get(3));
  delete s;
  delete sub;
}

// Сцепление двух списков
TEST(ArraySequence, concat) {
  // Первая последовательность
  int data1[] = {11, 22};
  DynamicArray<int> da1(data1, _countof(data1));    // Список целых чисел
  Sequence<int> *s1 = new ArraySequence<int>(da1);  // Список целых чисел
  // Вторая последовательность
  int data2[] = {33};
  DynamicArray<int> da2(data2, _countof(data2));    // Список целых чисел
  Sequence<int> *s2 = new ArraySequence<int>(da2);  // Список целых чисел
  // Сцепляем два списка
  Sequence<int> *res = s1->concat(s2);
  ASSERT_EQ(3, res->getLength());
  ASSERT_EQ(11, res->get(0));
  ASSERT_EQ(22, res->get(1));
  ASSERT_EQ(33, res->get(2));
  delete s1;
  delete s2;
  delete res;
}

TEST(ArraySequence, findSubsequence) {
  int data[] = {11, 22, 33, 44, 55};
  DynamicArray<int> da(data, _countof(data));
  Sequence<int> *main = new ArraySequence<int>(da);

  {
    int d[] = {11, 22};
    Sequence<int> *sub1 = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(0, main->findSubsequence(*sub1));
    delete sub1;
  }
  {
    int d[] = {22, 33};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(1, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(2, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {44, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(3, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(4, main->findSubsequence(*sub));
    delete sub;
  }
  {
    int d[] = {33, 55};
    Sequence<int> *sub = new ArraySequence<int>(DynamicArray<int>(d, _countof(d)));
    ASSERT_EQ(-1, main->findSubsequence(*sub));
    delete sub;
  }

  delete main;
}

// Функция сравнения элементов a и b
// a < b: -1
// a == b: 0
// a > b: +1
int cmp(int a, int b) {
  return (a < b) ? -1 : (a == b) ? 0 : 1;
}

// Проверка что Sequence отсортирован
void checkSorted(Sequence<int> *a) {
  // i меняется в диапазоне 1..n-1
  // сравниваем элементы a[i-1] и a[i]
  // И проверяем что элементы не убывают: a[i-1] <= a[i]
  for (int i = 1; i < a->getLength(); i++) {
    ASSERT_LE((*a)[i - 1], (*a)[i]);
  }
}

TEST(BubbleSort, sortSmall) {
  ASSERT_EQ(-1, cmp(1, 2));
  ASSERT_EQ(0, cmp(2, 2));
  ASSERT_EQ(1, cmp(2, 1));

  DynamicArray<int> da{11, 22, 66, 99, 55};
  Sequence<int> *main = new ArraySequence<int>(da);

  ISorter<int> *sorter = new BubbleSort<int>();
  Sequence<int> *sorted = sorter->Sort(main, cmp);
  checkSorted(sorted);
  sorted->print();
  // delete sorted;
  // delete sorter;

  // delete main;
}

TEST(ShakerSort, sortSmall) {
  ASSERT_EQ(-1, cmp(1, 2));
  ASSERT_EQ(0, cmp(2, 2));
  ASSERT_EQ(1, cmp(2, 1));

  DynamicArray<int> da{11, 22, 66, 99, 55};
  Sequence<int> *main = new ArraySequence<int>(da);

  ISorter<int> *sorter = new ShakerSort<int>();
  Sequence<int> *result = sorter->Sort(main, cmp);
  checkSorted(result);
  result->print();
  // delete result;
  // delete sorter;

  // delete main;
}

TEST(BubbleSort, bigArray) {
  RandomArrayGenerator gen;
  Sequence<int> *main = gen.generate(1000);

  ISorter<int> *sorter = new BubbleSort<int>();
  Sequence<int> *result = sorter->Sort(main, cmp);
  checkSorted(result);
  result->print();
  // delete result;
  // delete sorter;

  // delete main;
}

TEST(MergeSort, bigArray) {
  RandomArrayGenerator gen;
  Sequence<int> *array = gen.generate(10000);

  ISorter<int> *sorter = new MergeSort<int>();
  Sequence<int> *result = sorter->Sort(array, cmp);
  checkSorted(result);
  // result->print();
  //  delete result;
  //  delete sorter;

  // delete array;
}

TEST(QuickSort, bigArray) {
  RandomArrayGenerator gen;
  Sequence<int> *array = gen.generate(10000);

  ISorter<int> *sorter = new QuickSort<int>();
  Sequence<int> *result = sorter->Sort(array, cmp);
  checkSorted(result);
  // result->print();
  //  delete result;
  //  delete sorter;

  // delete array;
}

TEST(RandomizedQuickSort, bigArray) {
  RandomArrayGenerator gen;
  Sequence<int> *array = gen.generate(10000);

  ISorter<int> *sorter = new RandomizedQuickSort<int>();
  Sequence<int> *result = sorter->Sort(array, cmp);
  checkSorted(result);
  // result->print();
  //  delete result;
  //  delete sorter;

  // delete array;
}

TEST(ShellSort, bigArray) {
  RandomArrayGenerator gen;
  Sequence<int> *array = gen.generate(1000);

  ISorter<int> *sorter = new ShellSort<int>();
  Sequence<int> *result = sorter->Sort(array, cmp);
  checkSorted(result);
  result->print();
  // delete result;
  // delete sorter;

  // delete array;
}
