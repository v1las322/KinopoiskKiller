#include "collectionsaddwindow.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "ui_collectionsaddwindow.h"
#include "globals.h"
CollectionsAddWindow::CollectionsAddWindow(const QString &Collection, const QString &login, QWidget *parent)
    : QMainWindow(parent)
    , UserLogin(login)
    , CollectionName(Collection)
    , ui(new Ui::CollectionsAddWindow)
{
    ui->setupUi(this);
    FillingMovieOutput();
    connect(ui->ButtonSearch, &QPushButton::clicked, this, &CollectionsAddWindow::SearchButtonClick);
}

CollectionsAddWindow::~CollectionsAddWindow()
{
    delete ui;
}

int CollectionsAddWindow::GetMovieID(QString MovieName){
    QSqlQuery query(db);
    query.prepare("SELECT MovieID "
                  "FROM Movies "
                  "WHERE Name = ?");
    query.addBindValue(MovieName);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int CollectionsAddWindow::GetCollectionID(){
    QSqlQuery query(db);
    query.prepare("SELECT CollectionID "
                  "FROM Collections "
                  "WHERE Name = ? ");
    query.addBindValue(CollectionName);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

void CollectionsAddWindow::AddMovieCollectins(QString MovieName){
    QSqlQuery query(db);
    query.prepare("INSERT INTO CollectionsMovie (CollectionID, MovieID) VALUES (?, ?)");
    query.addBindValue(GetCollectionID());
    query.addBindValue(GetMovieID(MovieName));
    if (!query.exec()) {
        showMessageBox("Фильм уже в подборке", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    }else {
        SearchButtonClick();
    }

}

void CollectionsAddWindow::SearchButtonClick() {
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

int CollectionsAddWindow::CheckMovieInCollection(QString MovieName){
    QSqlQuery query(db);
    query.prepare("SELECT * "
                  "FROM CollectionsMovie "
                  "WHERE CollectionID = ? AND MovieID = ?");
    query.addBindValue(GetCollectionID());
    query.addBindValue(GetMovieID(MovieName));
    query.exec();

    if(query.next()) {
        return true;
    } else{
       return false;
    }

}

void CollectionsAddWindow::DeleteMovieInCollection(QString MovieName){
    QSqlQuery query(db);
    query.prepare("DELETE FROM CollectionsMovie "
                  "WHERE CollectionID = ? AND MovieID = ?");
    query.addBindValue(GetCollectionID());
    query.addBindValue(GetMovieID(MovieName));
    query.exec();
    SearchButtonClick();
}

void CollectionsAddWindow::FillingMovieOutput() {
    if (!getSqlConnection(db)) {
        showMessageBox("Потеряно соединение с БД", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    } else {
        QSqlQuery query(db);
        if (!ui->SearchEdit->text().isEmpty()) {
            query.prepare("SELECT Movies.*, AVG(GradeMovie.Grade) AS MidleGrade "
                          "FROM Movies "
                          "LEFT JOIN GradeMovie ON Movies.MovieID = GradeMovie.MovieID "
                          "WHERE Movies.Name LIKE CONCAT('%', ?, '%') "
                          "GROUP BY Movies.MovieID");
            query.addBindValue(ui->SearchEdit->text());
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

            QPushButton* button = new QPushButton("Добавить");
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            button->setStyleSheet("QPushButton { margin-left: auto; margin-right: auto; }");
            if(!CheckMovieInCollection(Name)) {
                connect(button, &QPushButton::clicked, this, [Name, this]() {
                    AddMovieCollectins(Name);
                });
            } else {
                button->setText("Удалить");
                connect(button, &QPushButton::clicked, this, [Name, this]() {
                    DeleteMovieInCollection(Name);
                });
            }

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

        }

        ui->Movie->setLayout(scrollLayout);
    }
}
