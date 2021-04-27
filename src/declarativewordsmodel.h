#ifndef DECLARATIVEWORDSMODEL_H
#define DECLARATIVEWORDSMODEL_H

#include <QAbstractListModel>

#include "wordsengine.h"

struct Word {
    explicit Word(QString text, int count) : text{text}, count{count} {};
    explicit Word() : text{""}, count{0} {};

    bool operator ==(const Word &other) const
    {
        return (text == other.text && count == other.count);
    };
    bool operator !=(const Word &other) const
    {
        return !(*this == other);
    };
    bool operator <(const Word &other) const
    {
        return !(*this == other);
    };

    bool isValid() const
    {
        return text.length() > 0 && count > 0;
    };

    QString text;
    int count;
};

class DeclarativeWordsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int maximumWords READ maximumWords NOTIFY maximumWordsChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    explicit DeclarativeWordsModel(QObject *parent = nullptr);
    ~DeclarativeWordsModel();

    enum WordRoles {
        Text = Qt::UserRole + 1,
        Count
    };

    enum Status {
        Idle,
        Loading,
        Finished
    };
    Q_ENUM(Status)

    Q_INVOKABLE void openFile(const QString &path);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    int maximumWords() const;
    double progress() const;
    Status status() const;

signals:
    void countChanged(int count);
    void maximumWordsChanged(int maximumWords);
    void progressChanged(double progress);
    void statusChanged(Status status);

private:
    void setMaximumWords(int maximumWords);
    void setProgress(double progress);
    void setStatus(Status status);

private slots:
    void updateData(const QList<Word> &words, int maximumWords, double progress);
    void finish(WordsEngine::State state);

private:
    int m_maximumWords;
    WordsEngine* m_engine;
    double m_progress;
    Status m_status;
    QList<Word> m_words;
};

#endif // DECLARATIVEWORDSMODEL_H
