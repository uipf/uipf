#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QGraphicsView>

#include "ModuleLoader.hpp"

// TODO rewrite this

namespace uipf {

class ImageWindow : public QGraphicsView
{
    Q_OBJECT

public:
    ImageWindow(ModuleLoader& mm, QWidget *parent = 0) : QGraphicsView(parent), mm_(mm) {};
    ImageWindow(ModuleLoader& mm, QGraphicsScene * scene, QWidget * parent = 0) : QGraphicsView(scene, parent), mm_(mm) {};
    ~ImageWindow() {};

protected:
	void closeEvent(QCloseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:

	ModuleLoader& mm_;

};

} // namespace

#endif // MAINWINDOW_H
