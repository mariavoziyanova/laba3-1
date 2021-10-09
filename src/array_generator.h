#pragma once

#include <random>

//#include "Sorts.h"
#include "arraysequence.h"
#include "sequence.h"

// Если нужны значения разных типов, то можем сделать:
// template <typename T>
// struct RandomValue {
//
//};

// Интерфейс генератора
class IGenerator {
 public:
  virtual string name() = 0;

  virtual Sequence<int> *generate(int n) = 0;
};

class RandomArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "random";
  }

  Sequence<int> *generate(int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    Sequence<int> *a = new ArraySequence<int>(n);

    for (int i = 0; i < n; ++i) {
      (*a)[i] = int(mersenne());
      if ((*a)[i] % 2 == 0) (*a)[i] = -(*a)[i];
    }

    return a;
  }
};

class SortedArrayGenerator : public IGenerator {
 public:
  string name() override {
    return "sorted";
  }

  Sequence<int> *generate(int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    int value = -int(mersenne());

    Sequence<int> *a = new ArraySequence<int>(n);
    for (int i = 0; i < n; ++i) {
      (*a)[i] = value;
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
  Sequence<int> *generate(int n) override {
    random_device rd;
    mt19937 mersenne(rd());
    int value = int(mersenne());
    Sequence<int> *a = new ArraySequence<int>(n);

    for (int i = 0; i < n; ++i) {
      (*a)[i] = value;
      value -= int(mersenne()) % 100;  // Каждый следующий элемент меньше предыдущего
    }

    return a;
  }
};

class SwapArrayGenerator: public IGenerator
{
 public:
  string name() override{
      return "swap";
  }

  Sequence<int> *generate(int n) override
  {
    random_device rd;
    mt19937 mersenne(rd());

    IGenerator *gen = new SortedArrayGenerator();
    Sequence<int> *a = gen->generate(n);
    int count_of_swap = n / 20 + mersenne() % (n / 20);

    for (int i = 0; i < count_of_swap; i++)
    {
      swap((*a)[mersenne() % (n / 2)], (*a)[n / 2 + mersenne() % (n / 2)]);
    }

    return a;
  }
};