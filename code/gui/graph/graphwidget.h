#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include "ProcessingChain.hpp"
#include "../GUIEventDispatcher.hpp"

namespace uipf{
namespace gui{

class Node;

//A Class that displays the uipf::Configuration as a directed graph.
//layout is generated automatically with fruchtermann_reingold algorithm.
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);

   void renderConfig(uipf::ProcessingChain& config);

   void selectNodeByName(const QString name, bool bUnselectOthers=true);

   void triggerNodeSelected(const uipf::gui::Node*);

	void selectNodesInGraphView(const std::vector<std::string>& vcNodeNames,uipf::gui::GraphViewSelectionType eType,bool bUnselectOthers);
	void clearSelectionInGraphView();


signals: //for QT to connect
    void nodeSelected(const uipf::gui::Node*);

private:
   void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

   void scaleView(qreal scaleFactor);

   qreal currentScale_;
   std::map<std::string,uipf::gui::Node*> nodes_;
};

}//gui
}//uipf

#endif // GRAPHWIDGET_H
