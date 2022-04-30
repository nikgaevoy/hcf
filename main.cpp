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

struct problem
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

	explicit problem(istream &cin = std::cin)
	{
		int w, g;

		cin >> tl >> d >> w >> g;

		l.resize(w);
		a.resize(w);
		names.resize(g);
		who.reserve(g);
		gifts.resize(g);

		for (int i = 0; i < w; i++)
			cin >> l[i] >> a[i];

		for (int i = 0; i < g; i++)
		{
			cin >> names[i];
			who[names[i]] = i;

			int score, weight;
			int x, y;

			cin >> score >> weight >> x >> y;

			gifts[i] = {i, score, weight, pnt{x, y}};
		}
	}


	bool check_distance(const pnt &x, const pnt &y) const
	{
		return len2(x - y) <= d * (ll) d;
	}
};

constexpr array dx = {0, 0, -1, 1}, dy = {1, -1, 0, 0};
constexpr array<pnt, 4> dpnt = {pnt{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
constexpr array<string_view, 8> action_names = {"AccUp", "AccDown", "AccLeft", "AccRight", "Float", "LoadCarrots",
                                                "LoadGift", "DeliverGift"};

enum action
{
	AccUp = 0, AccDown = 1, AccLeft = 2, AccRight = 3, Float = 4, LoadCarrots = 5, LoadGift = 6, DeliverGift = 7
};

struct solution
{
	vector<pair<action, int>> moves;

	void write(const problem &prb, ostream &cout = std::cout)
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

	solution()
	{}

	solution(const problem &prb, istream &cin)
	{
		read(prb, cin);
	}


	void read(const problem &prb, istream &cin = std::cin)
	{
		int n;

		cin >> n;

		moves.resize(n);

		for (auto &[acc, val]: moves)
		{
			string a;

			cin >> a;

			acc = action(find(action_names.begin(), action_names.end(), string_view{a}) - action_names.begin());

			if (int(acc) == action_names.size())
				throw runtime_error("Bad file format");

			if (acc == LoadGift || acc == DeliverGift)
			{
				string b;

				cin >> b;

				val = prb.who.at(b);
			}
			else
				cin >> val;
		}
	}
};


struct interactor
{
	enum gift_status
	{
		Unloaded = 0, Loaded, Delivered
	};
	const problem &prb;
	solution slv;
	vector<gift_status> gss;
	pnt pos, vel;
	int weight = 0, carrots = 0;
	ll score = 0;
	int cur_time = 0;

	explicit interactor(const problem &prb) : prb(prb), gss(prb.gifts.size(), Unloaded)
	{}

	explicit interactor(const problem &prb, const solution &base) : prb(prb), gss(prb.gifts.size(), Unloaded)
	{
		for (auto [acc, val]: base.moves)
		{
			switch (acc)
			{
				case AccLeft:
					acc_left(val);
					break;
				case AccUp:
					acc_up(val);
					break;
				case AccDown:
					acc_down(val);
					break;
				case AccRight:
					acc_right(val);
					break;
				case Float:
					idle(val);
					break;
				case LoadCarrots:
					load_carrots(val);
					break;
				case LoadGift:
					load_gift(val);
					break;
				case DeliverGift:
					deliver_gift(val);
					break;
			}
		}
	}

	void acc_left(int val)
	{
		slv.moves.emplace_back(AccLeft, val);
		vel += dpnt[AccLeft] * val;
		if (carrots <= 0)
			throw runtime_error("No carrots");
#ifndef NO_SPEED_CHECKS
		if (auto wh = lower_bound(prb.l.begin(), prb.l.end(), weight);
				wh == prb.l.end() || val > prb.a[wh - prb.l.begin()])
			throw runtime_error("Too fast");
#endif
		carrots--;
		weight--;
	}

	void acc_right(int val)
	{
		slv.moves.emplace_back(AccRight, val);
		vel += dpnt[AccRight] * val;
		if (carrots <= 0)
			throw runtime_error("No carrots");
#ifndef NO_SPEED_CHECKS
		if (auto wh = lower_bound(prb.l.begin(), prb.l.end(), weight);
				wh == prb.l.end() || val > prb.a[wh - prb.l.begin()])
			throw runtime_error("Too fast");
#endif
		carrots--;
		weight--;
	}

	void acc_up(int val)
	{
		slv.moves.emplace_back(AccUp, val);
		vel += dpnt[AccUp] * val;
		if (carrots <= 0)
			throw runtime_error("No carrots");
#ifndef NO_SPEED_CHECKS
		if (auto wh = lower_bound(prb.l.begin(), prb.l.end(), weight);
				wh == prb.l.end() || val > prb.a[wh - prb.l.begin()])
			throw runtime_error("Too fast");
#endif
		carrots--;
		weight--;
	}

	void acc_down(int val)
	{
		slv.moves.emplace_back(AccDown, val);
		vel += dpnt[AccDown] * val;
		if (carrots <= 0)
			throw runtime_error("No carrots");
#ifndef NO_SPEED_CHECKS
		if (auto wh = lower_bound(prb.l.begin(), prb.l.end(), weight);
				wh == prb.l.end() || val > prb.a[wh - prb.l.begin()])
			throw runtime_error("Too fast");
#endif
		carrots--;
		weight--;
	}

	void acc_x(int val)
	{
		if (val < 0)
			acc_left(-val);
		else
			acc_right(val);
	}

	void acc_y(int val)
	{
		if (val < 0)
			acc_down(-val);
		else
			acc_up(val);
	}

	void load_carrots(int val)
	{
		if (!prb.check_distance(pos, pnt{}))
			throw runtime_error("Distance to zero fail");
		slv.moves.emplace_back(LoadCarrots, val);
		weight += val;
		carrots += val;
	}

	void load_gift(int id)
	{
		if (gss[id] != Unloaded)
			throw runtime_error("Gift load fail");
		slv.moves.emplace_back(LoadGift, id);
		weight += prb.gifts[id].weight;
		gss[id] = Loaded;
	}

	void idle(int time)
	{
		slv.moves.emplace_back(Float, time);
		pos += vel * time;
		cur_time += time;

		if (cur_time > prb.tl)
			throw runtime_error("TL");
	}

	void deliver_gift(int id)
	{
		if (gss[id] != Loaded)
			throw runtime_error("Gift deliver fail");
		if (!prb.check_distance(prb.gifts[id].pos, pos))
			throw runtime_error("Distance to delivery fail");
		slv.moves.emplace_back(DeliverGift, id);
		weight -= prb.gifts[id].weight;
		score += prb.gifts[id].score;
		gss[id] = Delivered;
	}

	[[nodiscard]] ll get_score() const
	{
		return score;
	}

	void write(ostream &cout = std::cout)
	{
		slv.write(prb, cout);
	}
};


constexpr array file_names = {"a_an_example.in.txt", "b_better_hurry.in.txt", "c_carousel.in.txt",
                              "d_decorated_houses.in.txt", "e_excellent_weather.in.txt", "f_festive_flyover.in.txt"};


int main()
{
	ifstream ex(string("../") + file_names[3]), out("../output.txt");

	problem prb(ex);
	solution slv(prb, out);

	interactor inter(prb, slv);

	cout << inter.get_score() << endl;

	return 0;
}
