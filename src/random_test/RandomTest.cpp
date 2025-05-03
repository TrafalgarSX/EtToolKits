#include "RandomTest.h"

#include <QDebug>
#include <QUrl>
#include <cmath>

#include "self_test_rand.h"


RandomTest::RandomTest(QObject* parent) : QObject(parent)
{
    m_randomTestData.push_back(new RandomTestData{"单比特频数检测", 0, 0, RandomTestAlgorithm::FrequencyTest});
    m_randomTestData.push_back(
        new RandomTestData{"块内频数检测 m=10000", 0, 0, RandomTestAlgorithm::BlockFrequencyTest});
    m_randomTestData.push_back(new RandomTestData{"扑克检测 m=4", 0, 0, RandomTestAlgorithm::PokerTest_4});
    m_randomTestData.push_back(new RandomTestData{"扑克检测 m=8", 0, 0, RandomTestAlgorithm::PokerTest_8});
    m_randomTestData.push_back(new RandomTestData{"重叠子序列检测 m=3 P1", 0, 0, RandomTestAlgorithm::SerialTest_m3P1});
    m_randomTestData.push_back(new RandomTestData{"重叠子序列检测 m=3 P2", 0, 0, RandomTestAlgorithm::SerialTest_m3P2});
    m_randomTestData.push_back(new RandomTestData{"重叠子序列检测 m=5 P1", 0, 0, RandomTestAlgorithm::SerialTest_m5P1});
    m_randomTestData.push_back(new RandomTestData{"重叠子序列检测 m=5 P2", 0, 0, RandomTestAlgorithm::SerialTest_m5P2});
    m_randomTestData.push_back(new RandomTestData{"游程总数检测", 0, 0, RandomTestAlgorithm::RunsTest});
    m_randomTestData.push_back(new RandomTestData{"游程分布检测", 0, 0, RandomTestAlgorithm::RunsDistribution});
    m_randomTestData.push_back(
        new RandomTestData{"块内最大1游程检测 m=10000", 0, 0, RandomTestAlgorithm::LongestRunOfOnesTest});
    m_randomTestData.push_back(
        new RandomTestData{"块内最大0游程检测 m=10000", 0, 0, RandomTestAlgorithm::LongestRunOfZerosTest});
    m_randomTestData.push_back(
        new RandomTestData{"二元推导检测 k=3", 0, 0, RandomTestAlgorithm::BinaryDerivationTest_k3});
    m_randomTestData.push_back(
        new RandomTestData{"二元推导检测 k=7", 0, 0, RandomTestAlgorithm::BinaryDerivationTest_k7});
    m_randomTestData.push_back(new RandomTestData{"自相关检测 d=1", 0, 0, RandomTestAlgorithm::SelfCorrelationTest_d1});
    m_randomTestData.push_back(new RandomTestData{"自相关检测 d=2", 0, 0, RandomTestAlgorithm::SelfCorrelationTest_d2});
    m_randomTestData.push_back(new RandomTestData{"自相关检测 d=8", 0, 0, RandomTestAlgorithm::SelfCorrelationTest_d8});
    m_randomTestData.push_back(
        new RandomTestData{"自相关检测 d=16", 0, 0, RandomTestAlgorithm::SelfCorrelationTest_d16});
    m_randomTestData.push_back(new RandomTestData{"矩阵秩检测", 0, 0, RandomTestAlgorithm::BinaryMatrixRankTest});
    m_randomTestData.push_back(
        new RandomTestData{"累加和前向检测", 0, 0, RandomTestAlgorithm::CumulativeSumsTest_before});
    m_randomTestData.push_back(
        new RandomTestData{"累加和后向检测", 0, 0, RandomTestAlgorithm::CumulativeSumsTest_after});
    m_randomTestData.push_back(
        new RandomTestData{"近似熵检测 m=2", 0, 0, RandomTestAlgorithm::ApproximateEntropyTest_m2});
    m_randomTestData.push_back(
        new RandomTestData{"近似熵检测 m=5", 0, 0, RandomTestAlgorithm::ApproximateEntropyTest_m5});
    m_randomTestData.push_back(
        new RandomTestData{"线型复杂度检测 m=500", 0, 0, RandomTestAlgorithm::LinearComplexityTest_m500});
    m_randomTestData.push_back(
        new RandomTestData{"线型复杂度检测 m=1000", 0, 0, RandomTestAlgorithm::LinearComplexityTest_m1000});
    m_randomTestData.push_back(
        new RandomTestData{"Maurer通用统计检测 L=7 Q=1280", 0, 0, RandomTestAlgorithm::MaurerUniversalStatisticalTest});
    m_randomTestData.push_back(
        new RandomTestData{"离散傅里叶检测", 0, 0, RandomTestAlgorithm::DiscreteFourierTransformTest});
    m_randomTestListData = getRandomTestDataAsVariantList();

    connect(this, &RandomTest::fileDataReady, this, &RandomTest::onFileDataReady);
    m_timer = new QTimer(this);
}

