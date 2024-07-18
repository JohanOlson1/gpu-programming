#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <benchmark/benchmark.h>
#include <ipps.h>
#include <ippcore.h>
#include <omp.h>

#ifndef BENCHMARKER_H_
#define BENCHMARKER_H_

static void SetIppNumThreads(const int num_threads);

class Benchmarker : public benchmark::Fixture {
  public:
    Benchmarker() {
      input.reserve(N_elements_);
      input2.reserve(N_elements_);
      output.reserve(N_elements_);
      for(unsigned i = 0; i < N_elements_; ++i) {
        input[i] = {static_cast<float>(std::rand() % 100)};
        input2[i] = {static_cast<float>(std::rand() % 100)};
      }

      SetIppNumThreads(4);
    }

    ~Benchmarker() = default;

  protected:
    std::vector<float> input;
    std::vector<float> input2;
    std::vector<float> output;
    static constexpr unsigned N_elements_{500000};
};

BENCHMARK_DEFINE_F(Benchmarker, IppsCopy)(benchmark::State& state) {
  for(auto _ : state) {
    ippsCopy_32f(input.data(), output.data(), N_elements_);
  }
}

BENCHMARK_DEFINE_F(Benchmarker, IppsAdd)(benchmark::State& state) {
  for(auto _ : state) {
    ippsAdd_32f(input.data(), input2.data(), output.data(), N_elements_);
  }
}

BENCHMARK_DEFINE_F(Benchmarker, IppsMul)(benchmark::State& state) {
  for(auto _ : state) {
    ippsMul_32f(input.data(), input2.data(), output.data(), N_elements_);
  }
}

BENCHMARK_DEFINE_F(Benchmarker, IppsDiv)(benchmark::State& state) {
  for(auto _ : state) {
    ippsDiv_32f(input.data(), input2.data(), output.data(), N_elements_);
  }
}

BENCHMARK_DEFINE_F(Benchmarker, NaiveMul)(benchmark::State& state) {
  for(auto _ : state) {
    for(unsigned i = 0; i < N_elements_; ++i) {
      output[i] = input[i] * input2[i];
    }
  }
}

BENCHMARK_DEFINE_F(Benchmarker, ParallelMul)(benchmark::State& state) {
  for(auto _ : state) {
    #pragma omp parallel for num_threads(4)
    for(unsigned i = 0; i < N_elements_; ++i) {
      output[i] = input[i] * input2[i];
    }
  }
}

BENCHMARK_DEFINE_F(Benchmarker, SIMDParallelMul)(benchmark::State& state) {
  for(auto _ : state) {
    #pragma omp parallel for simd num_threads(4)
    for(unsigned i = 0; i < N_elements_; ++i) {
      output[i] = input[i] * input2[i];
    }
  }
}

// BENCHMARK_DEFINE_F(Benchmarker, SIMDParallelMul)(benchmark::State& state) {
//   for(auto _ : state) {
//     #pragma omp target teams distribute parallel
//     for(unsigned i = 0; i < N_elements_; ++i) {
//       output[i] = input[i] * input2[i];
//     }
//   }
// }

static void SetIppNumThreads(const int num_threads) {
    IppStatus status{ippSetNumThreads(num_threads)};

    if(status == ippStsNoErr) {
        std::cout << "set number of ipp threads to: " << num_threads << std::endl;
    } else if(status == ippStsNoOperation) {
        std::cout << "Failed to set number of threads" << std::endl;
    } else {
        std::cout << "Error setting threads" << std::endl;
    }

    int num_threads_after{0};
    ippGetNumThreads(&num_threads_after);
    std::cout << "Number of threads used by ipp: " << num_threads_after << std::endl;
}

#endif // BENCHMARKER_H_
