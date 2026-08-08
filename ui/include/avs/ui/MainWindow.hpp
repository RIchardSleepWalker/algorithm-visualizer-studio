#pragma once

#include <QMainWindow>

namespace avs::ui
{
    class MainWindow final : public QMainWindow
    {
    public:
        explicit MainWindow(QWidget* parent = nullptr);
    };
}
