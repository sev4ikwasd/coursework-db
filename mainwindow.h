#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void filterStudents();
    void clearFilterStudents();
    void addStudent();
    void removeStudent();
    void setCurrentStudentIndex(const QModelIndex &index);
    void addStudentClass();
    void removeStudentClass();
    void addStudentWork();
    void removeStudentWork();
    void acceptStudentsChanges();
    void cancelStudentsChanges();

    void filterTeachers();
    void clearFilterTeachers();
    void addTeacher();
    void removeTeacher();
    void setCurrentTeacherIndex(const QModelIndex &index);
    void addTeacherSubject();
    void removeTeacherSubject();
    void acceptTeachersChanges();
    void cancelTeachersChanges();

    void addSubject();
    void removeSubject();
    void acceptSubjectsChanges();
    void cancelSubjectsChanges();

    void filterClasses();
    void clearFilterClasses();
    void addClass();
    void removeClass();
    void acceptClassesChanges();
    void cancelClassesChanges();

    void filterWorks();
    void clearFilterWorks();
    void addWork();
    void removeWork();
    void acceptWorksChanges();
    void cancelWorksChanges();

private:
    Ui::MainWindow *ui;

    QSqlTableModel *studentModel;
    QSqlRelationalTableModel *groupMemberModel;
    QSqlQueryModel *possibleClassModel;
    QSqlRelationalTableModel *brigadeMemberModel;
    QSqlQueryModel *possibleWorkModel;

    QSqlTableModel *teacherModel;
    QSqlRelationalTableModel *teacherToSubjectModel;

    QSqlTableModel *subjectModel;

    QSqlRelationalTableModel *classModel;

    QSqlRelationalTableModel *workModel;
};

#endif // MAINWINDOW_H
