#pragma once

#include "avs/core/execution/ExecutionController.hpp"

#include <QMainWindow>

#include <memory>

class QLabel;
class QPushButton;

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
        void setupUi();
        void setupController();
        void connectSignals();

        void refreshView();
        void updateVisualizationFromCurrentStep();

    private:
        std::unique_ptr<::avs::core::execution::ExecutionController> controller_;

        widgets::ArrayVisualizationWidget* arrayWidget_ = nullptr;
        QLabel* statusLabel_ = nullptr;

        QPushButton* previousButton_ = nullptr;
        QPushButton* nextButton_ = nullptr;
        QPushButton* resetButton_ = nullptr;
    };
}
