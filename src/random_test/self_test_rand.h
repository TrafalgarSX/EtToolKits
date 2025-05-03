/*
 * Copyright 2023 The Tongsuo Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/Tongsuo-Project/Tongsuo/blob/master/LICENSE.txt
 */

#include <cstdio>
#include <cstddef> // for std::size_t

template <typename T, std::size_t N>
constexpr std::size_t array_size(const T (&)[N]) noexcept {
    return N;
}
void setAlpha(double alphaValue);
double getAlpha();
double cephes_igamc(double a, double x);
int rand_self_test_frequency(const unsigned char *buf, size_t nbit, double *P_value, double *Q_value);
int rand_self_test_block_frequency(const unsigned char *buf,
                                   size_t len,
                                   size_t m,
                                   double *P_value, double *Q_value);
int rand_self_test_poker(const unsigned char *buf, size_t nbit, size_t m,
                         double *P_value, double *Q_value);
int rand_self_test_serial(const unsigned char *buf,
                          size_t nbit,
                          size_t m,
                          double *P1,
                          double *P2,
                          double *Q1,
                          double *Q2);
int rand_self_test_runs(const unsigned char *buf, size_t nbit, double *P, double *Q);
int rand_self_test_runs_distribution(const unsigned char *buf, size_t nbit, double *P, double *Q);
int rand_self_test_longest_run_of_ones(const unsigned char *buf, size_t nbit, bool checkone, double *P, double *Q);
int rand_self_test_binary_derivation(const unsigned char *buf, size_t nbit,
                                     size_t k, double *P, double *Q);
int rand_self_test_self_correlation(const unsigned char *buf, size_t nbit,
                                    size_t d, double *P, double *Q);
int rand_self_test_binary_matrix_rank(const unsigned char *buf, size_t nbit,
                                      double *P, double *Q);
int rand_self_test_cumulative_sums(const unsigned char *buf,
                                   size_t nbit,
                                   double *P,
                                   double *Q,
                                   bool forward);
int rand_self_test_approximate_entropy(const unsigned char *buf,
                                       size_t nbit,
                                       size_t m,
                                       double *P, double *Q);
int rand_self_test_linear_complexity(const unsigned char *buf, size_t nbit,
                                     size_t M, double *P, double *Q);
int rand_self_test_maurer_universal_statistical(const unsigned char *buf,
                                                size_t nbit, double *P, double *Q);
int rand_self_test_discrete_fourier_transform(const unsigned char *buf,
                                              size_t nbit, double *P, double *Q);
#if 0                                            
int rand_delivery_test(const std::string &rand_file);
int rand_poweron_test(const std::string &rand_file);
int rand_cyclical_test(const std::string &rand_file);
int rand_single_test(const std::string &rand_file);
#endif