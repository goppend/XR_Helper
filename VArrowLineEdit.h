#ifndef VARROWLINEEDIT_H
#define VARROWLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class VArrowLineEdit : public QLineEdit
{
        Q_OBJECT
public:
    void keyPressEvent(QKeyEvent* event);
    VArrowLineEdit(const QString &text = QString(), QWidget* parent = NULL);
    ~VArrowLineEdit();
};

#endif // VARROWLINEEDIT_H
