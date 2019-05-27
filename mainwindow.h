#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPixmap>
#include <QCamera>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getFrame();
    void mousePressEvent(QMouseEvent *e);
private slots:
    void on_reset_button_clicked();

    void on_stop_button_clicked();

    void on_strat_button_clicked();

private:
    int zoom = 0;
    bool video_enable = true;
    cv::Mat image;
    cv::Mat image_clone;
    cv::Mat image_zoom;
    cv::Point first_line_one;
    cv::Point first_line_two;
    cv::Point second_line_one;
    cv::Point second_line_two;
    cv::Point zoom_point = {320, 240};
    bool first_left_click = true;
    bool first_right_click = true;
    cv::VideoCapture *m_cap;
    QTimer *m_video_timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
