#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "ModuleInterface.hpp"
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

	void resizeEvent ( QResizeEvent * event );
	void wheelEvent(QWheelEvent *event);
private:

	ModuleLoader& mm_;

	bool zoomed_ = false;

	void zoom(qreal factor);

};

} // namespace

#endif // MAINWINDOW_H
