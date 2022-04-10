#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataWidgetMapper>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QUuid>
#include <QSqlQuery>
#include <QSqlRelationalDelegate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    studentModel = new QSqlTableModel();
    studentModel->setTable("student");
    studentModel->setHeaderData(studentModel->fieldIndex("student_number"), Qt::Horizontal, tr("Student number"));
    studentModel->setHeaderData(studentModel->fieldIndex("name"), Qt::Horizontal, tr("Name", "person"));
    studentModel->setHeaderData(studentModel->fieldIndex("birth_date"), Qt::Horizontal, tr("Birth date"));
    studentModel->setHeaderData(studentModel->fieldIndex("status"), Qt::Horizontal, tr("Status"));
    studentModel->setHeaderData(studentModel->fieldIndex("course"), Qt::Horizontal, tr("Course"));
    studentModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    studentModel->select();

    ui->studentTableView->setModel(studentModel);
    ui->studentTableView->setColumnHidden(studentModel->fieldIndex("id"), true);
    ui->studentTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->studentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->studentTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QDataWidgetMapper *studentMapper = new QDataWidgetMapper();
    studentMapper->setModel(studentModel);
    studentMapper->addMapping(ui->studentNumberLineEdit, studentModel->fieldIndex("student_number"));
    studentMapper->addMapping(ui->studentNameLineEdit, studentModel->fieldIndex("name"));
    studentMapper->addMapping(ui->studentBirthDateEdit, studentModel->fieldIndex("birth_date"));
    studentMapper->addMapping(ui->studentStatusLineEdit, studentModel->fieldIndex("status"));
    studentMapper->addMapping(ui->studentCourseLineEdit, studentModel->fieldIndex("course"));
    studentMapper->toFirst();

    groupMemberModel = new QSqlRelationalTableModel();
    groupMemberModel->setTable("group_member");
    groupMemberModel->setRelation(groupMemberModel->fieldIndex("id_class"), QSqlRelation("class", "id", "name"));
    groupMemberModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->studentClassListView->setModel(groupMemberModel);
    ui->studentClassListView->setModelColumn(groupMemberModel->fieldIndex("id_class"));
    ui->studentClassListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    possibleClassModel = new QSqlQueryModel();
    possibleClassModel->setQuery("SELECT id, name FROM class;");

    ui->studentSelectClassComboBox->setModel(possibleClassModel);
    ui->studentSelectClassComboBox->setModelColumn(1);

    brigadeMemberModel = new QSqlRelationalTableModel();
    brigadeMemberModel->setTable("brigade_member");
    brigadeMemberModel->setRelation(brigadeMemberModel->fieldIndex("id_work"), QSqlRelation("student_work", "id", "name"));
    brigadeMemberModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->studentWorkListView->setModel(brigadeMemberModel);
    ui->studentWorkListView->setModelColumn(brigadeMemberModel->fieldIndex("id_work"));
    ui->studentWorkListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    possibleWorkModel = new QSqlQueryModel();
    possibleWorkModel->setQuery("SELECT id, name FROM student_work;");

    ui->studentSelectWorkComboBox->setModel(possibleWorkModel);
    ui->studentSelectWorkComboBox->setModelColumn(1);

    connect(ui->studentTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, studentMapper, &QDataWidgetMapper::setCurrentModelIndex);
    connect(ui->studentTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::setCurrentStudentIndex);

    ui->studentBirthDateFromEdit->setMaximumDate(QDate::currentDate());
    ui->studentBirthDateToEdit->setDate(QDate::currentDate());
    ui->studentBirthDateToEdit->setMaximumDate(QDate::currentDate());


    teacherModel = new QSqlTableModel();
    teacherModel->setTable("teacher");
    teacherModel->setHeaderData(teacherModel->fieldIndex("teacher_number"), Qt::Horizontal, tr("Teacher number"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("name"), Qt::Horizontal, tr("Name", "person"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("birth_date"), Qt::Horizontal, tr("Birth date"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("pulpit"), Qt::Horizontal, tr("Pulpit"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("qualification"), Qt::Horizontal, tr("Qualification"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("post"), Qt::Horizontal, tr("Post"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("maximal_hours"), Qt::Horizontal, tr("Maximal work hours"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("is_load_reduced"), Qt::Horizontal, tr("Is load reduced"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("reduced_hours"), Qt::Horizontal, tr("Reduced hours"));
    teacherModel->setHeaderData(teacherModel->fieldIndex("is_foreign"), Qt::Horizontal, tr("Is teacher foreign"));
    teacherModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    teacherModel->select();

    ui->teacherTableView->setModel(teacherModel);
    ui->teacherTableView->setColumnHidden(teacherModel->fieldIndex("id"), true);
    ui->teacherTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->teacherTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->teacherTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QDataWidgetMapper *teacherMapper = new QDataWidgetMapper();
    teacherMapper->setModel(teacherModel);
    teacherMapper->addMapping(ui->teacherNumberLineEdit, teacherModel->fieldIndex("teacher_number"));
    teacherMapper->addMapping(ui->teacherNameLineEdit, teacherModel->fieldIndex("name"));
    teacherMapper->addMapping(ui->teacherBirthDateEdit, teacherModel->fieldIndex("birth_date"));
    teacherMapper->addMapping(ui->teacherPulpitLineEdit, teacherModel->fieldIndex("pulpit"));
    teacherMapper->addMapping(ui->teacherQualificationLineEdit, teacherModel->fieldIndex("qualification"));
    teacherMapper->addMapping(ui->teacherPostLineEdit, teacherModel->fieldIndex("post"));
    teacherMapper->addMapping(ui->teacherMaximalWorkHoursSpinBox, teacherModel->fieldIndex("maximal_hours"));
    teacherMapper->addMapping(ui->teacherIsLoadReducedCheckBox, teacherModel->fieldIndex("is_load_reduced"));
    teacherMapper->addMapping(ui->teacherReducedHoursSpinBox, teacherModel->fieldIndex("reduced_hours"));
    teacherMapper->addMapping(ui->teacherIsForeignCheckBox, teacherModel->fieldIndex("is_foreign"));
    teacherMapper->toFirst();

    teacherToSubjectModel = new QSqlRelationalTableModel();
    teacherToSubjectModel->setTable("teacher_to_subject");
    teacherToSubjectModel->setRelation(teacherToSubjectModel->fieldIndex("id_subject"), QSqlRelation("subject", "id", "name"));
    teacherToSubjectModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->teacherSubjectListView->setModel(teacherToSubjectModel);
    ui->teacherSubjectListView->setModelColumn(teacherToSubjectModel->fieldIndex("id_subject"));
    ui->teacherSubjectListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->teacherTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, teacherMapper, &QDataWidgetMapper::setCurrentModelIndex);
    connect(ui->teacherTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::setCurrentTeacherIndex);

    ui->teacherBirthDateFromEdit->setMaximumDate(QDate::currentDate());
    ui->teacherBirthDateToEdit->setDate(QDate::currentDate());
    ui->teacherBirthDateToEdit->setMaximumDate(QDate::currentDate());


    subjectModel = new QSqlTableModel();
    subjectModel->setTable("subject");
    subjectModel->setHeaderData(subjectModel->fieldIndex("name"), Qt::Horizontal, tr("Name", "object"));
    subjectModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    subjectModel->select();

    ui->subjectTableView->setModel(subjectModel);
    ui->subjectTableView->setColumnHidden(subjectModel->fieldIndex("id"), true);
    ui->subjectTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->subjectTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->subjectTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QDataWidgetMapper *subjectMapper = new QDataWidgetMapper();
    subjectMapper->setModel(subjectModel);
    subjectMapper->addMapping(ui->subjectNameLineEdit, subjectModel->fieldIndex("name"));
    subjectMapper->toFirst();

    connect(ui->subjectTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, subjectMapper, &QDataWidgetMapper::setCurrentModelIndex);

    ui->teacherSelectSubjectComboBox->setModel(subjectModel);
    ui->teacherSelectSubjectComboBox->setModelColumn(1);


    classModel = new QSqlRelationalTableModel();
    classModel->setTable("class");
    classModel->setHeaderData(classModel->fieldIndex("teacher_id"), Qt::Horizontal, tr("Teacher"));
    classModel->setRelation(classModel->fieldIndex("teacher_id"), QSqlRelation("teacher", "id", "name"));
    classModel->setHeaderData(classModel->fieldIndex("subject_id"), Qt::Horizontal, tr("Subject"));
    classModel->setRelation(classModel->fieldIndex("subject_id"), QSqlRelation("subject", "id", "name"));
    classModel->setHeaderData(classModel->fieldIndex("name"), Qt::Horizontal, tr("Name", "object"));
    classModel->setHeaderData(classModel->fieldIndex("type"), Qt::Horizontal, tr("Type"));
    classModel->setHeaderData(classModel->fieldIndex("hours"), Qt::Horizontal, tr("Hours"));
    classModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    classModel->select();

    ui->classTableView->setModel(classModel);
    ui->classTableView->setItemDelegate(new QSqlRelationalDelegate());
    ui->classTableView->setColumnHidden(classModel->fieldIndex("id"), true);
    ui->classTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->classTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->classTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->classTeacherComboBox->setModel(classModel->relationModel(1));
    ui->classTeacherComboBox->setModelColumn(classModel->relationModel(1)->fieldIndex("name"));
    ui->classSubjectComboBox->setModel(classModel->relationModel(2));
    ui->classSubjectComboBox->setModelColumn(classModel->relationModel(2)->fieldIndex("name"));

    QDataWidgetMapper *classMapper = new QDataWidgetMapper();
    classMapper->setModel(classModel);
    classMapper->setItemDelegate(new QSqlRelationalDelegate());
    classMapper->addMapping(ui->classTeacherComboBox, 1);
    classMapper->addMapping(ui->classSubjectComboBox, 2);
    classMapper->addMapping(ui->classNameLineEdit, classModel->fieldIndex("name"));
    classMapper->addMapping(ui->classTypeLineEdit, classModel->fieldIndex("type"));
    classMapper->addMapping(ui->classHoursSpinBox, classModel->fieldIndex("hours"));
    classMapper->toFirst();

    connect(ui->classTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, classMapper, &QDataWidgetMapper::setCurrentModelIndex);

    ui->classTeacherFilterComboBox->setModel(classModel->relationModel(1));
    ui->classTeacherFilterComboBox->setModelColumn(classModel->relationModel(1)->fieldIndex("name"));
    ui->classSubjectFilterComboBox->setModel(classModel->relationModel(2));
    ui->classSubjectFilterComboBox->setModelColumn(classModel->relationModel(2)->fieldIndex("name"));


    workModel = new QSqlRelationalTableModel();
    workModel->setTable("student_work");
    workModel->setHeaderData(workModel->fieldIndex("teacher_id"), Qt::Horizontal, tr("Teacher"));
    workModel->setRelation(workModel->fieldIndex("teacher_id"), QSqlRelation("teacher", "id", "name"));
    workModel->setHeaderData(workModel->fieldIndex("subject_id"), Qt::Horizontal, tr("Subject"));
    workModel->setRelation(workModel->fieldIndex("subject_id"), QSqlRelation("subject", "id", "name"));
    workModel->setHeaderData(workModel->fieldIndex("name"), Qt::Horizontal, tr("Name", "object"));
    workModel->setHeaderData(workModel->fieldIndex("type"), Qt::Horizontal, tr("Type"));
    workModel->setHeaderData(workModel->fieldIndex("hours"), Qt::Horizontal, tr("Hours"));
    workModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    workModel->select();

    ui->workTableView->setModel(workModel);
    ui->workTableView->setItemDelegate(new QSqlRelationalDelegate());
    ui->workTableView->setColumnHidden(workModel->fieldIndex("id"), true);
    ui->workTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->workTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->workTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->workTeacherComboBox->setModel(workModel->relationModel(1));
    ui->workTeacherComboBox->setModelColumn(workModel->relationModel(1)->fieldIndex("name"));
    ui->workSubjectComboBox->setModel(workModel->relationModel(2));
    ui->workSubjectComboBox->setModelColumn(workModel->relationModel(2)->fieldIndex("name"));

    QDataWidgetMapper *workMapper = new QDataWidgetMapper();
    workMapper->setModel(workModel);
    workMapper->setItemDelegate(new QSqlRelationalDelegate());
    workMapper->addMapping(ui->workTeacherComboBox, 1);
    workMapper->addMapping(ui->workSubjectComboBox, 2);
    workMapper->addMapping(ui->workNameLineEdit, workModel->fieldIndex("name"));
    workMapper->addMapping(ui->workTypeLineEdit, workModel->fieldIndex("type"));
    workMapper->addMapping(ui->workHoursSpinBox, workModel->fieldIndex("hours"));
    workMapper->toFirst();

    connect(ui->workTableView->selectionModel(), &QItemSelectionModel::currentRowChanged, workMapper, &QDataWidgetMapper::setCurrentModelIndex);

    ui->workTeacherFilterComboBox->setModel(workModel->relationModel(1));
    ui->workTeacherFilterComboBox->setModelColumn(workModel->relationModel(1)->fieldIndex("name"));
    ui->workSubjectFilterComboBox->setModel(workModel->relationModel(2));
    ui->workSubjectFilterComboBox->setModelColumn(workModel->relationModel(2)->fieldIndex("name"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete studentModel;
    delete groupMemberModel;
    delete possibleClassModel;
    delete brigadeMemberModel;
    delete possibleWorkModel;
    delete teacherModel;
    delete teacherToSubjectModel;
    delete subjectModel;
    delete classModel;
    delete workModel;
}

void MainWindow::filterStudents()
{
    QString birthDateFromString = ui->studentBirthDateFromEdit->date().toString(Qt::ISODate);
    QString birthDateToString = ui->studentBirthDateToEdit->date().toString(Qt::ISODate);
    studentModel->setFilter(QString("birth_date BETWEEN \'%1\' AND \'%2\'").arg(birthDateFromString, birthDateToString));
    studentModel->select();
}

void MainWindow::clearFilterStudents()
{
    studentModel->setFilter("");
    studentModel->select();
}

void MainWindow::addStudent()
{
    QSqlRecord studentRecord = studentModel->record();
    studentRecord.setValue(studentModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    if (!studentModel->insertRecord(0, studentRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding student"),
            QString(tr("Error: %1")).arg(studentModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    ui->studentTableView->selectRow(0);
}

void MainWindow::removeStudent()
{
    int row = ui->studentTableView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!studentModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing student"),
            QString(tr("Error: %1")).arg(studentModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::setCurrentStudentIndex(const QModelIndex &index)
{
    QVariant studentId = studentModel->data(studentModel->index(index.row(), studentModel->fieldIndex("id")));
    groupMemberModel->setFilter(QString("id_student = \'%1\'").arg(studentId.toString()));
    groupMemberModel->select();
    brigadeMemberModel->setFilter(QString("id_student = \'%1\'").arg(studentId.toString()));
    brigadeMemberModel->select();
}

void MainWindow::addStudentClass()
{
    int row = ui->studentSelectClassComboBox->currentIndex();
    if(row == -1)
        return;
    QVariant classId = possibleClassModel->record(row).value(0);
    int studentRow = ui->studentTableView->selectionModel()->currentIndex().row();
    if(studentRow == -1)
        return;
    QVariant studentId = studentModel->record(studentRow).value(studentModel->fieldIndex("id"));
    QSqlRecord groupMemberRecord = groupMemberModel->record();
    groupMemberRecord.setValue(groupMemberModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    groupMemberRecord.setValue(groupMemberModel->fieldIndex("name"), classId);
    groupMemberRecord.setValue(groupMemberModel->fieldIndex("id_student"), studentId);

    if (!groupMemberModel->insertRecord(0, groupMemberRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding class"),
            QString(tr("Error: %1")).arg(groupMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::removeStudentClass()
{
    int row = ui->studentClassListView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!groupMemberModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing class"),
            QString(tr("Error: %1")).arg(groupMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::addStudentWork()
{
    int row = ui->studentSelectWorkComboBox->currentIndex();
    if(row == -1)
        return;
    QVariant workId = possibleWorkModel->record(row).value(0);
    int studentRow = ui->studentTableView->selectionModel()->currentIndex().row();
    if(studentRow == -1)
        return;
    QVariant studentId = studentModel->record(studentRow).value(studentModel->fieldIndex("id"));
    QSqlRecord brigadeMemberRecord = brigadeMemberModel->record();
    brigadeMemberRecord.setValue(brigadeMemberModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    brigadeMemberRecord.setValue(brigadeMemberModel->fieldIndex("name"), workId);
    brigadeMemberRecord.setValue(brigadeMemberModel->fieldIndex("id_student"), studentId);

    if (!brigadeMemberModel->insertRecord(0, brigadeMemberRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding student work"),
            QString(tr("Error: %1")).arg(brigadeMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::removeStudentWork()
{
    int row = ui->studentWorkListView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!brigadeMemberModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing student work"),
            QString(tr("Error: %1")).arg(brigadeMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::acceptStudentsChanges()
{
    if (!studentModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in students"),
            QString(tr("Error: %1")).arg(studentModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    if (!groupMemberModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in students"),
            QString(tr("Error: %1")).arg(groupMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    if (!brigadeMemberModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in students"),
            QString(tr("Error: %1")).arg(brigadeMemberModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::cancelStudentsChanges()
{
    studentModel->revertAll();
    groupMemberModel->revertAll();
    brigadeMemberModel->revertAll();
}


void MainWindow::filterTeachers()
{
    QString birthDateFromString = ui->teacherBirthDateFromEdit->date().toString(Qt::ISODate);
    QString birthDateToString = ui->teacherBirthDateToEdit->date().toString(Qt::ISODate);
    QString maximalWorkHoursFromString = QString::number(ui->teacherMaximalWorkHoursFromSpinBox->value());
    QString maximalWorkHoursToString = QString::number(ui->teacherMaximalWorkHoursToSpinBox->value());
    QString isLoadReducedString = ui->teacherIsLoadReducedFilterCheckBox->isChecked() ? "TRUE" : "FALSE";
    QString reducedHoursFromString = QString::number(ui->teacherReducedHoursFromSpinBox->value());
    QString reducedHoursToString = QString::number(ui->teacherReducedHoursToSpinBox->value());
    QString isForeignString = ui->teacherIsForeignFilterCheckBox->isChecked() ? "TRUE" : "FALSE";
    teacherModel->setFilter(QString("(birth_date BETWEEN \'%1\' AND \'%2\') AND (maximal_hours BETWEEN %3 AND %4) AND is_load_reduced = %5 AND (reduced_hours BETWEEN %6 AND %7) AND is_foreign = %8")
                            .arg(birthDateFromString, birthDateToString, maximalWorkHoursFromString, maximalWorkHoursToString, isLoadReducedString, reducedHoursFromString, reducedHoursToString, isForeignString));
    teacherModel->select();
}

void MainWindow::clearFilterTeachers()
{
    teacherModel->setFilter("");
    teacherModel->select();
}

void MainWindow::addTeacher()
{
    QSqlRecord teacherRecord = teacherModel->record();
    teacherRecord.setValue(teacherModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    if (!teacherModel->insertRecord(0, teacherRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding teacher"),
            QString(tr("Error: %1")).arg(teacherModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    ui->teacherTableView->selectRow(0);
}

void MainWindow::removeTeacher()
{
    int row = ui->teacherTableView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!teacherModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing teacher"),
            QString(tr("Error: %1")).arg(teacherModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::setCurrentTeacherIndex(const QModelIndex &index)
{
    QVariant teacherId = teacherModel->data(teacherModel->index(index.row(), teacherModel->fieldIndex("id")));
    teacherToSubjectModel->setFilter(QString("id_teacher = \'%1\'").arg(teacherId.toString()));
    teacherToSubjectModel->select();
}

void MainWindow::addTeacherSubject()
{
    int row = ui->teacherSelectSubjectComboBox->currentIndex();
    if(row == -1)
        return;
    QVariant subjectId = subjectModel->record(row).value(0);
    int teacherRow = ui->teacherTableView->selectionModel()->currentIndex().row();
    if(teacherRow == -1)
        return;
    QVariant teacherId = teacherModel->record(teacherRow).value(teacherModel->fieldIndex("id"));
    QSqlRecord teacherToSubjectRecord = teacherToSubjectModel->record();
    teacherToSubjectRecord.setValue(teacherToSubjectModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    teacherToSubjectRecord.setValue(teacherToSubjectModel->fieldIndex("name"), subjectId);
    teacherToSubjectRecord.setValue(teacherToSubjectModel->fieldIndex("id_teacher"), teacherId);

    if (!teacherToSubjectModel->insertRecord(0, teacherToSubjectRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding subject"),
            QString(tr("Error: %1")).arg(teacherToSubjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::removeTeacherSubject()
{
    int row = ui->teacherSubjectListView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!teacherToSubjectModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing subject"),
            QString(tr("Error: %1")).arg(teacherToSubjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::acceptTeachersChanges()
{
    if (!teacherModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in teachers"),
            QString(tr("Error: %1")).arg(teacherModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    if (!teacherToSubjectModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in teachers"),
            QString(tr("Error: %1")).arg(teacherToSubjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::cancelTeachersChanges()
{
    teacherModel->revertAll();
    teacherToSubjectModel->revertAll();
}

void MainWindow::addSubject()
{
    QSqlRecord subjectRecord = subjectModel->record();
    subjectRecord.setValue(subjectModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    if (!subjectModel->insertRecord(0, subjectRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding subject"),
            QString(tr("Error: %1")).arg(subjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    ui->subjectTableView->selectRow(0);
}

void MainWindow::removeSubject()
{
    int row = ui->subjectTableView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!subjectModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing subject"),
            QString(tr("Error: %1")).arg(subjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::acceptSubjectsChanges()
{
    if (!subjectModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in subjects"),
            QString(tr("Error: %1")).arg(subjectModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::cancelSubjectsChanges()
{
    subjectModel->revertAll();
}

void MainWindow::filterClasses()
{
    QString teacherId = classModel->relationModel(1)->record(ui->classTeacherFilterComboBox->currentIndex()).value(0).toString();
    QString subjectId = classModel->relationModel(2)->record(ui->classSubjectFilterComboBox->currentIndex()).value(0).toString();
    QString hoursFromString = QString::number(ui->classHoursFromSpinBox->value());
    QString hoursToString = QString::number(ui->classHoursToSpinBox->value());
    classModel->setFilter(QString("teacher_id = \'%1\' AND subject_id = \'%2\' AND (hours BETWEEN %3 AND %4)")
                            .arg(teacherId, subjectId, hoursFromString, hoursToString));
    classModel->select();
}

void MainWindow::clearFilterClasses()
{
    classModel->setFilter("");
    classModel->select();
}

void MainWindow::addClass()
{
    QSqlRecord classRecord = classModel->record();
    classRecord.setValue(classModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    if (!classModel->insertRecord(0, classRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding class"),
            QString(tr("Error: %1")).arg(classModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    ui->classTableView->selectRow(0);
}

void MainWindow::removeClass()
{
    int row = ui->classTableView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!classModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing class"),
            QString(tr("Error: %1")).arg(classModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::acceptClassesChanges()
{
    if (!classModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in classs"),
            QString(tr("Error: %1")).arg(classModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    possibleClassModel->setQuery(possibleClassModel->query().lastQuery());
}

void MainWindow::cancelClassesChanges()
{
    classModel->revertAll();
}

void MainWindow::filterWorks()
{
    QString teacherId = workModel->relationModel(1)->record(ui->workTeacherFilterComboBox->currentIndex()).value(0).toString();
    QString subjectId = workModel->relationModel(2)->record(ui->workSubjectFilterComboBox->currentIndex()).value(0).toString();
    QString hoursFromString = QString::number(ui->workHoursFromSpinBox->value());
    QString hoursToString = QString::number(ui->workHoursToSpinBox->value());
    workModel->setFilter(QString("teacher_id = \'%1\' AND subject_id = \'%2\' AND (hours BETWEEN %3 AND %4)")
                            .arg(teacherId, subjectId, hoursFromString, hoursToString));
    workModel->select();
}

void MainWindow::clearFilterWorks()
{
    workModel->setFilter("");
    workModel->select();
}

void MainWindow::addWork()
{
    QSqlRecord workRecord = workModel->record();
    workRecord.setValue(workModel->fieldIndex("id"), QUuid::createUuid().toString(QUuid::WithoutBraces));
    if (!workModel->insertRecord(0, workRecord))
    {
        QMessageBox::critical(this, tr("Error occured while adding work"),
            QString(tr("Error: %1")).arg(workModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    ui->workTableView->selectRow(0);
}

void MainWindow::removeWork()
{
    int row = ui->workTableView->selectionModel()->currentIndex().row();
    if (row == -1)
        return;
    if (!workModel->removeRow(row))
    {
        QMessageBox::critical(this, tr("Error occurd while removing work"),
            QString(tr("Error: %1")).arg(workModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
}

void MainWindow::acceptWorksChanges()
{
    if (!workModel->submitAll())
    {
        QMessageBox::critical(this, tr("Error occured while accepting changes in works"),
            QString(tr("Error: %1")).arg(workModel->lastError().text()),
            QMessageBox::Cancel);
        return;
    }
    possibleWorkModel->setQuery(possibleWorkModel->query().lastQuery());
}

void MainWindow::cancelWorksChanges()
{
    workModel->revertAll();
}
