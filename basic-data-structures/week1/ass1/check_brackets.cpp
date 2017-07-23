#include <iostream>
#include <stack>
#include <string>

struct Bracket {
    Bracket(char type, size_t position):
        type(type),
        position(position)
    {}

    bool Matchc(char c) {
        if (type == '[' && c == ']')
            return true;
        if (type == '{' && c == '}')
            return true;
        if (type == '(' && c == ')')
            return true;
        return false;
    }

    char type;
	size_t position;
};

int main() {
	std::string text;
	getline(std::cin, text);

	std::stack <Bracket> obs;
	for (size_t position = 0; position < text.length(); ++position)
	{
		char next = text[position];

		switch (next)
		{
		case '(':
		case '[':
		case '{':
			obs.push({ next, position + 1 });
			break;
		case ')':
			if (obs.top().type != '(')
				{
					std::cout << position + 1 << "\n";
					return 0;
				}
			obs.pop();
			break;
		case ']':
			if (obs.top().type != '[')
				{
					std::cout << position + 1 << "\n";
					return 0;
				}
			obs.pop();
			break;
		case '}':
			if (obs.top().type != '{')
				{
					std::cout << position + 1 << "\n";
					return 0;
				}
			obs.pop();
			break;
		default:
			break;
		}
	}

	if (obs.empty())
	{
		std::cout << "Success\n";
	}
	else
	{
		std::cout << obs.top().position << "\n";
	}

	return 0;
}
