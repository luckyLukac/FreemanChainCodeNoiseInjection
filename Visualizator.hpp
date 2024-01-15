#pragma once

#include <QGraphicsView>


// CONSTANTS
const double MIN_SCALE = 0.4;
const double MAX_SCALE = 10.0;


/// <summary>
/// Enhanced QGraphicsView with translation and zoom features.
/// </summary>
class Visualizator : public QGraphicsView {
private:
    bool m_MouseDown = false;   // True if left mouse button is pressed.
    double m_MouseX = 0.0;      // Mouse X position.
    double m_MouseY = 0.0;      // Mouse Y position.
    double m_Scale = 1.0;       // Current scale.


private:
    /// <summary>
    /// Event that occurs on mouse button press.
    /// </summary>
    /// <param name="event">: QMouseEvent</param>
    void mousePressEvent(QMouseEvent* event);

    /// <summary>
    /// Event that occurs on mouse button release.
    /// </summary>
    /// <param name="event">: QMouseEvent</param>
    void mouseReleaseEvent(QMouseEvent* event);

    /// <summary>
    /// Event that occurs on mouse move.
    /// </summary>
    /// <param name="event">: QMouseEvent</param>
    void mouseMoveEvent(QMouseEvent* event);

    /// <summary>
    /// Event that occurs on wheel change (zoom).
    /// </summary>
    /// <param name="event">: QWheelEvent</param>
    void wheelEvent(QWheelEvent* event);

public:
    /// <summary>
    /// Constructor of the Visualizator widget.
    /// </summary>
    /// <param name="parent">: QGraphicsView parent widget</param>
    Visualizator(QWidget* parent);
};

