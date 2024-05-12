#ifndef COLLECTIONSADDWINDOW_H
#define COLLECTIONSADDWINDOW_H

#include "qsqldatabase.h"
#include <QMainWindow>

namespace Ui {
class CollectionsAddWindow;
}

class CollectionsAddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CollectionsAddWindow(const QString &Collection, const QString &login, QWidget *parent = nullptr);
    ~CollectionsAddWindow();

private:
    void FillingMovieOutput();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "Collections");
    void AddMovieCollectins(QString MovieName);
    QString UserLogin;
    QString CollectionName;
    void SearchButtonClick();
    int GetMovieID(QString MovieName);
    int GetCollectionID();
    int CheckMovieInCollection(QString MovieName);
    void DeleteMovieInCollection(QString MovieName);
    Ui::CollectionsAddWindow *ui;
};

#endif // COLLECTIONSADDWINDOW_H
