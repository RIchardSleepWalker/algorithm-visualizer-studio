#include "avs/ui/MainWindow.hpp"

namespace avs::ui
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        setWindowTitle("Algorithm Visualizer Studio");
        resize(1000, 700);
    }
}
