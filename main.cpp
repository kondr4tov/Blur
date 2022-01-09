#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QSlider>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QDir>
#include <QLabel>
#include <QFileDialog>


QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent=0)
{
    if(src.isNull()) return QImage();   //No need to do anything else!
    if(!effect) return src;             //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}

void makeBlurEffect(int value, QString &filepath) {
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(value);
    QImage source(filepath);
    QImage result = applyEffectToImage(source, blur);
    result.save("/Users/andrejkondratov/CLionProjects/html_to_web/blur.jpeg");
}



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget mainWindow;

    QVBoxLayout vbox(&mainWindow);
    QSlider *blurSlider = new QSlider(&mainWindow);
    blurSlider->setOrientation(Qt::Horizontal);
    blurSlider->setMinimum(0);
    blurSlider->setMaximum(100);

    vbox.addWidget(blurSlider);

    auto *button = new QPushButton(&mainWindow);
    button->setText("Choose image");
    vbox.addWidget(button);

    QString filepath;
    QString blurFilePath = "/Users/andrejkondratov/CLionProjects/html_to_web/blur.jpeg";

    QLabel* label = new QLabel;
    auto *image = new QPixmap(blurFilePath);
    label->setScaledContents(true);
    label->setFixedSize(500,600);
    vbox.addWidget(label);


    QObject::connect(button, &QPushButton::clicked, [&filepath, &vbox, &mainWindow, &label]()
    {
        filepath = QFileDialog::getOpenFileName(nullptr);

        auto *image = new QPixmap(filepath);
        label->setPixmap(*image);
        mainWindow.update();
    });

    QObject::connect(blurSlider, &QSlider::valueChanged, [&blurSlider, &filepath, &mainWindow, &blurFilePath, &label]()
    {
        makeBlurEffect(blurSlider->value(), filepath);
        auto *image = new QPixmap(blurFilePath);
        label->setPixmap(*image);
        mainWindow.update();
    });

    mainWindow.show();
    return QApplication::exec();
}
