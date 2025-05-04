#include <gtest/gtest.h>
#include <random_test/self_test_rand.h>

#include <array>
#include <cstdint>
#include <iostream>

#include "self_random_test.h"


static std::vector<uint8_t> bits2buf(const char* bits)
{
    size_t i;
    size_t nbit = strlen(bits);
    size_t len = (nbit + 7) / 8;
    std::vector<uint8_t> buf(len, 0);

    for (i = 0; i < nbit; i++) buf[i / 8] |= (bits[i] - '0') << (7 - i % 8);

    return buf;
}

TEST(RandomTest, RandomTestAlg)
{
    std::vector<uint8_t> bits128 = bits2buf(
        "11001100000101010110110001001100111000000000001001001101010100010001001111010110100000001101011111001100111001"
        "101101100010110010");
    std::vector<uint8_t> bits100 = bits2buf(
        "1100100100001111110110101010001000100001011010001100001000110100110001001100011001100010100010111000");

    double actual_P = 0.0;
    double actual_P1 = 0.0;
    double actual_P2 = 0.0;
    double actual_Q = 0.0;
    double actual_Q1 = 0.0;
    double actual_Q2 = 0.0;

    EXPECT_EQ(sizeof(million_bits_of_e), 1000000 / 8);

    EXPECT_TRUE(rand_self_test_frequency(bits128.data(), 128, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.215925, 1e-6);
    EXPECT_NEAR(actual_Q, 0.892038, 1e-6);

    EXPECT_TRUE(rand_self_test_block_frequency(bits100.data(), 100, 10, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.706438, 1e-6);
    EXPECT_NEAR(actual_Q, 0.706438, 1e-6);

    EXPECT_TRUE(rand_self_test_poker(bits128.data(), 128, 4, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.213734, 1e-6);
    EXPECT_NEAR(actual_Q, 0.213734, 1e-6);

    EXPECT_TRUE(rand_self_test_serial(bits128.data(), 128, 2, &actual_P1, &actual_P2, &actual_Q1, &actual_Q2));
    EXPECT_NEAR(actual_P1, 0.436868, 1e-6);
    EXPECT_NEAR(actual_P2, 0.723674, 1e-6);
    EXPECT_NEAR(actual_Q1, 0.436868, 1e-6);
    EXPECT_NEAR(actual_Q2, 0.723674, 1e-6);

    EXPECT_TRUE(rand_self_test_runs(bits128.data(), 128, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.620729, 1e-6);
    EXPECT_NEAR(actual_Q, 0.310364, 1e-6);

    EXPECT_TRUE(rand_self_test_runs_distribution(bits128.data(), 128, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.970152, 1e-6);
    EXPECT_NEAR(actual_Q, 0.970152, 1e-6);

    EXPECT_TRUE(rand_self_test_longest_run_of_ones(bits128.data(), 128, true, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.180598, 1e-6);
    EXPECT_NEAR(actual_Q, 0.180598, 1e-6);

    EXPECT_TRUE(rand_self_test_longest_run_of_ones(bits128.data(), 128, false, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.839299, 1e-6);
    EXPECT_NEAR(actual_Q, 0.839299, 1e-6);

    EXPECT_TRUE(rand_self_test_binary_derivation(bits128.data(), 128, 3, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.039669, 1e-6);
    EXPECT_NEAR(actual_Q, 0.980166, 1e-6);

    EXPECT_TRUE(rand_self_test_self_correlation(bits128.data(), 128, 1, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.790080, 1e-6);
    EXPECT_NEAR(actual_Q, 0.395040, 1e-6);

    EXPECT_TRUE(rand_self_test_binary_matrix_rank(million_bits_of_e, 1000000, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.307543, 1e-6);
    EXPECT_NEAR(actual_Q, 0.307543, 1e-6);

    EXPECT_TRUE(rand_self_test_cumulative_sums(bits100.data(), 100, &actual_P, &actual_Q, true));
    EXPECT_NEAR(actual_P, 0.219194, 1e-6);
    EXPECT_NEAR(actual_Q, 0.219194, 1e-6);

    EXPECT_TRUE(rand_self_test_cumulative_sums(bits100.data(), 100, &actual_P, &actual_Q, false));
    EXPECT_NEAR(actual_P, 0.114866, 1e-6);
    EXPECT_NEAR(actual_Q, 0.114866, 1e-6);

    EXPECT_TRUE(rand_self_test_approximate_entropy(bits100.data(), 100, 2, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.235301, 1e-6);
    EXPECT_NEAR(actual_Q, 0.235301, 1e-6);

    EXPECT_TRUE(rand_self_test_linear_complexity(million_bits_of_e, 1000000, 1000, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.844721, 1e-6);
    EXPECT_NEAR(actual_Q, 0.844721, 1e-6);

    EXPECT_TRUE(rand_self_test_maurer_universal_statistical(million_bits_of_e, 1000000, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.282568, 1e-6);
    EXPECT_NEAR(actual_Q, 0.141284, 1e-6);

    EXPECT_TRUE(rand_self_test_discrete_fourier_transform(bits100.data(), 100, &actual_P, &actual_Q));
    EXPECT_NEAR(actual_P, 0.654721, 1e-6);
    EXPECT_NEAR(actual_Q, 0.327360, 1e-6);
}

TEST(RandomTest, PtValueTest)
{
    double V = 0.0;
    std::array<int, 10> m_FArray = {2, 5, 8, 7, 2, 5, 2, 8, 9, 2};
    int sampleCount = 50;  // Example sample count

    for (int i = 0; i < 10; ++i) {
        int Fi = m_FArray[i];
        int tmp = sampleCount / 10;
        V += (double)((Fi - tmp) * (Fi - tmp)) / (tmp);
    }
    double ptValue = cephes_igamc(9 / 2.0, V / 2.0);

    int expectValue = static_cast<int>(ptValue * 1e6);
    EXPECT_EQ(expectValue, 96578);  // Adjust the expected value based on your test case
}

int main()
{
    ::testing::InitGoogleTest();
    int result = RUN_ALL_TESTS();
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << "Some tests failed." << std::endl;
    }

    return 0;
}