#include <QtGui>
#include "cell.h"

Cell::Cell()
{
	setDirty();
}

QTableWidgetItem* Cell::clone() const
{
	return new Cell(*this);
}

void Cell::setDirty()
{
	cacheIsDirty = true;
}

QString Cell::formula() const
{
	return data(Qt::EditRole).toString();
}

void Cell::setFormula(const QString& formula)
{
	setData(Qt::EditRole, formula);
}

void Cell::setData(int role, const QVariant& value)
{
	QTableWidgetItem::setData(role, value);
	if (Qt::EditRole == role)
		setDirty();
}

QVariant Cell::data(int role) const
{
	if (Qt::DisplayRole == role)
	{
		return value().isValid() ? value().toString() : "####";
	}
	else if (Qt::TextAlignmentRole == role)
	{
		return value().type() == QVariant::String ?
			int(Qt::AlignLeft | Qt::AlignVCenter) : 
			int(Qt::AlignRight | Qt::AlignVCenter);
	}
	else
		return QTableWidgetItem::data(role);
}

const QVariant Invalid;

QVariant Cell::value() const
{
	if (cacheIsDirty)
	{
		cacheIsDirty = false;
		QString formulaStr = formula();
		if (formulaStr.startsWith('\''))
			cachedValue = formulaStr.mid(1);
		else if (formulaStr.startsWith('='))
		{
			cachedValue = Invalid;
			auto expr = formulaStr.mid(1);
			expr.replace(" ", "");
			expr.append(QChar::Null);
			
			int pos = 0;
			cachedValue = evalExpression(expr, pos);
			if (expr[pos] != QChar::Null)
				cachedValue = Invalid;
		}
		else
		{
			bool ok;
			double d = formulaStr.toDouble(&ok);
			cachedValue = ok ? QVariant(d) : formulaStr;
		}
	}
	return cachedValue;
}

QVariant Cell::evalExpression(const QString& str, int& pos) const
{
	QVariant result = evalTerm(str, pos);
	while (str[pos] != QChar::Null)
	{
		QChar op = str[pos];
		if (op != '+' && op != '-')
			return result;
		++pos;
		
		QVariant term = evalTerm(str, pos);
		if (result.type() == QVariant::Double && term.type() == QVariant::Double)
		{
			if (op == '+')
				result = result.toDouble() + term.toDouble();
			else
				result = result.toDouble() - term.toDouble();
		}
		else
			result = Invalid;
	}
	return result;
}

QVariant Cell::evalTerm(const QString& str, int& pos) const
{
	auto result = evalFactor(str, pos);
	while (str[pos] != QChar::Null)
	{
		QChar op = str[pos];
		if (op != '*' && op != '/')
			return result;
		++pos;
		
		auto factor = evalFactor(str, pos);
		if (result.type() == QVariant::Double && factor.type() == QVariant::Double)
		{
			if (op == '*')
				result = result.toDouble() * factor.toDouble();
			else
			{
				if (factor.toDouble() == 0.0)
					result = Invalid;
				else
					result = result.toDouble() / factor.toDouble();
			}
		}
		else
			result = Invalid;
	}
	return result;
}

QVariant Cell::evalFactor(const QString& str, int& pos) const
{
	QVariant result;
	bool negative = false;
	
	if (str[pos] == '-')
	{
		negative = true;
		++pos;
	}
	
	if (str[pos] == '(')
	{
		++pos;
		result = evalExpression(str, pos);
		if (str[pos] != ')')
			result = Invalid;
		++pos;
	}
	else
	{
		QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
		QString token;
		while (str[pos].isLetterOrNumber() || str[pos] == '.')
		{
			token += str[pos];
			++pos;
		}
		if (regExp.exactMatch(token))
		{
			int column = token[0].toUpper().unicode() - 'A';
			int row = token.mid(1).toInt() - 1;
			
			auto c = static_cast<Cell*>(tableWidget()->item(row, column));
			if (c)
				result = c->value();
			else
				result = 0.0;
		}
		else
		{
			bool ok;
			result = token.toDouble(&ok);
			if (!ok)
				result = Invalid;
		}
	}
	
	if (negative)
	{
		if (result.type() == QVariant::Double)
			result = -result.toDouble();
		else
			result = Invalid;
	}
	return result;
}