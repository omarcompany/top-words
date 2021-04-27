#ifndef WORDSENGINE_H
#define WORDSENGINE_H

#include <QObject>
#include <QMutex>

struct Word;

class WordsEngine : public QObject
{
    Q_OBJECT
public:
    explicit WordsEngine(QObject *parent = nullptr);
    ~WordsEngine();

    enum State {
        Succes,
        Failed,
        Canceled, // To do!! Should add ability to cancel loading
        Loading
    };
    Q_ENUM(State)

    void openFile(const QString &path);

signals:
    void progressChanged(const QList<Word> &words, int maximumWords, double progress);
    void finished(State status);

private:
    void processFileThread(const QString &path);
    void processFile(const QString &path);
    void sortAndSendData(const QHash<QString, int> &words, double progress);

private:
    QAtomicInteger<bool> m_abortThread;
    bool m_runningThread;
    mutable QMutex m_mutex;
};

#endif // WORDSENGINE_H
