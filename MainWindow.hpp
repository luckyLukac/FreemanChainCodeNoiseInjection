#pragma once

#include <QtWidgets/QMainWindow>
#include <unordered_set>

#include "ChainCode.hpp"
#include "ChainCodeNoise.hpp"
#include "ui_MainWindow.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

// PRIVATE VARIABLES
private:
    Ui::ChainCodeNoiseClass m_Ui;              // Qt GUI object.
    std::vector<ChainCode> m_ChainCodes;       // Vector of chain codes.
    uint m_MaxCoordinate;                      // Maximal coordinate.
    std::unordered_set<Pixel> m_BorderPixels;  // Hash table of border pixels.
    std::vector<Pixel> m_StartPixels;          // Vector of starting pixels of chain codes.
    ChainCodeNoise m_ChainCodeNoise;           // Chain code algorithm object.

// SLOTS
private slots:
    /// <summary>
    /// Loading of a chain code.
    /// </summary>
    void loadChainCode();

    /// <summary>
    /// Applying noise to chain codes, stored in the object.
    /// </summary>
    void applyNoiseToChainCodes();

// PRIVATE METHODS
private:
    /// <summary>
    /// Reading a file that contains chain codes.
    /// </summary>
    /// <param name="file">: path to the file</param>
    /// <returns>Vector of chain codes</returns>
    std::vector<ChainCode> readChainCodeFile(const std::string& file);

    /// <summary>
    /// Rendering of chain codes as the image.
    /// </summary>
    /// <param name="chainCodes">: vector of chain codes</param>
    void renderChainCodes(const std::vector<ChainCode>& chainCodes);

// PUBLIC METHODS
public:
    /// <summary>
    /// Constructor of the graphical user interface of the app.
    /// </summary>
    /// <param name="parent">: parent widget</param>
    MainWindow(QWidget* parent = nullptr);

    /// <summary>
    /// Destructor of the graphical user interface.
    /// </summary>
    ~MainWindow();
};