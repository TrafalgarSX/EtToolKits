#include <iostream>
#include <array>
#include <random_test/self_test_rand.h>
#include <gtest/gtest.h>

TEST(RandomTest, PtValueTest) {
    double V = 0.0;
    std::array<int, 10> m_FArray = {2, 5, 8, 7, 2, 5, 2, 8, 9, 2};
    int sampleCount = 50; // Example sample count

    for(int i = 0; i < 10; ++i) {
        int Fi = m_FArray[i];
        int tmp = sampleCount / 10;
        V += (double)((Fi - tmp) * (Fi - tmp)) / (tmp);
    }
    double ptValue = cephes_igamc(9 / 2.0, V /2.0);

    int expectValue = static_cast<int>(ptValue * 1e6);
    EXPECT_EQ(expectValue, 96578); // Adjust the expected value based on your test case
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