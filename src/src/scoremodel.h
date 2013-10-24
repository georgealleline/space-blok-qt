#ifndef SCOREMODEL_H
#define SCOREMODEL_H

#include <QAbstractListModel>
#include <QColor>


class ScoreData : public QObject
{
    Q_OBJECT

public:
    ScoreData(int score, int ballId);
    ScoreData(const ScoreData &scoreData);
    ScoreData& operator=(const ScoreData &scoreData);

    int score() const;
    int ballId() const;

protected:
    int m_Score;
    int m_BallId;
};


class ScoreModel : public QAbstractListModel
{
    Q_OBJECT
public:
    // Types to carry score and ball color information
    typedef QPair<int, int> typeScoreData;
    typedef QList<typeScoreData> typeScoreList;

    enum ScoreRoles {
        ScoreRole = Qt::UserRole + 1,
        BallIdRole
    };

    ScoreModel(QObject *parent = 0);

    void setScores(const typeScoreList &scores);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    static bool sortFunction(const typeScoreData &data1,
                             const typeScoreData &data2);

protected:
    QList<ScoreData> m_Scores;

};


#endif // SCOREMODEL_H
