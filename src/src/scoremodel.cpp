#include <qdeclarative.h>
#include "scoremodel.h"


/*!
  \class ScoreData
  \brief Custom QML data element to transfer score information to the QML UI.
*/

/*!
  Constructor, initializes ScoreData.
*/
ScoreData::ScoreData(int score, int ballId)
    : m_Score(score),
      m_BallId(ballId)
{
}


/*!
  Copy constructor.
*/
ScoreData::ScoreData(const ScoreData &scoreData)
    : QObject(),
      m_Score(scoreData.m_Score),
      m_BallId(scoreData.m_BallId)
{
}


/*!
  Assigment operator.
*/
ScoreData& ScoreData::operator=(const ScoreData &scoreData)
{
    m_Score = scoreData.m_Score;
    m_BallId = scoreData.m_BallId;

    return *this;
}


/*!
  Score get method.
*/
int ScoreData::score() const
{
    return m_Score;
}


/*!
  Color get method.
*/
int ScoreData::ballId() const
{
    return m_BallId;
}


/*!
  \class ScoreModel
  \brief Custom list model to provide score information to winning screen
         in QML
*/

/*!
  Constructor, sets the roles for data. QML will query data with these roles.
*/
ScoreModel::ScoreModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[ScoreRole] = "score";
    roles[BallIdRole] = "ballId";
    setRoleNames(roles);
}


/*!
  Sets the scores for the model. Model is first cleared and then given data is
  added to the model. The scores is added as descending order.
*/
void ScoreModel::setScores(const typeScoreList &scores)
{
    typeScoreList scoreCopy = scores;
    qSort(scoreCopy.begin(), scoreCopy.end(), sortFunction);


    if (!m_Scores.empty()) {
        beginRemoveRows(QModelIndex(), 0, m_Scores.count() - 1);
        m_Scores.clear();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    foreach (typeScoreData score, scoreCopy) {
        m_Scores << ScoreData(score.first, score.second);
    }

    endInsertRows();
}


/*!
  Returns the row count.
*/
int ScoreModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_Scores.count();
}


/*!
  Returns the data in given row and role.
*/
QVariant ScoreModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_Scores.count())
        return QVariant();

    const ScoreData &score = m_Scores[index.row()];
    if (role == ScoreRole)
        return score.score();
    else if (role == BallIdRole)
        return score.ballId();

    return QVariant();
}


/*!
  Static function for qSort to sort the score data in descening order by the
  score.
*/
bool ScoreModel::sortFunction(const typeScoreData &data1,
                              const typeScoreData &data2)
{
    return data1.first > data2.first;
}


QML_DECLARE_TYPE(ScoreModel)
