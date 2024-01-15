#include <QScrollBar>
#include <QMessageBox>
#include <QWheelEvent>

#include "Visualizator.hpp"


Visualizator::Visualizator(QWidget* parent) : QGraphicsView(parent)
{}


void Visualizator::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_MouseDown = true;
        m_MouseX = event->x();
        m_MouseY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
}

void Visualizator::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_MouseDown = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void Visualizator::mouseMoveEvent(QMouseEvent* event) {
    if (m_MouseDown) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_MouseX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_MouseY));
        m_MouseX = event->x();
        m_MouseY = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void Visualizator::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        if (m_Scale <= MAX_SCALE) {
            m_Scale += 0.1;
        }
    }
    else {
        if (m_Scale >= MIN_SCALE) {
            m_Scale -= 0.1;
        }
    }

    setTransform(QTransform().scale(m_Scale, m_Scale));
}