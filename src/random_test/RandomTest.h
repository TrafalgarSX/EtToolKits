#ifndef _RANDTEST_H_
#define _RANDTEST_H_

#include <ThreadPool.h>

#include <QObject>
#include <QSemaphore>
#include <QString>
#include <QtQml>
#include <QList>
#include <QTimer>
#include <atomic>
#include <memory>

enum class RandomTestAlgorithm {
    FrequencyTest = 0,
    BlockFrequencyTest,
    PokerTest_4,
    PokerTest_8,
    SerialTest_m3P1,
    SerialTest_m3P2,
    SerialTest_m5P1,
    SerialTest_m5P2,
    RunsTest,
    RunsDistribution,
    LongestRunOfOnesTest,
    LongestRunOfZerosTest,
    BinaryDerivationTest_k3,
    BinaryDerivationTest_k7,
    SelfCorrelationTest_d1,
    SelfCorrelationTest_d2,
    SelfCorrelationTest_d8,
    SelfCorrelationTest_d16,
    BinaryMatrixRankTest,
    CumulativeSumsTest_before,
    CumulativeSumsTest_after,
    ApproximateEntropyTest_m2,
    ApproximateEntropyTest_m5,
    LinearComplexityTest_m500,
    LinearComplexityTest_m1000,
    MaurerUniversalStatisticalTest,  // L=7 Q=1280
    DiscreteFourierTransformTest
};

class RandomTestData
{
   public:
    RandomTestData(QString algorithm, int significantLevelSampleCount, int nonSignificantLevelSampleCount, enum class RandomTestAlgorithm key)
        : m_randTestAlgorithm(algorithm),
          m_significantLevelSampleCount(significantLevelSampleCount),
          m_nonSignificantLevelSampleCount(nonSignificantLevelSampleCount),
          m_key(key)
    {
    }

    RandomTestData(const RandomTestData&) = delete;             // 禁止拷贝
    RandomTestData& operator=(const RandomTestData&) = delete;  // 禁止赋值

    enum class RandomTestAlgorithm m_key;
    QString m_randTestAlgorithm = "";
    std::atomic_int m_significantLevelSampleCount = 1000;
    std::atomic_int m_nonSignificantLevelSampleCount{};
    double m_uniformityPTValue = 0.0;
};

constexpr int max_queue_size = 50;  // 最大队列大小

// #define USE_CONDITION_VARIABLE 
#define USE_QT_SEMAPHORE 
class RandomTest : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList randomTestListData READ randomTestListData NOTIFY randomTestListDataChanged)
    Q_PROPERTY(double progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString sampleUrl READ sampleUrl WRITE setsampleUrl)
   public:
    explicit RandomTest(QObject* parent = nullptr);
    ~RandomTest() override;
    Q_DISABLE_COPY(RandomTest)

    Q_INVOKABLE void runRandomTest(QList<int> algorithms);
    Q_INVOKABLE void clearRandomTestData();

    Q_INVOKABLE QVariantList randomTestListData() const;
    Q_INVOKABLE void significantLevel(double significantLevel);
    Q_INVOKABLE QList<QVariant> checkSampleInfo(QString localFilePath);

    double progress() const;
    void setProgress(double progress);
    QString sampleUrl() const { return m_sampleUrl; }
    void setsampleUrl(const QString& sampleUrl);

   signals:
    void randomTestListDataChanged(const QVariantList& randomTestData);
    void fileDataReady(QList<int> algorithms);  // Notify that new data is available
    void progressChanged(double progress);  // Notify that progress has changed
    void randomTestSuccess(QString testRetInfo);  // Notify that random test succeeded
    void randomTestFailed(QString testRetInfo);  // Notify that random test failed

   private slots:
    void onFileDataReady(QList<int> algorithms);  // Slot to handle the file data ready signal
   private:
    QVariantList getRandomTestDataAsVariantList() const;
    bool readFileData(QString filePath);
    void testSample(QList<int> algorithms, const QByteArray& fileDataBuffer);
    bool checkRandomTestResult(QString& failTestInfo);
   private:
    QVector<RandomTestData*> m_randomTestData;
    QVariantList m_randomTestListData;
    std::unique_ptr<ThreadPool> m_threadPoolPtr;  // 线程池智能指针
    SafeQueue<QByteArray> m_fileDataQueue;
#ifdef USE_CONDITION_VARIABLE
    std::mutex m_mutex;
    std::condition_variable m_not_full;
    std::atomic_int m_queue_size = 0;  // 当前队列大小
#elif defined (USE_QT_SEMAPHORE)
    QSemaphore m_freeSpace{max_queue_size};  // 初始空闲空间为 50
    QSemaphore m_usedSpace{0};   // 初始已使用空间为 0
#endif
    double m_progress = 0.0; // 进度
    QTimer* m_timer = nullptr; // 定时器
    QString m_sampleUrl = ""; // 样本文件路径
    int m_sampleCount = 0;
    std::atomic_int m_processData{0}; // 处理数据的数量
};

#endif  // _RANDTEST_H_