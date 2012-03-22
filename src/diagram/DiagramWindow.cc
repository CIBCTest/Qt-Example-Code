#include <QtGui>
#include "DiagramWindow.h"
#include "Node.h"
#include "Link.h"
#include "propertiesdialog.h"

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

void DiagramWindow::bringToFront()
{
  ++maxZ_;
  setZValue(maxZ_);
}

void DiagramWindow::sendToBack()
{
  --minZ_;
  setZValue(minZ_);
}

void DiagramWindow::setZValue(int z)
{
  Node* node = selectedNode();
  if (node)
    node->setZValue(z);
}

Node* DiagramWindow::selectedNode() const
{
  QList<QGraphicsItem*> items = scene_->selectedItems();
  if (items.count() == 1)
    return dynamic_cast<Node*>(items.first());
  return 0;
}

Link* DiagramWindow::selectedLink() const
{
  QList<QGraphicsItem*> items = scene_->selectedItems();
  if (items.count() == 1)
    return dynamic_cast<Link*>(items.first());
  return 0;
}

void DiagramWindow::addLink()
{
  NodePair nodes = selectedNodePair();
  if (nodes == NodePair())
    return;

  Link* link = new Link(nodes.first, nodes.second);
  scene_->addItem(link);
}

DiagramWindow::NodePair DiagramWindow::selectedNodePair() const
{
  QList<QGraphicsItem*> items = scene_->selectedItems();
  if (items.count() == 2)
  {
    Node* first = dynamic_cast<Node*>(items.first());
    Node* second = dynamic_cast<Node*>(items.last());
    if (first && second)
      return NodePair(first, second);
  }
  return NodePair();
}

void DiagramWindow::del()
{
  QList<QGraphicsItem*> items = scene_->selectedItems();
  QMutableListIterator<QGraphicsItem*> i(items);
  while (i.hasNext())
  {
    Link* link = dynamic_cast<Link*>(i.next());
    if (link)
    {
      delete link;
      i.remove();
    }
  }
  qDeleteAll(items);
}

void DiagramWindow::properties()
{
  Node* node = selectedNode();
  Link* link = selectedLink();

  if (node)
  {
    PropertiesDialog dialog(node, this);
    dialog.exec();
  }
  else if (link)
  {
    QColor color = QColorDialog::getColor(link->color(), this);
    if (color.isValid())
      link->setColor(color);
  }
}

void DiagramWindow::cut()
{
  Node* node = selectedNode();
  if (!node)
    return;

  copy();
  delete node;
}

void DiagramWindow::copy()
{
  Node* node = selectedNode();
  if (!node)
    return;

  QString str = QString("Node %1 %2 %3 %4")
                .arg(node->textColor().name())
                .arg(node->outlineColor().name())
                .arg(node->backgroundColor().name())
                .arg(node->text());
  QApplication::clipboard()->setText(str);
}

void DiagramWindow::paste()
{
  QString str = QApplication::clipboard()->text();
  QStringList parts = str.split(" ");
  if (parts.count() >= 5 && parts.first() == "Node")
  {
    Node* node = new Node;
    node->setText(QStringList(parts.mid(4)).join(" "));
    node->setTextColor(QColor(parts[1]));
    node->setOutlineColor(QColor(parts[2]));
    node->setBackgroundColor(QColor(parts[3]));
    setupNode(node);
  }
}

void DiagramWindow::updateActions()
{
  bool hasSelection = !scene_->selectedItems().isEmpty();
  bool isNode = (selectedNode() != 0);
  bool isNodePair = (selectedNodePair() != NodePair());

  cutAction_->setEnabled(isNode);
  copyAction_->setEnabled(isNode);
  addLinkAction_->setEnabled(isNodePair);
  deleteAction_->setEnabled(hasSelection);
  bringToFrontAction_->setEnabled(isNode);
  sendToBackAction_->setEnabled(isNode);
  propertiesAction_->setEnabled(isNode);

  foreach (QAction* action, view_->actions())
    view_->removeAction(action);

  foreach (QAction* action, editMenu_->actions())
  {
    if (action->isEnabled())
      view_->addAction(action);
  }
}

