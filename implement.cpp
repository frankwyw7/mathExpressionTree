#include "declaration.h"

using namespace expressionTree;

exception::exception(int ln, int ps, std::string b) : line(ln), pos(ps), err(b){}

Token::Token(Type ty, std::shared_ptr<Token> lf, std::shared_ptr<Token> rt) :type(ty), left(lf), right(rt){}

std::string exception::what()
{
	std::ostringstream bb;
	std::string str = "in line ";

	bb << line;
	bb << " pos ";
	bb << pos;
	bb << " , There has ";
	bb << err;

	str += bb.str();
	return str;
}

Exp::Exp(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty) :Token(ty, lf, rt){}

int Exp::op()
{
	int lf = left->op();
	int rt;
	if (right != nullptr)
	{
		switch (right->type)
		{
		case Type::add:
			rt = right->op();
			return lf + rt;
		case Type::sub:
			rt = right->op();
			return lf - rt;
		default:
			break;
		}

	}
	return lf;
}

Exp_::Exp_(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty) :Token(ty, lf, rt){}

int Exp_::op()
{
	int lf = left->op();
	int rt;
	if (right != nullptr){
		switch (right->type)
		{
		case Type::add:
			rt = right->op();
			return lf + rt;
		case Type::sub:
			rt = right->op();
			return lf - rt;
		default:
			break;
		}
	}
	return lf;

}

Term::Term(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty) :Token(ty, lf, rt){}

int Term::op()
{
	int lf = left->op();
	int rt;
	if (right != nullptr){
		switch (right->type)
		{
		case Type::mul:
			rt = right->op();
			return lf * rt;
		case Type::div:
			rt = right->op();
			return lf / rt;
		default:
			break;
		}
	}
	return lf;
}

Term_::Term_(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty) :Token(ty, lf, rt){}

int Term_::op()
{
	int lf = left->op();
	int rt;
	if (right != nullptr){
		switch (right->type)
		{
		case Type::mul:
			rt = right->op();
			return lf * rt;
		case Type::div:
			rt = right->op();
			return lf / rt;
		default:
			break;
		}
		return lf;
	}
}

Factor::Factor(std::shared_ptr<Token> lf, Type ty) :Token(ty, lf, nullptr){}

Num::Num(Type ty, int vl) : Token(ty, nullptr, nullptr) { value = vl; }

Calculator::Calculator(std::vector<std::string> strvec) : buf(strvec), pos(0), resultval(0){}

void Calculator::run()
{
	int thislen;
	for (; line < buf.size(); line++, pos = 0)
	{
		result.push_back(getExp());

		thislen = buf[line].size();
		
		if (pos != thislen)
		{
			throw exception(line+1, pos + 1, "grammar error");
		}

		std::cout << "line "<<line + 1<< " input is :";
		print(result[line]);
		std::cout << "\n";
		std::cout << "line " << line+1 << " result is :";
		std::cout << result[line]->op();
		std::cout << '\n' << '\n';
	}
	
}

void Calculator::print(std::shared_ptr<Token> t)
{
	switch (t->type)
	{
	case Type::parenthese:
		std::cout << "(";
		print(t->left);
		std::cout << ")";
		break;
	case Type::add:
		std::cout << "+";
		print(t->left);
		print(t->right);
		break;
	case Type::mul:
		std::cout << "*";
		print(t->left);
		print(t->right);
		break;
	case Type::sub:
		std::cout << "-";
		print(t->left);
		print(t->right);
		break;
	case Type::div:
		std::cout << "/";
		print(t->left);
		print(t->right);
		break;
	case Type::num:
		std::cout << t->left->value;
		break;
	case Type::factor:
		print(t->left);
		print(t->right);
		break;
	case Type::term:
		print(t->left);
		print(t->right);
		break;
	default:
		break;
	}
}

void Calculator::skipspace(){ if (buf[line][pos] == ' ' && pos < buf[line].size())pos++; }

std::shared_ptr<Exp> Calculator::getExp()
{
	auto left = getTerm();
	auto right = getExp_();
	return std::make_shared<Exp>(left, right, Type::term);
}

std::shared_ptr<Exp_> Calculator::getExp_()
{
	skipspace();
	if (buf[line][pos] == '+')
	{
		pos++;
		auto left = getTerm();
		auto right = getExp_();
		return std::make_shared<Exp_>(left, right, Type::add);
	}
	else if (buf[line][pos] == '-')
	{
		pos++;
		auto left = getTerm();
		auto right = getExp_();
		return std::make_shared<Exp_>(left, right, Type::sub);
	}
	else
	{
		return std::make_shared<Exp_>(Type::nil);
	}
}

std::shared_ptr<Term> Calculator::getTerm()
{
	auto left = getFactor();
	auto right = getTerm_();
	return std::make_shared<Term>(left, right, Type::factor);
}

std::shared_ptr<Term_> Calculator::getTerm_()
{
	skipspace();
	if (buf[line][pos] == '*')
	{
		pos++;
		auto left = getFactor();
		auto right = getTerm_();
		return std::make_shared<Term_>(left, right, Type::mul);
	}
	else if (buf[line][pos] == '/')
	{
		pos++;
		auto left = getFactor();
		auto right = getTerm_();
		return std::make_shared<Term_>(left, right, Type::div);
	}
	else
	{
		return std::make_shared<Term_>(Type::nil);
	}
}

std::shared_ptr<Factor> Calculator::getFactor()
{
	skipspace();
	if (buf[line][pos] == '(')
	{
		pos++;
		auto res = std::make_shared<Factor>(getExp(), Type::parenthese);

		if (buf[line][pos] == ')')
			pos++;
		else throw exception(line+1, pos + 1, "no right parenthese");
		return res;
	}
	else
		return std::make_shared<Factor> (getNum(), Type::num);
}

std::shared_ptr<Num> Calculator::getNum()
{
	char test = buf[line][pos];
	if ((test <= '0') || (test >= '9'))
		throw exception(line+1, pos + 1, "no digit");
	int value = 0;
	while (('0' <= test) && (test <= '9'))
	{
		value = value * 10 + test - '0';
		pos++;
		test = buf[line][pos];
	}
	return std::make_shared<Num>(Type::num, value);
}