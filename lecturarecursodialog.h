#ifndef LECTURARECURSODIALOG_H
#define LECTURARECURSODIALOG_H

#include <QDialog>

namespace Ui {
class LecturaRecursoDialog;
}

class LecturaRecursoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LecturaRecursoDialog(QWidget *parent = 0);
    ~LecturaRecursoDialog();

private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::LecturaRecursoDialog *ui;
};

#endif // LECTURARECURSODIALOG_H
