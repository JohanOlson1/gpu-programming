#include "benchmarker.h"

#include <benchmark/benchmark.h>

BENCHMARK_REGISTER_F(Benchmarker, IppsCopy)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, IppsAdd)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, IppsMul)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, IppsDiv)->Unit(benchmark::kMicrosecond);

BENCHMARK_REGISTER_F(Benchmarker, NaiveMul)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, ParallelMul)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, SIMDParallelMul)->Unit(benchmark::kMicrosecond);

BENCHMARK_REGISTER_F(Benchmarker, TestGPU)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, NaiveMulGPU)->Unit(benchmark::kMicrosecond);

BENCHMARK_REGISTER_F(Benchmarker, CostlyOperation)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, CostlyOperationParallel)->Unit(benchmark::kMicrosecond);
BENCHMARK_REGISTER_F(Benchmarker, CostlyOperationGPU)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
