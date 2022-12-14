#include <SFML/Graphics.hpp>

#include <iostream>
#include <format>
#include <array>

const int n{ 10 };
std::array<std::array<std::string, n>, n> LCSCache;
std::array<std::array<std::string, n>, n> LCSRecursiveCache;
int LCSRecursiveCalls = 0;

// =========================
// UTILITY
// =========================

template<int n>
void printArray(std::array<std::array<std::string, n>, n> numbers)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << std::format("{0:8} ", numbers[i][j]);
		std::cout << '\n';
	}
}

std::string maxString(std::string s1, std::string s2)
{
	return s1.size() > s2.size() ? s1 : s2;
}

// =========================
// LCS
// =========================

std::string LCSDynamic(std::string s1, std::string s2)
{
	for (int i = 0; i <= s1.size(); i++)
		for (int j = 0; j <= s2.size(); j++)
		{
			if (i == 0 || j == 0)
			{
				LCSCache[i][j] = "";
				continue;
			}
			if (s1[i - 1] == s2[j - 1])
			{
				LCSCache[i][j] = LCSCache[i - 1][j - 1] + s1[i - 1];
				continue;
			}
			LCSCache[i][j] = maxString(LCSCache[i - 1][j], LCSCache[i][j - 1]);
		}
	return LCSCache[s1.size()][s2.size()];
}

std::string LCSRecursive(std::string s1, int x, std::string s2, int y)
{
	std::string result;
	LCSRecursiveCalls++;
	if (x == 0 || y == 0)
	{
		result = "";
		LCSRecursiveCache[x][y] = result;
		return result;
	}
	if (s1[x - 1] == s2[y - 1])
	{
		result = LCSRecursive(s1, x - 1, s2, y - 1) + s1[x - 1];
		LCSRecursiveCache[x][y] = result;
		return result;
	}
	result = maxString(LCSRecursive(s1, x - 1, s2, y), LCSRecursive(s1, x, s2, y - 1));
	LCSRecursiveCache[x][y] = result;
	return result;
}

std::string computeLCS(std::string s1, std::string s2)
{
	return LCSRecursive(s1, s1.size(), s2, s2.size());
}

// =========================
//		MAIN
// =========================

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


	std::string s1 = "AGCATCZZT";
	std::string s2 = "GACTCTGAT";
	std::cout << std::format("Dynamic LCS({}, {}) = {} with {} steps\n", s1, s2, LCSDynamic(s1, s2), s1.size() * s2.size());
	printArray<n>(LCSCache);
	std::cout << std::format("Recursive LCS({}, {}) = {} with {} calls\n", s1, s2, computeLCS(s1, s2), LCSRecursiveCalls);
	printArray<n>(LCSRecursiveCache);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}