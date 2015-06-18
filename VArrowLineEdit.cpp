#include "VArrowLineEdit.h"

VArrowLineEdit::VArrowLineEdit(const QString &text, QWidget* parent) : QLineEdit(text, parent)
{

}

VArrowLineEdit::~VArrowLineEdit()
{

}

void VArrowLineEdit::keyPressEvent(QKeyEvent* event)
{
    bool keyOk;
    int value = this->text().toInt( &keyOk );
    if(event->key() == Qt::Key_Up)
    {
        this->setText(QString::number(value + 1));
    }
    else if(event->key() == Qt::Key_Down)
    {
        this->setText(QString::number(value - 1));
    }
	QLineEdit::keyPressEvent(event);
}
