/*
factor = '(' exp ')' | num
term = factor term'
term' = * factor term' | e
exp = term exp'
exp' = + term exp' | e
*/

#include <sstream>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

namespace expressionTree
{
	enum class Type
	{
		parenthese, nil, add, sub, mul, div, num, factor, term
	};

	struct exception
	{
		int line;
		int pos;
		std::string err;
	public:
		exception(int ln, int ps, std::string b);
		std::string what();
	};

	struct Token
	{
		Token(Type ty, std::shared_ptr<Token> lf, std::shared_ptr<Token> rt);
		virtual int op() = 0;

		int value;
		Type type;
		std::shared_ptr<Token> left;
		std::shared_ptr<Token> right;
	};

	struct Exp : Token
	{
	public:
		Exp(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty);
		void print();
		int op();
	};
	struct Exp_ : Token
	{
	public:
		Exp_(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty);
		Exp_(Type ty) :Token(ty, nullptr, nullptr){}
		void print();
		int op();
	};

	struct Term : Token
	{
	public:
		Term(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty);
		void print();
		int op();
	};

	struct Term_ : Token
	{
	public:
		Term_(std::shared_ptr<Token> lf, std::shared_ptr<Token> rt, Type ty);
		Term_(Type ty) :Token(ty, nullptr, nullptr){}
		void print();
		int op();
	};



	struct Factor : Token
	{
	public:
		Factor(std::shared_ptr<Token> lf, Type ty);
		void print();
		int op()
		{
			return left->op();
		}
	};


	struct Num :Token
	{
	public:
		Num(Type ty, int vl);
		void print();
		int op()
		{
			return value;
		}
	};


	class Calculator
	{
	public:
		Calculator(std::vector<std::string> strvec);
		~Calculator(){}

		Calculator(Calculator&) = delete;
		Calculator operator = (Calculator &) = delete;

		void run();
		void print(std::shared_ptr<Token> t);

	private:
		void skipspace();

		std::shared_ptr<Exp> getExp();
		std::shared_ptr<Exp_> getExp_();
		std::shared_ptr<Term> getTerm();
		std::shared_ptr<Term_> getTerm_();
		std::shared_ptr<Factor> getFactor();
		std::shared_ptr<Num> getNum();


		int resultval;
		int pos;
		int line;

		std::vector<std::string> buf;
		std::vector<std::shared_ptr<Exp>> result;
	};
}