RandomTest::~RandomTest() { m_randomTestData.clear(); }

QVariantList RandomTest::getRandomTestDataAsVariantList() const
{
    QVariantList list;

    for (int i = 0; i < m_randomTestData.size(); ++i) {
        QVariantMap map;
        map["randTestAlgorithm"] = m_randomTestData[i]->m_randTestAlgorithm;
        map["significantLevelSampleCount"] = m_randomTestData[i]->m_significantLevelSampleCount.load();
        map["nonSignificantLevelSampleCount"] = m_randomTestData[i]->m_nonSignificantLevelSampleCount.load();
        map["uniformityPTValue"] = QString::number(m_randomTestData[i]->m_uniformityPTValue, 'f', 4);
        map["key"] = QString::number(static_cast<int>(m_randomTestData[i]->m_key));
        list.append(map);
    }

    for (const auto& data : m_randomTestData) {
    }
    return list;
}

void RandomTest::clearRandomTestData()
{
    m_randomTestData.clear();
    emit randomTestListDataChanged(m_randomTestListData);
}

QVariantList RandomTest::randomTestListData() const { return m_randomTestListData; }

constexpr uint64_t sample_file_size1 = 1000 * 1000;
constexpr uint64_t sample_file_size2 = 1024 * 1024;

bool RandomTest::readFileData(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return false;
    }

    // file size should be 1000 * 1000 bytes or 1024 * 1024 bytes
    qint64 fileSize = file.size();
    if (fileSize < sample_file_size1) {
        qDebug() << "File size is not valid:" << fileSize;
        // emit invalidFileSize(fileSize);
        return false;
    }

    if (fileSize != sample_file_size1 && fileSize != sample_file_size2) {
        qDebug() << "File size is not valid:" << fileSize;
        // emit notRandomFileSize(fileSize);
        return false;
    }

    QByteArray fileDataBuffer = file.readAll();
    file.close();

    m_fileDataQueue.enqueue(fileDataBuffer);
    return true;
}

