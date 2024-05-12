#ifndef MOVIESHOWWINDOW_H
#define MOVIESHOWWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>

namespace Ui {
class MovieShowWindow;
}

class MovieShowWindow : public QMainWindow
{
    Q_OBJECT

public:
    MovieShowWindow(const QString &name, const QString &login, QWidget *parent = nullptr);
    ~MovieShowWindow();
private:
    void ValueButtonsGrade();
    void ValueButtonsReview();
    void UpdateGrade();
    void SetAvgGrade();
    void InsertGrade();
    void DeleteGrade();
    void DeleteReview();
    void OutPutReview();
    void UpdateReview();
    void InsertReview();
    Ui::MovieShowWindow *ui;
    QString MovieName;
    QString UserLogin;
    int GetUserID();
    int GetMovieID();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "MoviesShow");
    void OutPutMovieInfo();
};

#endif // MOVIESHOWWINDOW_H
