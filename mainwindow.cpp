#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_cap = new cv::VideoCapture(0); //700
    m_video_timer = new QTimer();
    m_video_timer->setInterval(16);
    m_video_timer->start();
    connect(m_video_timer, &QTimer::timeout, this, &MainWindow::getFrame);
    this->move(20, 20);
}


void MainWindow::getFrame()
{
    using namespace cv;
    if(video_enable)
    {
        *m_cap >> image_clone;
        this->resize(790, 550);
        cvtColor(image_clone, image_clone, COLOR_BGR2RGB);
        image = image_clone.clone();
    }
    else
    {
        this->resize(1435, 550);
        zoom = ui->zoom_value->value();
        int x1, x2, y1, y2;
        if(zoom == 0)
        {
            x1 = 0;
            y1 = 0;
            x2 = 640;
            y2 = 480;
        }
        else
        {
            x1 = zoom_point.x - 160 / zoom;
            y1 = zoom_point.y - 120 / zoom;
            x2 = 320 / zoom;
            y2 = 240 / zoom;
        }
        image = imread("image4.jpg");
        cvtColor(image, image, COLOR_BGR2RGB);
        image_zoom = image.clone();
        image_zoom = image_zoom(cv::Rect(x1, y1, x2, y2));
        cv::rectangle(image, cv::Point(zoom_point.x - 160 / zoom, zoom_point.y - 120 / zoom), cv::Point(zoom_point.x + 160 / zoom, zoom_point.y + 120 / zoom), cv::Scalar(0, 0, 255), 1);
        cv::resize(image_zoom, image_zoom, cv::Size(640, 480));
        cv::rectangle(image, zoom_point, zoom_point, cv::Scalar(0, 255, 0), 2);
        cv::line(image_zoom, first_line_one, first_line_two, cv::Scalar(255, 0, 0), 1);
        cv::line(image_zoom, second_line_one, second_line_two, cv::Scalar(255, 255, 0), 1);
    }
    ui->picture->setPixmap(QPixmap::fromImage(QImage((unsigned char*) image.data, image.cols, image.rows, QImage::Format_RGB888)));
    ui->zoom_picture->setPixmap(QPixmap::fromImage(QImage((unsigned char*) image_zoom.data, image_zoom.cols, image_zoom.rows, QImage::Format_RGB888)));
    float first_line_length = sqrt((abs(first_line_one.x - first_line_two.x) * abs(first_line_one.x - first_line_two.x)) + (abs(first_line_one.y - first_line_two.y) * abs(first_line_one.y - first_line_two.y)));
    float second_line_length = sqrt((abs(second_line_one.x - second_line_two.x) * abs(second_line_one.x - second_line_two.x)) + (abs(second_line_one.y - second_line_two.y) * abs(second_line_one.y - second_line_two.y)));
    float result = (second_line_length * ui->line_value->value()) / first_line_length;
    ui->result->setNum(result);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::MouseButton::LeftButton) {
        if(first_left_click)
        {
            first_line_one.x = e->x() - 790;
            first_line_one.y = e->y();
            first_left_click = false;
        }
        else
        {
            first_line_two.x = e->x() - 790;
            first_line_two.y = e->y();
            first_left_click = true;
        }
    }
    if (e->button() == Qt::MouseButton::RightButton) {
        if(first_right_click)
        {
            second_line_one.x = e->x() - 790;
            second_line_one.y = e->y();
            first_right_click = false;
        }
        else
        {
            second_line_two.x = e->x() - 790;
            second_line_two.y = e->y();
            first_right_click = true;
        }
    }
    if(e->button() == Qt::MouseButton::MiddleButton)
    {
        zoom_point.x = e->x();
        zoom_point.y = e->y();
        if(e->x() - 160 / zoom < 0)
        {
            zoom_point.x = 0 + 160 / zoom;
        }
        else if(e->x() + 160 / zoom > 640)
        {
            zoom_point.x = 640 - 160 / zoom;
        }
        else
        {
            zoom_point.x = e->x();
        }
        if(e->y() - 120 / zoom < 0)
        {
            zoom_point.y = 0 + 120 / zoom;
        }
        else if(e->y() + 120 / zoom > 480)
        {
            zoom_point.y = 480 - 120 / zoom;
        }
        else
        {
            zoom_point.y = e->y();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reset_button_clicked()
{
    first_line_one = {0,0};
    first_line_two = {0,0};
    second_line_one = {0,0};
    second_line_two = {0,0};
    first_left_click = true;
    first_right_click = true;
}

void MainWindow::on_stop_button_clicked()
{
    cv::imwrite("image.jpg", image_clone);
    video_enable = false;
}

void MainWindow::on_strat_button_clicked()
{
    video_enable = true;
}