void RandomTest::testSample(QList<int> algorithms, const QByteArray& fileDataBuffer)
{
    uint64_t nbit = fileDataBuffer.size() << 3;  // fileDataBuffer.size() * 8
    unsigned char* buf = (unsigned char*)fileDataBuffer.data();
    // Implement the random test logic here
    int fail;
    for (int algorithm : algorithms) {
        switch (algorithm) {
            case 0:
                fail = rand_self_test_frequency(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[0]->m_significantLevelSampleCount += !fail;
                m_randomTestData[0]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 1:
                fail = rand_self_test_block_frequency(buf, nbit, 10000, nullptr, nullptr) ^ 1;
                m_randomTestData[1]->m_significantLevelSampleCount += !fail;
                m_randomTestData[1]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 2:
                fail = rand_self_test_poker(buf, nbit, 4, nullptr, nullptr) ^ 1;
                m_randomTestData[2]->m_significantLevelSampleCount += !fail;
                m_randomTestData[2]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 3:
                fail = rand_self_test_poker(buf, nbit, 8, nullptr, nullptr) ^ 1;
                m_randomTestData[3]->m_significantLevelSampleCount += !fail;
                m_randomTestData[3]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 4:
                fail = rand_self_test_serial(buf, nbit, 3, nullptr, nullptr, nullptr, nullptr) ^ 1;
                m_randomTestData[4]->m_significantLevelSampleCount += !fail;
                m_randomTestData[4]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 5:
                fail = rand_self_test_serial(buf, nbit, 3, nullptr, nullptr, nullptr, nullptr) ^ 1;
                m_randomTestData[5]->m_significantLevelSampleCount += !fail;
                m_randomTestData[5]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 6:
                fail = rand_self_test_serial(buf, nbit, 5, nullptr, nullptr, nullptr, nullptr) ^ 1;
                m_randomTestData[6]->m_significantLevelSampleCount += !fail;
                m_randomTestData[6]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 7:
                fail = rand_self_test_serial(buf, nbit, 5, nullptr, nullptr, nullptr, nullptr) ^ 1;
                m_randomTestData[7]->m_significantLevelSampleCount += !fail;
                m_randomTestData[7]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 8:
                fail = rand_self_test_runs(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[8]->m_significantLevelSampleCount += !fail;
                m_randomTestData[8]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 9:
                fail = rand_self_test_runs_distribution(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[9]->m_significantLevelSampleCount += !fail;
                m_randomTestData[9]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 10:
                fail = rand_self_test_longest_run_of_ones(buf, nbit, true, nullptr, nullptr) ^ 1;
                m_randomTestData[10]->m_significantLevelSampleCount += !fail;
                m_randomTestData[10]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 11:
                fail = rand_self_test_longest_run_of_ones(buf, nbit, false, nullptr, nullptr) ^ 1;
                m_randomTestData[11]->m_significantLevelSampleCount += !fail;
                m_randomTestData[11]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 12:
                fail = rand_self_test_binary_derivation(buf, nbit, 3, nullptr, nullptr) ^ 1;
                m_randomTestData[12]->m_significantLevelSampleCount += !fail;
                m_randomTestData[12]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 13:
                fail = rand_self_test_binary_derivation(buf, nbit, 7, nullptr, nullptr) ^ 1;
                m_randomTestData[13]->m_significantLevelSampleCount += !fail;
                m_randomTestData[13]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 14:
                fail = rand_self_test_self_correlation(buf, nbit, 1, nullptr, nullptr) ^ 1;
                m_randomTestData[14]->m_significantLevelSampleCount += !fail;
                m_randomTestData[14]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 15:
                fail = rand_self_test_self_correlation(buf, nbit, 2, nullptr, nullptr) ^ 1;
                m_randomTestData[15]->m_significantLevelSampleCount += !fail;
                m_randomTestData[15]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 16:
                fail = rand_self_test_self_correlation(buf, nbit, 8, nullptr, nullptr) ^ 1;
                m_randomTestData[16]->m_significantLevelSampleCount += !fail;
                m_randomTestData[16]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 17:
                fail = rand_self_test_self_correlation(buf, nbit, 16, nullptr, nullptr) ^ 1;
                m_randomTestData[17]->m_significantLevelSampleCount += !fail;
                m_randomTestData[17]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 18:
                fail = rand_self_test_binary_matrix_rank(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[18]->m_significantLevelSampleCount += !fail;
                m_randomTestData[18]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 19:
                fail = rand_self_test_cumulative_sums(buf, nbit, nullptr, nullptr, true) ^ 1;
                m_randomTestData[19]->m_significantLevelSampleCount += !fail;
                m_randomTestData[19]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 20:
                fail = rand_self_test_cumulative_sums(buf, nbit, nullptr, nullptr, false) ^ 1;
                m_randomTestData[20]->m_significantLevelSampleCount += !fail;
                m_randomTestData[20]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 21:
                fail = rand_self_test_approximate_entropy(buf, nbit, 2, nullptr, nullptr) ^ 1;
                m_randomTestData[21]->m_significantLevelSampleCount += !fail;
                m_randomTestData[21]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 22:
                fail = rand_self_test_approximate_entropy(buf, nbit, 5, nullptr, nullptr) ^ 1;
                m_randomTestData[22]->m_significantLevelSampleCount += !fail;
                m_randomTestData[22]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 23:
                fail = rand_self_test_linear_complexity(buf, nbit, 500, nullptr, nullptr) ^ 1;
                m_randomTestData[23]->m_significantLevelSampleCount += !fail;
                m_randomTestData[23]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 24:
                fail = rand_self_test_linear_complexity(buf, nbit, 1000, nullptr, nullptr) ^ 1;
                m_randomTestData[24]->m_significantLevelSampleCount += !fail;
                m_randomTestData[24]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 25:
                fail = rand_self_test_maurer_universal_statistical(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[25]->m_significantLevelSampleCount += !fail;
                m_randomTestData[25]->m_nonSignificantLevelSampleCount += fail;
                break;
            case 26:
                fail = rand_self_test_discrete_fourier_transform(buf, nbit, nullptr, nullptr) ^ 1;
                m_randomTestData[26]->m_significantLevelSampleCount += !fail;
                m_randomTestData[26]->m_nonSignificantLevelSampleCount += fail;
                break;
            default:
                qDebug() << "Unknown algorithm key:" << algorithm;
                break;
        }
    }
}

void RandomTest::runRandomTest(QList<int> algorithms)
{
    QUrl url(m_sampleUrl);
    QString filePath = url.toLocalFile();
    QFileInfo fileInfo(filePath);
    QString dirPath = fileInfo.absolutePath();
    QString baseName = fileInfo.baseName();
    QString suffix = fileInfo.suffix();

    qDebug() << "algorithms:" << algorithms;
    // Extract the numeric part of the file name
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(baseName);
    if (!match.hasMatch()) {
        qDebug() << "No numeric part found in file name:" << baseName;
        return;
    }

    QString numSuffix = match.captured(0);
    int fileIndex = numSuffix.toInt();
    QString filePrefix = baseName.left(match.capturedStart(0));

    // Submit the file reading task to the thread pool
    m_threadPool.submit([this, numSuffix, filePrefix, dirPath, fileIndex, algorithms, suffix]() mutable {
        while (fileIndex <= 999) {
            QString fileNumSuf;
            if (numSuffix.size() == 1) {
                fileNumSuf = QString::number(fileIndex);
            } else if (numSuffix.size() == 3) {
                fileNumSuf = QString::number(fileIndex).rightJustified(3, '0');
            } else if (numSuffix.size() == 0) {
                fileNumSuf = "";
            }
            QString currentFileName = filePrefix + fileNumSuf + "." + suffix;
            QString currentFilePath = dirPath + "/" + currentFileName;

            QByteArray fileData;
            if(!algorithms.isEmpty()) {
                qDebug() << "algorithms is empty!";
                QFile file(currentFilePath);
                if (!file.exists()) {
                    qDebug() << "File does not exist:" << currentFilePath;
                    break;
                }

                if (!file.open(QIODevice::ReadOnly)) {
                    qDebug() << "Failed to open file:" << currentFilePath;
                    break;
                }

                fileData = file.readAll();
                file.close();
            }

#ifdef USE_CONDITION_VARIABLE
            // Wait if the queue is full
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_not_full.wait(lock, [this]() { return m_queue_size < max_queue_size; });
            }

            m_fileDataQueue.enqueue(fileData);
            ++m_queue_size;
#elif defined(USE_QT_SEMAPHORE)
            m_freeSpace.acquire();  // 等待空闲空间
            m_fileDataQueue.enqueue(fileData);
            m_usedSpace.release();  // 增加已使用空间
#endif

            emit fileDataReady(algorithms);  // Notify that new data is available
            fileIndex++;
        }
    });

    connect(m_timer, &QTimer::timeout, this, [this]() {
        double progress = (double)m_processData / m_sampleCount;

        if (m_processData >= m_sampleCount) {
            setProgress(progress);
            m_timer->stop();
            QString testRetInfo;
            bool testRet = checkRandomTestResult(testRetInfo);
            qDebug() << testRetInfo;
            if(testRet) {
                emit randomTestSuccess(testRetInfo);
            } else {
                emit randomTestFailed(testRetInfo);
            }
        }
        m_randomTestListData = getRandomTestDataAsVariantList();
        emit randomTestListDataChanged(m_randomTestListData);
        setProgress(progress);
    });

    m_timer->start(2000);  // 1 second interval
}

void RandomTest::onFileDataReady(QList<int> algorithms)
{
#ifdef USE_CONDITION_VARIABLE
    // Submit the testSample task to the thread pool
    m_threadPool.submit([this]() {
        QByteArray fileDataBuffer;
        if (!m_fileDataQueue.dequeue(fileDataBuffer)) {
            m_not_full.notify_one();
            return;
        }
        // Execute the ample logic
        testSample(fileDataBuffer);

        // Notify producer thread that there is space in the queue
        --m_queue_size;
        m_not_full.notify_one();
    });
#elif defined(USE_QT_SEMAPHORE)
    // Submit the testSample task to the thread pool
    m_threadPool.submit([this, algorithms]() {
        m_usedSpace.acquire();  // 等待已使用空间
        QByteArray fileDataBuffer;
        if (!m_fileDataQueue.dequeue(fileDataBuffer)) {
            return;
        }
        // Execute the ample logic
        testSample(algorithms, fileDataBuffer);

        // Notify producer thread that there is space in the queue

        m_processData++;
        qDebug() << "m_processData:" << m_processData;
        m_freeSpace.release();  // 释放一个空闲位置
    });
#endif
}

void RandomTest::significantLevel(double significantLevel) { setAlpha(significantLevel); }

double RandomTest::progress() const { return m_progress; }

void RandomTest::setProgress(double progress)
{
    if (m_progress != progress) {
        m_progress = progress;
        emit progressChanged(m_progress);
    }
}

void RandomTest::setsampleUrl(const QString& sampleUrl)
{
    if (m_sampleUrl != sampleUrl) {
        qDebug() << "sampleUrl:" << sampleUrl;
        m_sampleUrl = sampleUrl;
    }
}

QList<QVariant> RandomTest::checkSampleInfo(QString localFilePath)
{
    QVariantMap map;
    QList<QVariant> list;
    QFileInfo fileInfo(localFilePath);
    QString dirPath = fileInfo.absolutePath();
    QString baseName = fileInfo.baseName();
    QString suffix = fileInfo.suffix();

    if (suffix != "bin") {
        qDebug() << "File suffix is not bin:" << suffix;
        // TODO
        return list;
    }

    // Extract the numeric part of the file name
    QRegularExpression re("\\d+");
    QRegularExpressionMatch match = re.match(baseName);
    if (!match.hasMatch()) {
        qDebug() << "No numeric part found in file name:" << baseName;
        // TODO
        return list;
    }
    int sampleSize = 0;
    bool getSampleSize = true;

    QString numSuffix = match.captured(0);
    int fileIndex = numSuffix.toInt();
    QString filePrefix = baseName.left(match.capturedStart(0));
    while (fileIndex <= 999) {
        QString fileNumSuf;
        if (numSuffix.size() == 1) {
            fileNumSuf = QString::number(fileIndex);
        } else if (numSuffix.size() == 3) {
            fileNumSuf = QString::number(fileIndex).rightJustified(3, '0');
        } else if (numSuffix.size() == 0) {
            fileNumSuf = "";
        }
        QString currentFileName = filePrefix + fileNumSuf + "." + suffix;
        QString currentFilePath = dirPath + "/" + currentFileName;

        QFile file(currentFilePath);
        if (!file.exists()) {
            qDebug() << "File does not exist:" << currentFilePath;
            break;
        }

        if (getSampleSize) {
            sampleSize = file.size() * 8;
            getSampleSize = false;
        }

        ++fileIndex;
    }
    m_sampleCount = fileIndex;
    int needPassSampleCount = 0;
    double alpha = getAlpha();
    needPassSampleCount =
        static_cast<int>(std::round(m_sampleCount * (1 - alpha - 3 * (sqrt(alpha * (1 - alpha) / m_sampleCount)))));

    map["sampleCount"] = m_sampleCount;
    map["sampleSize"] = sampleSize;
    map["needPassSampleCount"] = needPassSampleCount;
    list.append(map);
    return list;
}

bool RandomTest::checkRandomTestResult(QString& testRetInfo)
{
    bool testRet = true;
    double alpha = getAlpha();
    int needPassSampleCount =
        static_cast<int>(std::round(m_sampleCount * (1 - alpha - 3 * (sqrt(alpha * (1 - alpha) / m_sampleCount)))));
    QString failTestName;
    int failTestCount = 0;
    for(int i = 0; i < m_randomTestData.size(); ++i) {
        if (m_randomTestData[i]->m_significantLevelSampleCount.load() < needPassSampleCount) {
            testRet = false;
            failTestName += m_randomTestData[i]->m_randTestAlgorithm + "\n";
            failTestCount++;
        }
    }

    // TODO 样本分布均匀性判定

    if(testRet) {
        testRetInfo = "随机性测试通过！";
    } else {
        testRetInfo = "随机性测试失败！\n测试不通过的个数：" + QString::number(failTestCount) + "\n" + failTestName;
    }
    return testRet;
}