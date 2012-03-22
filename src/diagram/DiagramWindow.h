#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>

class Node;
class Link;

class DiagramWindow : public QMainWindow
{
	Q_OBJECT
	
public:
  DiagramWindow();

private slots:
  void addNode();
  void addLink();
  void del();
  void cut();
  void copy();
  void paste();
  void bringToFront();
  void sendToBack();
  void properties();
  void updateActions();

private:
  typedef QPair<Node*, Node*> NodePair;
  void createActions();
  void createMenus();
  void createToolBars();
  void setZValue(int z);
  void setupNode(Node* node);
  Node* selectedNode() const;
  Link* selectedLink() const;
  NodePair selectedNodePair() const;

  QMenu* fileMenu_;
  QMenu* editMenu_;
  QToolBar* editToolBar_;
  QAction* exitAction_;
  //...
  QAction* propertiesAction_;

  QGraphicsScene* scene_;
  QGraphicsView* view_;
  
  int minZ_;
  int maxZ_;
  int seqNumber_;
};

#endif