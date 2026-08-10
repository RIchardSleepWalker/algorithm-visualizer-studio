#pragma once

#include <QMainWindow>

namespace avs::ui::widgets
{
    class ArrayVisualizationWidget;
}

namespace avs::ui
{
    class MainWindow final : public QMainWindow
    {
    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        widgets::ArrayVisualizationWidget* arrayWidget_ = nullptr;
    };
}
