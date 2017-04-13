#include "ImageWindow.hpp"

#include <iostream>
#include <logging.hpp>


using namespace uipf;
using namespace std;

void ImageWindow::closeEvent(QCloseEvent *event) {

	// resume a paused chain when an image window gets closed
//	mm_.resumeChain();
	QGraphicsView::closeEvent(event);
}

void ImageWindow::keyReleaseEvent(QKeyEvent *event)
{
	// resume a paused chain on key press
//	mm_.resumeChain();

	UIPF_LOG_TRACE(event->key());
	switch(event->key())
	{
		case 43: // +
			zoom(1.1);
			return;
		case 45: // -
			zoom(0.9);
			return;
		case 48: // 0
			zoomed_ = false;
			fitInView(sceneRect(), Qt::KeepAspectRatio);
			return;
		default:
			QGraphicsView::keyReleaseEvent(event);
	}
}

void ImageWindow::resizeEvent(QResizeEvent *event) {

	// make sure image is visible in the window
	if (!zoomed_) {
		fitInView(sceneRect(), Qt::KeepAspectRatio);
	}

	QGraphicsView::resizeEvent(event);
}

void ImageWindow::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() & Qt::ControlModifier || event->modifiers() & Qt::ShiftModifier) {
		int angle = event->delta();
		if (angle > 0) {
			zoom(1.1);
		} else {
			zoom(0.9);
		}
	} else {
		QGraphicsView::wheelEvent(event);
	}
}

void ImageWindow::zoom(qreal factor){

	// zoom
	const ViewportAnchor anchor = transformationAnchor();
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	scale(factor, factor);
	setTransformationAnchor(anchor);

	zoomed_ = (scene()->width() > width() || scene()->height() > height());
}
