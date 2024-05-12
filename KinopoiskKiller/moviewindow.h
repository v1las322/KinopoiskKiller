#ifndef MOVIEWINDOW_H
#define MOVIEWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QLabel>
#include <QHBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui {
class MovieWindow;
}
QT_END_NAMESPACE

class MovieWindow : public QMainWindow
{
    Q_OBJECT

public:
    MovieWindow(const QString &login, QWidget *parent = nullptr);
    ~MovieWindow();

private slots:
    void on_CollectionsBox_currentTextChanged();

private:
    Ui::MovieWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "movies");
    QString UserLogin;
    void SearchButtonClick();
    void OutPutReview();
    int GetCollectionID(QString MovieName);
    void OutputCollections();
    void ButtonConnects();
    void DeleteFromCollection(int MovieID);
    void ButtonDeleteCollection();
    void SetNameCollectionsInBox();
    void ButtonAddCollection();
    void ButtonAddMovieInCollection();
    int GetUserID();
    void ProfileFilling();
    void onButtonClicked(const QString& movieName);
    void FillingMovieOutput();
    void CreateOutput(QString imagePath, QGridLayout *scrollLayout, int rowCount, QString Name, int columnCount, QString Rating);
};
#endif // MOVIEWINDOW_H
