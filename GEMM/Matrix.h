#pragma once
#include <cstdint>
#include <vector>
#include <cassert>

namespace GEMM {
  struct MatrixDimension {
    uint32_t height;
    uint32_t width;

    uint32_t elements() const {
      return height * width;
    }
  };

  template<class T>
  class Matrix
  {
  public:
    Matrix<T>() = default;
    Matrix<T>(const MatrixDimension& dimension);
    Matrix<T> operator*(const Matrix<T>& rhs) const;
    MatrixDimension get_dimension() const;
  private:
    const T* get_data() const { return data_.data(); }
    T* get_mutable_data() { return data_.data(); }
    std::vector<T> data_;
    MatrixDimension dimension_{};
  };

  template<class T>
  Matrix<T>::Matrix(const MatrixDimension& dimension)
    : dimension_{ dimension }
  {
    data_.resize(dimension_.elements());
  }

  template<class T>
  MatrixDimension Matrix<T>::get_dimension() const {
    return dimension_;
  }

  template<class T>
  Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const {
    const MatrixDimension lhs_dims{ get_dimension() };
    const MatrixDimension rhs_dims{ rhs.get_dimension() };

    assert(lhs_dims.width == rhs_dims.height);

    MatrixDimension prod_dim{ lhs_dims.height,rhs_dims.width };

    Matrix<T> prod{ prod_dim };

    auto p_lhs_data{ get_data() };
    auto p_rhs_data{ rhs.get_data() };
    auto p_prod_data{ prod.get_mutable_data() };

    for (uint32_t col_out{}; col_out < prod_dim.width; ++col_out) {
      auto p_rhs_col{ p_rhs_data };
      p_rhs_col += (col_out * rhs_dims.height);

      for (uint32_t ri{}; ri < rhs_dims.height; ++ri) {
        const auto rin{ p_rhs_col[ri] };
        auto p_lhs_col{ p_lhs_data };
        p_lhs_col += (ri * lhs_dims.height);

        for (uint32_t li{}; li < lhs_dims.height; ++li) {
          p_prod_data[li] += rin * p_lhs_col[li];
        }
      }

      p_prod_data += prod_dim.height;
    }

    return prod;
  }
}