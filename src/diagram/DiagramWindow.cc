#include <QtGui>
#include "DiagramWindow.h"
#include "Node.h"
#include "Link.h"

DiagramWindow::DiagramWindow()
{
  scene_ = new QGraphicsScene(0, 0, 600, 500);

  view_ = new QGraphicsView;
  view_->setScene(scene_);
  view_->setDragMode(QGraphicsView::RubberBandDrag);
  view_->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  view_->setContextMenuPolicy(Qt::ActionsContextMenu);
  setCentralWidget(view_);

  minZ_ = 0;
  maxZ_ = 0;
  seqNumber_ = 0;

  createActions();
  createMenus();
  createToolBars();

  connect(scene_, SIGNAL(selectionChanged()), this, SLOT(updateActions()));

  setWindowTitle(tr("Diagram"));
  updateActions();
}

void DiagramWindow::addNode()
{
  Node* node = new Node;
  node->setText(tr("Node %1").arg(seqNumber_ + 1));
  setupNode(node);
}

void DiagramWindow::setupNode(Node* node)
{
  node->setPos(QPoint(80 + (100 * (seqNumber_ % 5)), 80 + (50 * ((seqNumber_ / 5) % 7))));
  scene_->addItem(node);
  ++seqNumber_;

  scene_->clearSelection();
  node->setSelected(true);
  bringToFront();
}

