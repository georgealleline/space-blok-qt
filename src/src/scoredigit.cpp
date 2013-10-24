#include <qglbuilder.h>
#include <qglmaterialcollection.h>
#include <qglpainter.h>
#include "scoredigit.h"

/*!
  \class ScoreDigit
  \brief Represents a single digit in a score beside the platforms. Each digit
         are implemented with a texture attached to a 3D object's face.
*/


/*!
  Constructor, the material collection and indexes of 10 materials are given
  to access the materials for each digit.
*/
ScoreDigit::ScoreDigit(QGLMaterialCollection *materialCollection,
                     int materialIndex,
                     const QVector3D &pos,
                     QGLSceneNode *parent)
    : QGLSceneNode(parent)
{
    QGLBuilder builder;
    builder.addPane(0.7f);
    QGLSceneNode *rootNode = builder.finalizedSceneNode();

    addNode(rootNode->children()[0]->clone(this));

    delete rootNode;
    rootNode = 0;

    setPosition(pos);
    setPalette(materialCollection);
    setMaterialIndex(materialIndex);
    setEffect(QGL::FlatReplaceTexture2D);

    // Hide the digit at start
    setValue(-1);
}


/*!
  Destructor.
*/
ScoreDigit::~ScoreDigit()
{
}


/*!
  Returns the current digit that will be drawn. The digit is in range 0-9
  representing the current digit and -1 if the digit is hidden.
*/
int ScoreDigit::value() const
{
    return m_Value;
}


/*!
  Sets the digit to be drawn.
*/
void ScoreDigit::setValue(int value)
{
    if (m_Value == value) {
        return;
    }

    m_Value = value;
    QGeometryData geometryData = children()[0]->geometry();
    geometryData.texCoord(0).setX(0.1f * m_Value);
    geometryData.texCoord(1).setX(0.1f * m_Value + 0.1f);
    geometryData.texCoord(2).setX(0.1f * m_Value + 0.1f);
    geometryData.texCoord(3).setX(0.1f * m_Value);
}


/*!
  Draws the digit with alpha blending.
*/
void ScoreDigit::draw(QGLPainter *painter)
{
    if (m_Value >= 0) {
        QGLSceneNode::draw(painter);
    }
}
