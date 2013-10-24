#include <qglscenenode.h>
#include <qglpainter.h>
#include <qglmaterialcollection.h>
#include "score.h"
#include "scoredigit.h"


/*!
  \class Score
  \brief Represents a score drawn beside each Platform. Uses ScoreDigit
         objects to draw each digit individually.
*/


/*!
  Constructor.
*/
Score::Score(QGLMaterialCollection *materialCollection,
             int materialIndex,
             const QVector3D &pos,
             const QQuaternion &quaternion,
             QObject *parent)
    : QGLSceneNode(parent)
{
    setPosition(pos);

    QMatrix4x4 transform = localTransform();
    transform.rotate(quaternion);
    setLocalTransform(transform);

    // The ScoreDigits will be placed as child nodes of this

    // Score will be reported with 3 digits.
    for (int i=0; i<3; i++) {
        m_Digits << new ScoreDigit(materialCollection,
                                   materialIndex,
                                   QVector3D(0.6f*i - 1.5*0.6f, 0.0f, 0.0f),
                                   this);
    }

    setScore(0);
}


/*!
  Return score that is currently drawn by each digits.
*/
int Score::score() const
{
    int score = 0;

    int digitMagnitude = 1;
    for(int i=0; i<m_Digits.count()-1; i++) {
        digitMagnitude *= 10;
    }

    foreach(ScoreDigit *digit, m_Digits) {
        int digitValue = digit->value();
        if (digitValue < 0) {
            // If value is < 0, digit is hidden and it equals zero.
            digitValue = 0;
        }

        score += digitValue * digitMagnitude;
        digitMagnitude /= 10;
    }

    return score;
}


/*!
  Sets the score that will be drawn by each digits.
*/
void Score::setScore(int score)
{
    int digit = 0;

    foreach(ScoreDigit *digit, m_Digits) {
         digit->setValue(-1);
    }

    if (score == 0) {
        m_Digits.last()->setValue(0);
        return;
    }

    while (score != 0) {
        int digitValue = score % 10;

        int digitIndex = m_Digits.count() - 1 - digit;
        if (digitIndex >= 0) {
            m_Digits[digitIndex]->setValue(digitValue);
        }

        score /= 10;
        digit++;
    }
}


/*!
  Draws the member m_SceneNode
*/
void Score::draw(QGLPainter *painter)
{
    glEnable(GL_BLEND);
    QGLSceneNode::draw(painter);
    glDisable(GL_BLEND);
}
