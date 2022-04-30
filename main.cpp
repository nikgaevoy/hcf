#include <bits/stdc++.h>

using namespace std;

using ll = long long;

using pnt = complex<int>;

ll operator&(const pnt &a, const pnt &b)
{
	return a.real() * ll(b.real()) + a.imag() * ll(b.imag());
}

ll len2(const pnt &x)
{
	return x & x;
}

struct Problem
{
	struct gift
	{
		int id{}, score{}, weight{};
		pnt pos;
	};

	int tl, d;
	vector<int> l, a;
	unordered_map<string, int> who;
	vector<string> names;
	vector<gift> gifts;

	explicit Problem(istream &cin = std::cin)
	{
		int w, g;

		cin >> tl >> d >> w >> g;

		l.resize(w);
		a.resize(w);
		names.resize(g);
		who.reserve(g);
		gifts.resize(g);

		for (auto &it: l)
			cin >> it;
		for (auto &it: a)
			cin >> it;

		for (int i = 0; i < g; i++)
		{
			cin >> names[i];
			who[names[i]] = i;

			int score, weight;
			int x, y;

			cin >> score >> weight >> x >> y;

			gifts[i] = {i, score, weight, pnt{x, y}};
		}

		assert(cin.eof());
	}
};

constexpr array dx = {0, 0, -1, 1}, dy = {1, -1, 0, 0};
constexpr array<string_view, 8> action_names = {"AccUp", "AccDown", "AccLeft", "AccRight", "Float", "LoadCarrots",
                                                "LoadGift", "DeliverGift"};

enum action
{
	AccUp = 0, AccDown = 1, AccLeft = 2, AccRight = 3, Float = 4, LoadCarrots = 5, LoadGift = 6, DeliverGift = 7
};

struct Solution
{
	vector<pair<action, int>> moves;

	void write(Problem &prb, ostream &cout = std::cout)
	{
		cout << moves.size() << endl;

		for (auto [act, val]: moves)
		{
			cout << action_names[act] << ' ';

			if (act == LoadGift || act == DeliverGift)
				cout << prb.names[val] << endl;
			else
				cout << val << endl;
		}
	}
};


int main()
{
	ifstream ex("../a_an_example.in.txt");

	Problem prb(ex);

	return 0;
}
