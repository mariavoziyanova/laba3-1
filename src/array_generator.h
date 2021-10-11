#pragma once

#include <random>

//#include "Sorts.h"
#include "arraysequence.h"
#include "sequence.h"


// Интерфейс генератора
class IGenerator {
 public:
  virtual string name() = 0;

  virtual Sequence<int> *generate(Sequence<int> *a, int n) = 0;
};

class RandomArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "random";
  }

  Sequence<int> *generate(Sequence<int> *a, int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    for (int i = 0; i < n; ++i) {
      int value = int(mersenne());
      if (value % 2 == 0) value = -value;
      a->append(value);
    }
    return a;
  }
};

class SortedArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "sorted";
  }

  Sequence<int> *generate(Sequence<int> *a, int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    int value = -int(mersenne());

    for (int i = 0; i < n; ++i) {
      a->append(value);
      value += int(mersenne()) % 100;
    }

    return a;
  }
};

class DecreasingArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "decreasing";
  }
  // Массив отсортированный в обратном порядке
  Sequence<int> *generate(Sequence<int> *a, int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    int value = int(mersenne());
    for (int i = 0; i < n; ++i) {
      a->append(value);
      value -= int(mersenne()) % 100;  // Каждый следующий элемент меньше предыдущего
    }

    return a;
  }
};

class SwapArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "swap";
  }

  Sequence<int> *generate(Sequence<int> *seq, int n) override {
    random_device rd;
    mt19937 mersenne(rd());

    IGenerator *gen = new SortedArrayGenerator();
    Sequence<int> *a = gen->generate(seq, n);
    int count_of_swap = n / 20 + mersenne() % (n / 20);

    for (int i = 0; i < count_of_swap; i++) {
      swap((*a)[mersenne() % (n / 2)], (*a)[n / 2 + mersenne() % (n / 2)]);
    }

    return a;
  }
};