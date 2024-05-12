#include "movieshowwindow.h"
#include "ui_movieshowwindow.h"
#include "globals.h"
#include <QStyledItemDelegate>
#include <QTableWidget>

MovieShowWindow::MovieShowWindow(const QString &name, const QString &login, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MovieShowWindow)
    , MovieName(name)
    , UserLogin(login)
{
    ui->setupUi(this);
    OutPutMovieInfo();
    SetAvgGrade();
    ValueButtonsGrade();
    ValueButtonsReview();
    OutPutReview();
    connect(ui->ButtonDelete, &QPushButton::clicked, this, &MovieShowWindow::DeleteReview);
    connect(ui->ButtonDeleteGrade, &QPushButton::clicked, this, &MovieShowWindow::DeleteGrade);
}

MovieShowWindow::~MovieShowWindow()
{
    delete ui;
}

void MovieShowWindow::ValueButtonsGrade() {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM GradeMovie WHERE MovieID = ? AND UserID = ?");
    query.addBindValue(GetMovieID());
    query.addBindValue(GetUserID());
    query.exec();
    if (query.next()) {
        ui->ButtonGrade->setText("Обновить");
        disconnect(ui->ButtonGrade, &QPushButton::clicked, this, &MovieShowWindow::InsertGrade);
        connect(ui->ButtonGrade, &QPushButton::clicked, this, &MovieShowWindow::UpdateGrade);
    } else {
        ui->ButtonGrade->setText("Оценить");
        disconnect(ui->ButtonGrade, &QPushButton::clicked, this, &MovieShowWindow::UpdateGrade);
        connect(ui->ButtonGrade, &QPushButton::clicked, this, &MovieShowWindow::InsertGrade);
    }
}

void MovieShowWindow::ValueButtonsReview(){
    QSqlQuery query(db);
    query.prepare("SELECT * FROM Review WHERE MovieID = ? AND UserID = ?");
    query.addBindValue(GetMovieID());
    query.addBindValue(GetUserID());
    query.exec();
    if (query.next()) {
        ui->ButtonReview->setText("Обновить рецензию");
        disconnect(ui->ButtonReview, &QPushButton::clicked, this, &MovieShowWindow::InsertReview);
        connect(ui->ButtonReview, &QPushButton::clicked, this, &MovieShowWindow::UpdateReview);
    } else {
        ui->ButtonReview->setText("Отправить рецензию");
        disconnect(ui->ButtonReview, &QPushButton::clicked, this, &MovieShowWindow::UpdateReview);
        connect(ui->ButtonReview, &QPushButton::clicked, this, &MovieShowWindow::InsertReview);
    }
}

void MovieShowWindow::UpdateReview() {
    if(!ui->InputReview->toPlainText().isEmpty()) {
        QSqlQuery query(db);
        query.prepare("UPDATE Review SET TextReview = ? WHERE UserID = ? AND MovieID = ?");
        query.addBindValue(ui->InputReview->toPlainText());
        query.addBindValue(GetUserID());
        query.addBindValue(GetMovieID());
        if(!query.exec()) {
            showMessageBox("Не удалось обновить ревью", "Действие прервано",
                           QMessageBox::Ok, QMessageBox::Warning);
        } else {
        OutPutReview();
        ValueButtonsReview();
        }
    } else {
        showMessageBox("Поля ревью не должно быть пустым", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Warning);
    }
}

void MovieShowWindow::DeleteReview(){
    QSqlQuery query(db);
    query.prepare("DELETE FROM Review WHERE UserID = ? AND MovieID = ?");
    query.addBindValue(GetUserID());
    query.addBindValue(GetMovieID());
    if(query.exec()) {
        OutPutReview();
        ValueButtonsReview();
    }
}

void MovieShowWindow::InsertReview() {
    if(!ui->InputReview->toPlainText().isEmpty()) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO Review (UserID, MovieID, TextReview) VALUES (?, ?, ?)");
        query.addBindValue(GetUserID());
        query.addBindValue(GetMovieID());
        query.addBindValue(ui->InputReview->toPlainText());
        if(query.exec()) {
            OutPutReview();
            ValueButtonsReview();
        }
    }else {
        showMessageBox("Поля ревью не должно быть пустым", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Warning);
    }
}

