
#include <iostream>
#include <limits>
#include <iomanip>
#include "Matrix.h"
#include "PerformanceCounter.h"

using namespace GEMM;
using namespace Perf;

template<typename T>
double data_type_benchmark()
{
  const auto  M_ROWS{ 512u };
  const auto  M_COLS{ 256u };
  const auto  N_ROWS{ M_COLS };
  const auto  N_COLS{ 1024u };
  const auto INTERATIONS{ 100u };

  static_assert(M_COLS == N_ROWS);
  const double MAC_OPS{ static_cast<double>(M_COLS * N_COLS * M_ROWS) };
  double operations_per_cycle{};

  for(auto i {0u}; i < INTERATIONS; ++i)
  {
    Matrix<T> M{ MatrixDimension {M_ROWS,M_COLS} };
    Matrix<T> N{ MatrixDimension {N_ROWS,N_COLS} };
    PerformanceCounter perf_counter{};
    perf_counter.start();
    auto P{ M * N };
    perf_counter.stop();

    operations_per_cycle += (static_cast<double>(perf_counter.clock_ticks()) / MAC_OPS);

    const auto P_dims{ P.get_dimension() };    
  }

  operations_per_cycle /= static_cast<double>(INTERATIONS);
  return (operations_per_cycle * 2.0);
}

int main()
{  
  const auto int_metric{ data_type_benchmark<int8_t>() };
  std::cout << "int8 compute: " << std::setprecision(2) 
    << int_metric << " MACS/cycle\n";

  const auto float_metric{ data_type_benchmark<float>() };
  std::cout << "float compute: " << std::setprecision(2) 
    << float_metric << " MACS/cycle\n\n";

  const auto speed_up{ int_metric / float_metric };

  std::cout << "int8 vs. float32 compute speed up: "
    << std::setprecision(2) << speed_up << "X\n";

  return 0;
}

