#include "wordsengine.h"
#include "declarativewordsmodel.h"

#include <QtConcurrent/QtConcurrent>
#include <QFile>
#include <QtAlgorithms>
#include <QDebug>

const static int MAXIMUM_LINES{15};

WordsEngine::WordsEngine(QObject *parent)
    : QObject{parent}
    , m_abortThread{false}
    , m_runningThread{false}
{
    qRegisterMetaType<QList<Word> >("QList<Word>");
    qRegisterMetaType<State>("State");
}

WordsEngine::~WordsEngine()
{
    m_abortThread.fetchAndStoreAcquire(true);
    m_mutex.lock();
    m_mutex.unlock();
}

void WordsEngine::openFile(const QString &path)
{
    processFileThread(path);
}

void WordsEngine::processFileThread(const QString &path)
{
    if (m_runningThread) // Abort if the user open new file
        m_abortThread.fetchAndStoreAcquire(true);

    QtConcurrent::run([this, path]() {
        m_mutex.lock();
        m_runningThread = true;

        processFile(path);

        m_runningThread = false;
        m_abortThread.fetchAndStoreAcquire(false);
        m_mutex.unlock();
    });
}

void WordsEngine::processFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit finished(Failed);
        return;
    }

    qint64 totalBytes = file.size();
    qint64 chunkBytes = 2*1024*1024; // Set size of chunk
    qint64 currentBytes = 0;
    double progress = 0;

    QHash<QString, int> words;

    // counter of sending data by chunks
    int counter = 1;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList wordList = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts); // to do!! remove symbols from words
        for (const auto &word : qAsConst(wordList)) {
            if (word.length() > 1) {
                ++words[word];
            }
        }

        currentBytes += line.size();
        if (currentBytes >= chunkBytes * counter) {
            progress += static_cast<double>(currentBytes)/static_cast<double>(totalBytes);
            sortAndSendData(words, progress);
            counter++;
        }
    }
    sortAndSendData(words, 1.0);
    emit finished(Succes);
}

void WordsEngine::sortAndSendData(const QHash<QString, int> &words, double progress)
{
    QVector<QPair<QString, int>> wordVector;
    wordVector.reserve(words.size());
    QHashIterator<QString, int> iterator(words);
    while (iterator.hasNext()) {
        iterator.next();
        QPair<QString, int> value(iterator.key(), iterator.value());
        wordVector.append(value);
    }

    std::sort(wordVector.begin(), wordVector.end(),
              [] (const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    int maximumWords = wordVector.first().second;

    QList<Word> wordList;
    for (int i = 0; i < MAXIMUM_LINES && i < wordVector.size(); i++) {
        Word word;
        word.text = wordVector.at(i).first;
        word.count = wordVector.at(i).second;
        wordList << word;
    }

    std::sort(wordList.begin(), wordList.end(),
         [](const Word &a, const Word &b) {
        return a.text.toLower() < b.text.toLower();
    });

    emit progressChanged(wordList, maximumWords, progress);
}
