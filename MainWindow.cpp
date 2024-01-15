#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <stdexcept>
#include <string>

#include "ChainCodeNoise.hpp"
#include "MainWindow.hpp"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_MaxCoordinate(0)
{
    m_Ui.setupUi(this);

    // Connecting signals to slots.
    QObject::connect(m_Ui.btn_LoadChainCode, &QPushButton::released, this, &MainWindow::loadChainCode);     // Loading chain code.
    QObject::connect(m_Ui.btnAddNoise, &QPushButton::released, this, &MainWindow::applyNoiseToChainCodes);  // Applying noise to chain codes.
}

MainWindow::~MainWindow()
{}



void MainWindow::loadChainCode() {
	// Creating a file dialog to choose the file that contains a chain code and setting the file name as the text in a text box.
	const std::string file = QFileDialog::getOpenFileName(this, "Load chain code", "./Datasets", "TXT files (*.txt)|*.txt").toStdString();
    m_Ui.tbxChainCodeFile->setText(QString::fromStdString(file.substr(file.rfind('/') + 1)));
    
    // If our beloved user is acting weird and cancels the dialog,
    // Freddie Mercury sings: "Goodbye everybody, I gotta go!".
    if (file.empty()) {
        return;
    }

    // Reading the chain code file and rendering.
    m_ChainCodes = readChainCodeFile(file);
    renderChainCodes(m_ChainCodes);
    m_ChainCodeNoise = ChainCodeNoise(m_ChainCodes);

    // Enabling noise group box.
    m_Ui.gbxNoise->setEnabled(true);
}

void MainWindow::applyNoiseToChainCodes() {
    // Reading input parameters.
    const double noiseProbability = m_Ui.sbxNoiseProbability->value() / 100.0;
    const uint numberOfIterations = m_Ui.sbxNumberOfIterations->value();
    std::string name = m_Ui.tbxChainCodeFile->text().toStdString();
    name = name.substr(0, name.rfind('.'));

    // Adding noise to each chain code.
    m_ChainCodes = m_ChainCodeNoise.applyNoise(m_ChainCodes, m_StartPixels, m_BorderPixels, noiseProbability, numberOfIterations, name);

    // Rerendering of noisy chain codes.
    renderChainCodes(m_ChainCodes);
}



std::vector<ChainCode> MainWindow::readChainCodeFile(const std::string& file) {
    std::vector<ChainCode> chainCodes;

    // Opening a file.
    std::ifstream in(file);

    // If a file is not open, we do not panic but abort the process.
    if (!in.is_open()) {
        throw std::logic_error("File could not be found or opened.");
    }

    // Reading the header.
    std::string firstLine;
    std::getline(in, firstLine);
    if (firstLine != "CC Multi") {
        throw std::logic_error("Invalid header of the chain code.");
    }

    // Reading the chain code.
    std::string value;
    while (std::getline(in, value, ';')) {
        ChainCodeType type;
        if (value == "F8") {
            type = ChainCodeType::F8;
        }
        else if (value == "F4") {
            type = ChainCodeType::F4;
        }
        else {
            throw std::logic_error("Unknown type of the chain code.");
        }

        // Reading clockwise or anti-clockwise orientation.
        std::getline(in, value, ';');

        // Reading the starting point.
        std::getline(in, value, ',');
        const int startX = std::stoi(value);
        std::getline(in, value, ';');
        const int startY = -std::stoi(value);
        std::getline(in, value, ';');

        // Reading the chain code.
        std::getline(in, value);
        chainCodes.push_back(ChainCode(value, type, startX, startY));
    }

    // Number of segments calculation.
    uint segmentCount = 0;
    for (const ChainCode& chainCode : chainCodes) {
        segmentCount += chainCode.code.size();
    }
    std::cout << "[INFO] Segment count: " << segmentCount << std::endl;

    return chainCodes;
}

void MainWindow::renderChainCodes(const std::vector<ChainCode>& chainCodes) {
    // Transforming chain codes into coordinates.
    const auto [coordinates, maxXCoordinate, maxYCoordinate] = ChainCodeFunctions::calculateCoordinates(chainCodes);

    // Inserting start pixels into a vector.
    m_StartPixels.clear();
    for (uint i = 0; i < coordinates.size(); i++) {
        m_StartPixels.push_back(coordinates[i][0]);
    }

    // Transforming border pixels into a hash table.
    m_BorderPixels = ChainCodeFunctions::coordinatesToSet(coordinates, maxXCoordinate);

    // Drawing the coordinates on PixMap.
    const uint scale = 2;
    const uint padding = 50;
    QPixmap pix(scale * (maxXCoordinate + 2 * padding), scale * (maxYCoordinate + 2 * padding));
    pix.fill(QColor("white"));
    QPainter paint(&pix);
    paint.setPen(QColor(0, 0, 0, 255));
    for (const std::vector<Pixel>& currentCoordinates : coordinates) {
        for (const Pixel& coordinate : currentCoordinates) {
            paint.drawRect(scale * (coordinate.x + padding), scale * (maxYCoordinate - (coordinate.y) + padding), 1, 1);
        }
    }

    // Drawing the bounding box of the object.
    //paint.setPen(QPen(Qt::black, 3));
    //paint.drawLine(scale * padding, scale * padding, scale * maxXCoordinate + scale * padding, scale * padding);
    //paint.drawLine(scale * maxXCoordinate + scale * padding, scale * padding, scale * maxXCoordinate + scale * padding, scale * maxYCoordinate + scale * padding);
    //paint.drawLine(scale * maxXCoordinate + scale * padding, scale * maxYCoordinate + scale * padding, scale * padding, scale * maxYCoordinate + scale * padding);
    //paint.drawLine(scale * padding, scale * maxYCoordinate + scale * padding, scale * padding, scale * padding);

    // Displaying the PixMap.
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(pix);
    m_Ui.imgChainCode->setScene(scene);
    m_Ui.imgChainCode->show();
}