void DiagramWindow::createActions()
{
  exitAction_ = new QAction(tr("E&xit"), this);
  exitAction_->setShortcut(tr("Ctrl+Q"));
  connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));

  addNodeAction_ = new QAction(tr("Add &Node"), this);
  addNodeAction_->setIcon(QIcon(":/images/node.png"));
  addNodeAction_->setShortcut(tr("Ctrl+N"));
  connect(addNodeAction_, SIGNAL(triggered()), this, SLOT(addNode()));

  addLinkAction_ = new QAction(tr("Add &Link"), this);
  addLinkAction_->setIcon(QIcon(":/images/link.png"));
  addLinkAction_->setShortcut(tr("Ctrl+L"));
  connect(addLinkAction_, SIGNAL(triggered()), this, SLOT(addLink()));

  deleteAction_ = new QAction(tr("&Delete"), this);
  deleteAction_->setIcon(QIcon(":/images/delete.png"));
  deleteAction_->setShortcut(tr("Del"));
  connect(deleteAction_, SIGNAL(triggered()), this, SLOT(del()));

  cutAction_ = new QAction(tr("Cu&t"), this);
  cutAction_->setIcon(QIcon(":/images/cut.png"));
  cutAction_->setShortcut(tr("Ctrl+X"));
  connect(cutAction_, SIGNAL(triggered()), this, SLOT(cut()));

  copyAction_ = new QAction(tr("&Copy"), this);
  copyAction_->setIcon(QIcon(":/images/copy.png"));
  copyAction_->setShortcut(tr("Ctrl+C"));
  connect(copyAction_, SIGNAL(triggered()), this, SLOT(copy()));

  pasteAction_ = new QAction(tr("&Paste"), this);
  pasteAction_->setIcon(QIcon(":/images/paste.png"));
  pasteAction_->setShortcut(tr("Ctrl+V"));
  connect(pasteAction_, SIGNAL(triggered()), this, SLOT(paste()));

  bringToFrontAction_ = new QAction(tr("Bring to &Front"), this);
  bringToFrontAction_->setIcon(QIcon(":/images/bringtofront.png"));
  connect(bringToFrontAction_, SIGNAL(triggered()),
    this, SLOT(bringToFront()));

  sendToBackAction_ = new QAction(tr("&Send to Back"), this);
  sendToBackAction_->setIcon(QIcon(":/images/sendtoback.png"));
  connect(sendToBackAction_, SIGNAL(triggered()),
    this, SLOT(sendToBack()));

  propertiesAction_ = new QAction(tr("P&roperties..."), this);
  connect(propertiesAction_, SIGNAL(triggered()),
    this, SLOT(properties()));
}

void DiagramWindow::createMenus()
{
  fileMenu_ = menuBar()->addMenu(tr("&File"));
  fileMenu_->addAction(exitAction_);

  editMenu_ = menuBar()->addMenu(tr("&Edit"));
  editMenu_->addAction(addNodeAction_);
  editMenu_->addAction(addLinkAction_);
  editMenu_->addAction(deleteAction_);
  editMenu_->addSeparator();
  editMenu_->addAction(cutAction_);
  editMenu_->addAction(copyAction_);
  editMenu_->addAction(pasteAction_);
  editMenu_->addSeparator();
  editMenu_->addAction(bringToFrontAction_);
  editMenu_->addAction(sendToBackAction_);
  editMenu_->addSeparator();
  editMenu_->addAction(propertiesAction_);
}

void DiagramWindow::createToolBars()
{
  editToolBar_ = addToolBar(tr("Edit"));
  editToolBar_->addAction(addNodeAction_);
  editToolBar_->addAction(addLinkAction_);
  editToolBar_->addAction(deleteAction_);
  editToolBar_->addSeparator();
  editToolBar_->addAction(cutAction_);
  editToolBar_->addAction(copyAction_);
  editToolBar_->addAction(pasteAction_);
  editToolBar_->addSeparator();
  editToolBar_->addAction(bringToFrontAction_);
  editToolBar_->addAction(sendToBackAction_);
}