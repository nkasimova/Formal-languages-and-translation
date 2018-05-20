#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
const int INF = INT_MAX;

struct Prefix {
	int lengthPrefix; // длина префикса
	int wholeWord;//максимальная длина слова, которое можно составить только из буквы х
	bool isEps; //может ли это слово быть пустым
};

bool isCorrectSymbol(char symbol) {
	std::string correctSymbol = "abc1.+*";

	if (correctSymbol.find(symbol) != std::string::npos) {
		return true;
	}
	return false;
}

int getMaxPrefixLength(std::string expression, char x) {
	std::stack<Prefix> prefixLength;
	std::stack<std::string> polishNotation;
	char curSymbol;
	Prefix firstReg;
	Prefix secondReg;
	Prefix resultOperation;
	for (int i = 0; i < expression.length(); ++i) {
		curSymbol = expression[i];
		if (!isCorrectSymbol(curSymbol)) {
			return -INF;
		}

		if (curSymbol == '+') {
			if (!prefixLength.empty()) {
				secondReg = prefixLength.top();
				prefixLength.pop();
			}
			else {
				return -INF;
			}

			if (!prefixLength.empty()) {
				firstReg = prefixLength.top();
				prefixLength.pop();
			}
			else {
				return -INF;
			}

			resultOperation.lengthPrefix = std::max(firstReg.lengthPrefix, secondReg.lengthPrefix);
			resultOperation.wholeWord = std::max(firstReg.wholeWord, secondReg.wholeWord);
			resultOperation.isEps = firstReg.isEps + secondReg.isEps;

			prefixLength.push(resultOperation);
		}
		else
			if (curSymbol == '*') {
				if (!prefixLength.empty()) {
					firstReg = prefixLength.top();
					prefixLength.pop();
				}
				else {
					return -INF;
				}
				if (firstReg.wholeWord != 0) {
					prefixLength.push({ INF, INF, 1 });
				}
				else {
					prefixLength.push({ firstReg.lengthPrefix, 0, 1 });
				}
			}
			else
				if (curSymbol == '.') {
					if (!prefixLength.empty()) {
						secondReg = prefixLength.top();
						prefixLength.pop();
					}
					else {
						return -INF;
					}

					if (!prefixLength.empty()) {
						firstReg = prefixLength.top();
						prefixLength.pop();
					}
					else {
						return -INF;
					}

					int firstVariant = firstReg.lengthPrefix;
					int secondVariant = firstReg.lengthPrefix;
					int thirdVariant = firstReg.lengthPrefix;
					if (firstReg.isEps) {
						firstVariant = secondReg.lengthPrefix;
					}
					if (firstReg.wholeWord != 0) {
						thirdVariant = firstReg.wholeWord + secondReg.lengthPrefix;
					}
					resultOperation.lengthPrefix = std::max(firstVariant, std::max(secondVariant, thirdVariant));

					firstVariant = 0;
					secondVariant = 0;
					thirdVariant = 0;
					if (firstReg.isEps && secondReg.wholeWord != 0) {
						firstVariant = secondReg.wholeWord;
					}
					if (firstReg.wholeWord != 0 && secondReg.isEps) {
						secondVariant = firstReg.wholeWord;
					}
					if (firstReg.wholeWord != 0 && secondReg.wholeWord != 0) {
						thirdVariant = firstReg.wholeWord + secondReg.wholeWord;
					}
					resultOperation.wholeWord = std::max(firstVariant, std::max(secondVariant, thirdVariant));

					resultOperation.isEps = firstReg.isEps * secondReg.isEps;
					prefixLength.push({ resultOperation });
				}
				else {
					polishNotation.push(std::to_string(curSymbol));
					if (curSymbol == x) {
						prefixLength.push({ 1, 1, 0 });
					}
					else if (curSymbol == '1') {
						prefixLength.push({ 0, 0, 1 });
					}
					else {
						prefixLength.push({ 0, 0, 0 });
					}
				}
	}
	return prefixLength.top().lengthPrefix;
}

int main() {
	std::string polishNotation;
	char x;
	int k;
	std::cin >> polishNotation >> x >> k;

	int maxPrefixLength = getMaxPrefixLength(polishNotation, x);

	if (maxPrefixLength == -INF) {
		std::cout << "Error: Uncorrect expression!\n";
		return 0;
	}

	if (maxPrefixLength >= k) {
		std::cout << "YES\n";
	}
	else {
		std::cout << "NO\n";
	}

	return 0;
}