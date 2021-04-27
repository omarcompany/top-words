#include "declarativewordsmodel.h"
#include "wordsengine.h"

#include <QDebug>

DeclarativeWordsModel::DeclarativeWordsModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_maximumWords{0}
    , m_engine{nullptr}
    , m_progress{0}
    , m_status{Idle}
{
    m_engine = new WordsEngine(this);
    connect(m_engine, &WordsEngine::progressChanged, this, &DeclarativeWordsModel::updateData);
    connect(m_engine, &WordsEngine::finished, this, &DeclarativeWordsModel::finish);
}

DeclarativeWordsModel::~DeclarativeWordsModel()
{

}

void DeclarativeWordsModel::openFile(const QString &path)
{
    if (!m_engine)
        return;

    setStatus(Loading);
    m_engine->openFile(path);
}

int DeclarativeWordsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_words.count();
}

QVariant DeclarativeWordsModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_words.count())
        return QVariant();

    const Word word = m_words[index.row()];

    if (role == Text) {
        return word.text;
    } else if (role == Count) {
        return word.count;
    }
    return QVariant();
}

QHash<int, QByteArray> DeclarativeWordsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Text] = "text";
    roles[Count] = "count";
    return roles;
}

void DeclarativeWordsModel::updateData(const QList<Word> &words, int maximumWords, double progress)
{
    setMaximumWords(maximumWords);

    for (int i = 0; i < m_words.count() && i < words.count(); i++) {
        if (m_words.at(i) != words.at(i)) {
            m_words[i] = words[i];
            emit dataChanged(index(i), index(i));
        }
    }

    int diff = words.count() - m_words.count();
    if (diff != 0) {
        if (diff < 0) {
            beginRemoveRows(QModelIndex(), words.count(), m_words.count() - 1);
            m_words.erase(m_words.begin() + words.count(), m_words.end());
            endRemoveRows();
            emit countChanged(rowCount());
        } else {
            beginInsertRows(QModelIndex(), m_words.count(), words.count()-1);
            m_words.append(words.mid(m_words.count()));
            endInsertRows();
            emit countChanged(rowCount());
        }
    }
    setProgress(progress);
}

void DeclarativeWordsModel::finish(WordsEngine::State state)
{
    switch (state) {
    case WordsEngine::Succes:
        setStatus(Finished);
        break;
    case WordsEngine::Canceled:
        qDebug() << "Process was canceled";
        break;
    case WordsEngine::Failed:
        qDebug() << "Loading file was failed";
        break;
    default:
        break;
    }
}

int DeclarativeWordsModel::maximumWords() const
{
    return m_maximumWords;
}

double DeclarativeWordsModel::progress() const
{
    return m_progress;
}

DeclarativeWordsModel::Status DeclarativeWordsModel::status() const
{
    return m_status;
}

void DeclarativeWordsModel::setStatus(DeclarativeWordsModel::Status status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}

void DeclarativeWordsModel::setProgress(double progress)
{
    if (m_progress == progress)
        return;

    m_progress = progress;
    emit progressChanged(m_progress);
}

void DeclarativeWordsModel::setMaximumWords(int maximumWords)
{
    if (m_maximumWords == maximumWords)
        return;

    m_maximumWords = maximumWords;
    emit maximumWordsChanged(m_maximumWords);
}