void MovieShowWindow::SetAvgGrade() {
    QSqlQuery query(db);
    query.prepare("SELECT AVG(Grade) FROM GradeMovie WHERE MovieID = ?");
    query.addBindValue(GetMovieID());
    query.exec();
    query.next();
    ui->AvgGrade->setText("Средняя оценка: " + query.value(0).toString());
}

void MovieShowWindow::UpdateGrade(){
    QSqlQuery query(db);
    query.prepare("UPDATE GradeMovie SET Grade = ? WHERE UserID = ? AND MovieID = ?");
    query.addBindValue(ui->UserGrade->value());
    query.addBindValue(GetUserID());
    query.addBindValue(GetMovieID());
    if(!query.exec()) {
        showMessageBox("Не удалось Обновить оценку", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Warning);
    } else {
        SetAvgGrade();
        ValueButtonsGrade();
    }
}

void MovieShowWindow::OutPutMovieInfo() {
    db = QSqlDatabase::database("MoviesShow");
    if (getSqlConnection(db)) {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM Movies WHERE Name = ?");
        query.addBindValue(MovieName);
        query.exec();
        query.next();
        QString imagePath = QCoreApplication::applicationDirPath() + query.value("PathImage").toString();
        QPixmap originalPixmap(imagePath);
        QPixmap scaledPixmap = originalPixmap.scaled(211, 281, Qt::KeepAspectRatio);
        ui->ImageMovie->setPixmap(scaledPixmap);
        ui->MovieName->setText(MovieName);
        ui->DescriptionMovie->setText(query.value("Description").toString());
    } else {
        showMessageBox("Нет связи", "Действие прервано",
                       QMessageBox::Ok, QMessageBox::Warning);
        this->close();
    }

}

void MovieShowWindow::OutPutReview() {
    if (getSqlConnection(db)) {
        QSqlQuery query(db);
        query.prepare("SELECT Users.Username, Review.TextReview "
                      "FROM Users JOIN Review ON Users.UserID = Review.UserID "
                      "WHERE Review.MovieID = ?;");
        query.addBindValue(GetMovieID());
        query.exec();
        QStandardItemModel *model = new QStandardItemModel();
        int columnCount = 2;
        model->setColumnCount(columnCount);
        QStringList headers = {"Пользователь", "Ревью"};
        model->setHorizontalHeaderLabels(headers);
        int row = 0;
        while (query.next()) {
            for (int column = 0; column < columnCount; column++) {
                QString cellValue = query.value(column).toString();
                QStandardItem *item = new QStandardItem(cellValue);
                model->setItem(row, column, item);
            }
            ui->OtherReview->setModel(model);
            row++;
        }
        ui->OtherReview->setModel(model);
        ui->OtherReview->resizeRowsToContents();
        ui->OtherReview->resizeColumnsToContents();
    } else {
        showMessageBox("Потеряно соединение с БД", "Неудачно", QMessageBox::Ok,
                       QMessageBox::Warning);
    }
}

int MovieShowWindow::GetUserID(){
    QSqlQuery query(db);
    query.prepare("SELECT UserID FROM Users WHERE Username = ?");
    query.addBindValue(UserLogin);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int MovieShowWindow::GetMovieID(){
    QSqlQuery query(db);
    query.prepare("SELECT MovieID FROM Movies WHERE Name = ?");
    query.addBindValue(MovieName);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

void MovieShowWindow::DeleteGrade(){
    QSqlQuery query(db);
    query.prepare("DELETE FROM GradeMovie WHERE UserID = ? AND MovieID = ?");
    query.addBindValue(GetUserID());
    query.addBindValue(GetMovieID());
    if(query.exec()) {
        SetAvgGrade();
        ValueButtonsGrade();
    }
}

void MovieShowWindow::InsertGrade() {
    QSqlQuery query(db);
    query.prepare("INSERT INTO GradeMovie (UserID, MovieID, Grade) VALUES (?, ?, ?)");
    query.addBindValue(GetUserID());
    query.addBindValue(GetMovieID());
    query.addBindValue(ui->UserGrade->value());
    if(query.exec()) {
        SetAvgGrade();
        ValueButtonsGrade();
    }
}


