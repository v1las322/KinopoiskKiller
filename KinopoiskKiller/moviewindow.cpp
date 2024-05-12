#include "moviewindow.h"
#include "collectionsaddwindow.h"
#include "ui_moviewindow.h"
#include <QImageReader>
#include <QScrollArea>
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include "globals.h"
#include "movieshowwindow.h"
MovieWindow::MovieWindow(const QString &login, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MovieWindow), UserLogin(login)
{
    ui->setupUi(this);
    FillingMovieOutput();
    ButtonConnects();
    ProfileFilling();
    OutputCollections();
}

MovieWindow::~MovieWindow()
{
    delete ui;
}

void MovieWindow::FillingMovieOutput() {
    if (!getSqlConnection(db)) {
        showMessageBox("Потеряно соединение с БД", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    } else {
        QSqlQuery query(db);
        if (!ui->Search_Edit->text().isEmpty()) {
            query.prepare("SELECT Movies.*, AVG(GradeMovie.Grade) AS MidleGrade "
                          "FROM Movies "
                          "LEFT JOIN GradeMovie ON Movies.MovieID = GradeMovie.MovieID "
                          "WHERE Movies.Name LIKE CONCAT('%', ?, '%') "
                          "GROUP BY Movies.MovieID");
            query.addBindValue(ui->Search_Edit->text());
        } else {
            query.prepare("SELECT Movies.*, AVG(GradeMovie.Grade) AS MidleGrade "
                          "FROM Movies "
                          "LEFT JOIN GradeMovie ON Movies.MovieID = GradeMovie.MovieID "
                          "GROUP BY Movies.MovieID");
        }
        query.exec();
        QWidget* scrollWidget = new QWidget;
        QGridLayout* scrollLayout = new QGridLayout(scrollWidget);
        int columnCount = 0;
        int rowCount = 0;
        while (query.next()) {

            QString imagePath = QCoreApplication::applicationDirPath() + query.value("PathImage").toString();
            QString Name = query.value("Name").toString();
            QString Rating = query.value("MidleGrade").toString();

            QWidget* widget = new QWidget;
            QVBoxLayout* widgetLayout = new QVBoxLayout(widget);
            QLabel* imageLabel = new QLabel;
            QLabel* nameLabel = new QLabel;
            QLabel* ratingLabel = new QLabel;
            QPushButton* button = new QPushButton("Открыть");
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            button->setStyleSheet("QPushButton { margin-left: auto; margin-right: auto; }");
            QPixmap originalPixmap(imagePath);
            QPixmap scaledPixmap = originalPixmap.scaled(150, 225, Qt::KeepAspectRatio);
            imageLabel->setPixmap(scaledPixmap);
            nameLabel->setText(Name);
            ratingLabel->setText("Средняя оценка: " + Rating);

            widgetLayout->addWidget(imageLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
            widgetLayout->addWidget(nameLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
            widgetLayout->addWidget(ratingLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
            widgetLayout->addWidget(button, 0, Qt::AlignTop | Qt::AlignHCenter);
            widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
            scrollLayout->addWidget(widget, rowCount, columnCount, Qt::AlignTop | Qt::AlignLeft);

            columnCount++;
            if (columnCount == 5) {
                columnCount = 0;
                rowCount++;
            }
            connect(button, &QPushButton::clicked, this, [Name, this]() {
                onButtonClicked(Name);
            });
        }

        ui->Movie->setLayout(scrollLayout);
    }
}

void MovieWindow::OutputCollections(){
    QLayout* layout = ui->CollectionMovie->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    QSqlQuery query(db);
    query.prepare("SELECT Movies.*, AVG(GradeMovie.Grade) AS MidleGrade "
                  "FROM Movies "
                  "JOIN CollectionsMovie ON Movies.MovieID = CollectionsMovie.MovieID "
                  "JOIN Collections ON Collections.CollectionID = CollectionsMovie.CollectionID "
                  "LEFT JOIN GradeMovie ON Movies.MovieID = GradeMovie.MovieID "
                  "WHERE Collections.Name = :CollectionName "
                  "GROUP BY Movies.MovieID");
    query.addBindValue(ui->CollectionsBox->currentText());
    query.exec();
    QWidget* scrollWidget = new QWidget;
    QGridLayout* scrollLayout = new QGridLayout(scrollWidget);
    int columnCount = 0;
    int rowCount = 0;
    while (query.next()) {

        QString imagePath = QCoreApplication::applicationDirPath() + query.value("PathImage").toString();
        QString Name = query.value("Name").toString();
        QString Rating = query.value("MidleGrade").toString();
        int MovieID = query.value("MovieID").toInt();
        QWidget* widget = new QWidget;
        QVBoxLayout* widgetLayout = new QVBoxLayout(widget);
        QLabel* imageLabel = new QLabel;
        QLabel* nameLabel = new QLabel;
        QLabel* ratingLabel = new QLabel;
        QPushButton* button = new QPushButton("Удалить");
        button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        button->setStyleSheet("QPushButton { margin-left: auto; margin-right: auto; }");
        QPixmap originalPixmap(imagePath);
        QPixmap scaledPixmap = originalPixmap.scaled(150, 225, Qt::KeepAspectRatio);
        imageLabel->setPixmap(scaledPixmap);
        nameLabel->setText(Name);
        ratingLabel->setText("Средняя оценка: " + Rating);

        widgetLayout->addWidget(imageLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
        widgetLayout->addWidget(nameLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
        widgetLayout->addWidget(ratingLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
        widgetLayout->addWidget(button, 0, Qt::AlignTop | Qt::AlignHCenter);
        widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
        scrollLayout->addWidget(widget, rowCount, columnCount, Qt::AlignTop | Qt::AlignLeft);

        columnCount++;
        if (columnCount == 5) {
            columnCount = 0;
            rowCount++;
        }
        connect(button, &QPushButton::clicked, this, [MovieID, this]() {
            DeleteFromCollection(MovieID);
        });
    }

    ui->CollectionMovie->setLayout(scrollLayout);
}

void MovieWindow::DeleteFromCollection(int MovieID){
    QSqlQuery query(db);
    query.prepare("DELETE FROM CollectionsMovie "
                  "WHERE CollectionID = ? AND MovieID = ?");
    query.addBindValue(GetCollectionID(ui->CollectionsBox->currentText()));
    query.addBindValue(MovieID);
    query.exec();
    OutputCollections();
}

void MovieWindow::ButtonConnects() {
    connect(ui->Search_Button, &QPushButton::clicked, this, &MovieWindow::SearchButtonClick);
    connect(ui->ButtonAddCollection, &QPushButton::clicked, this, &MovieWindow::ButtonAddCollection);
    connect(ui->ButtonDeleteCollection, &QPushButton::clicked, this, &MovieWindow::ButtonDeleteCollection);
    connect(ui->ButtonAddMovie, &QPushButton::clicked, this, &MovieWindow::ButtonAddMovieInCollection);
    SetNameCollectionsInBox();
}

int MovieWindow::GetCollectionID(QString Name){
    QSqlQuery query(db);
    query.prepare("SELECT CollectionID "
                  "FROM Collections "
                  "WHERE Name = ? ");
    query.addBindValue(Name);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

void MovieWindow::ButtonDeleteCollection(){
    if(!ui->CollectionsBox->currentText().isEmpty()){
        QSqlQuery query(db);
        query.prepare("DELETE FROM CollectionsMovie WHERE CollectionID = ?");
        query.addBindValue(GetCollectionID(ui->CollectionsBox->currentText()));
        query.exec();

        query.prepare("DELETE FROM Collections WHERE UserID = ? AND Name = ?");
        query.addBindValue(GetUserID());
        query.addBindValue(ui->CollectionsBox->currentText());
        query.exec();

        query.finish();
        SetNameCollectionsInBox();
    }
}

void MovieWindow::ButtonAddMovieInCollection(){
    if(!ui->CollectionsBox->currentText().isEmpty()){
        CollectionsAddWindow *movieCollectionShow = new CollectionsAddWindow(ui->CollectionsBox->currentText(), UserLogin);
        this->hide();
        movieCollectionShow->show();
        while (movieCollectionShow->isVisible()) {
            QApplication::processEvents();
        }
        OutputCollections();
        this->show();
    }
}

void MovieWindow::SetNameCollectionsInBox() {
    ui->CollectionsBox->clear();
    QSqlQuery query(db);
    query.prepare("SELECT Name FROM Collections WHERE UserID = ?");
    query.addBindValue(GetUserID());
    query.exec();
    while (query.next()) {
        ui->CollectionsBox->addItem(query.value(0).toString());
    }
}

void MovieWindow::ButtonAddCollection(){
    if(ui->NameCollectionEdit->text().isEmpty()){
        showMessageBox("Поле название подборки не должно быть пустым", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    }else {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Collections (UserID, Name) "
                      "VALUES (?, ?)");
        query.addBindValue(GetUserID());
        query.addBindValue(ui->NameCollectionEdit->text());
        if(!query.exec()){
            showMessageBox("Подборка с таким названием уже существует", "Неудачно", QMessageBox::Ok,
                           QMessageBox::Warning);
        } else {
          query.finish();
          SetNameCollectionsInBox();
        }
    }
}

void MovieWindow::SearchButtonClick() {
    QLayout* layout = ui->Movie->layout();
    if (layout) {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    FillingMovieOutput();
}

void MovieWindow::onButtonClicked(const QString& movieName) {
    MovieShowWindow *movieShowForm = new MovieShowWindow(movieName, UserLogin);
    this->hide();
    movieShowForm->show();
    while (movieShowForm->isVisible()) {
        QApplication::processEvents();
    }
    ProfileFilling();
    SearchButtonClick();
    this->show();
}

void MovieWindow::ProfileFilling() {
    QSqlQuery query(db);
    query.prepare("SELECT Email "
                  "FROM Users "
                  "WHERE Username = ?;");
    query.addBindValue(UserLogin);
    query.exec();
    query.next();
    ui->User_Label->setText("Пользователь: " + UserLogin);
    ui->Mail_Label->setText("Почта: " + query.value(0).toString());
    OutPutReview();
}

void MovieWindow::OutPutReview() {
    if (getSqlConnection(db)) {
        QSqlQuery query(db);
        query.prepare("SELECT Movies.Name, Review.TextReview "
                      "FROM Movies JOIN Review ON Movies.MovieID = Review.MovieID "
                      "WHERE Review.UserID = ?;");
        query.addBindValue(GetUserID());
        query.exec();
        QStandardItemModel *model = new QStandardItemModel();
        int columnCount = 2;
        model->setColumnCount(columnCount);
        QStringList headers = {"Фильм", "Ревью"};
        model->setHorizontalHeaderLabels(headers);
        int row = 0;
        while (query.next()) {
            for (int column = 0; column < columnCount; column++) {
                QString cellValue = query.value(column).toString();
                QStandardItem *item = new QStandardItem(cellValue);
                model->setItem(row, column, item);
            }
            ui->User_Review_Table->setModel(model);
            row++;
        }
        ui->User_Review_Table->setModel(model);
        ui->User_Review_Table->resizeRowsToContents();
        ui->User_Review_Table->resizeColumnsToContents();
    } else {
        showMessageBox("Потеряно соединение с БД", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    }
}

int MovieWindow::GetUserID(){
    QSqlQuery query(db);
    query.prepare("SELECT UserID FROM Users WHERE Username = ?");
    query.addBindValue(UserLogin);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

void MovieWindow::on_CollectionsBox_currentTextChanged()
{
    OutputCollections();
}

