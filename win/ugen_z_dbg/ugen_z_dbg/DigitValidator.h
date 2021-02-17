#pragma once

#include <QtWidgets/QHeaderView>


class DigitValidator : public QValidator
{
private:
	int maxLen;

public:
	DigitValidator(QObject * parent, int maxlength) : QValidator(parent),
														maxLen(maxlength + 1)
	{
	}


	virtual State validate(QString &str, int &pos) const
	{
		QRegExp rxp = QRegExp("[0-9]");
		if(str.length() == 0 || (str.contains(rxp) && str.length() < maxLen))
		{
			return Acceptable;
		}
		return Invalid;
	}
};