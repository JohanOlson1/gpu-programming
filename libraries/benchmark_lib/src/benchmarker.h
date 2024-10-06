#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <benchmark/benchmark.h>
#include <ipp.h>
#include <omp.h>

#ifndef BENCHMARKER_H_
#define BENCHMARKER_H_

static void CheckAvailableGPU();
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

      std::cout << input[10] << ", " << input2[10] << std::endl;
    }

    ~Benchmarker() = default;

  protected:
    std::vector<float> input;
    std::vector<float> input2;
    std::vector<float> output;
    static constexpr unsigned N_elements_{10000000};
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

BENCHMARK_DEFINE_F(Benchmarker, NaiveMulGPU)(benchmark::State& state) {
  CheckAvailableGPU();
  for(auto _ : state) {
    float* in_data = input.data();
    float* in_data2 = input2.data();
    float* out_data = output.data();
    #pragma omp target teams distribute parallel for map(to:in_data[0:N_elements_], in_data2[0:N_elements_]) map(from:out_data[0:N_elements_])
    for(unsigned i = 0; i < N_elements_; ++i) {
      out_data[i] = in_data[i] * in_data2[i];
    }
  }
}

BENCHMARK_DEFINE_F(Benchmarker, CostlyOperation)(benchmark::State& state) {
  for(auto _ : state) {
    for(unsigned i = 0; i < N_elements_; ++i) {
      output[i] = (input[i] * input2[i]) - std::pow(input[i], 3.0f);
    }
  }
}

BENCHMARK_DEFINE_F(Benchmarker, CostlyOperationParallel)(benchmark::State& state) {
  for(auto _ : state) {
    #pragma omp parallel for simd num_threads(8)
    for(unsigned i = 0; i < N_elements_; ++i) {
      output[i] = (input[i] * input2[i]) - std::pow(input[i], 3.0f);
    }
  }
}

BENCHMARK_DEFINE_F(Benchmarker, CostlyOperationGPU)(benchmark::State& state) {
  CheckAvailableGPU();
  for(auto _ : state) {
    float* in_data = input.data();
    float* in_data2 = input2.data();
    float* out_data = output.data();
    //#pragma omp target teams distribute parallel for simd map(to:in_data[0:N_elements_], in_data2[0:N_elements_]) map(from:out_data[0:N_elements_])
    for(unsigned i = 0; i < N_elements_; ++i) {
      out_data[i] = (in_data[i] * in_data2[i]) - std::pow(in_data[i], 3.0f);
    }
  }
}

static void CheckAvailableGPU() {
  if(omp_get_num_devices() == 0) {
    std::cout << "GPU is not available" << std::endl;
  }
}

[[maybe_unused]] static void SetIppNumThreads(const int num_threads) {
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
