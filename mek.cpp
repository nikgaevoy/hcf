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

		// assert(cin.eof());
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

	void write(problem &prb, ostream &cout = std::cout)
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


struct interactor
{
	enum gift_status
	{
		Unloaded = 0, Loaded, Delivered
	};
	problem &prb;
	solution slv;
	vector<gift_status> gss;
	pnt pos, vel;
	int weight = 0, carrots = 0;
	ll score = 0;

	explicit interactor(problem &prb) : prb(prb), gss(prb.gifts.size(), Unloaded)
	{}

	void acc_left(int val)
	{
		slv.moves.emplace_back(AccLeft, val);
		vel += dpnt[AccLeft] * val;
		carrots--;
		weight--;
	}

	void acc_right(int val)
	{
		slv.moves.emplace_back(AccRight, val);
		vel += dpnt[AccRight] * val;
		carrots--;
		weight--;
	}

	void acc_up(int val)
	{
		slv.moves.emplace_back(AccUp, val);
		vel += dpnt[AccUp] * val;
		carrots--;
		weight--;
	}

	void acc_down(int val)
	{
		slv.moves.emplace_back(AccDown, val);
		vel += dpnt[AccDown] * val;
		carrots--;
		weight--;
	}

	void load_carrots(int val)
	{
		slv.moves.emplace_back(LoadCarrots, val);
		weight += val;
		carrots += val;
	}

	void load_gift(int id)
	{
		slv.moves.emplace_back(LoadGift, id);
		weight += prb.gifts[id].weight;
		if (gss[id] != Unloaded)
			throw runtime_error("Gift load fail");
		gss[id] = Loaded;
	}

	void idle(int time)
	{
		slv.moves.emplace_back(Float, time);
		pos += vel * time;
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

	ll get_score() const
	{
		return score;
	}

	void write(ostream &cout = std::cout)
	{
		slv.write(prb, cout);
	}
};


int main()
{
	ifstream ex("../f_festive_flyover.in.txt");

	problem prb(ex);
    interactor inter(prb);
    inter.load_carrots(4);
    inter.acc_up(1);
    inter.idle(1);
    inter.acc_right(1);
    int g = prb.gifts.size();
    vector<int> taken(g);
    const int KEK = 180;
    for (int i = 0; i < g; i++) {
        bool ok = false;
        for (int j = 1; j <= KEK; j++)
            ok |= prb.check_distance(prb.gifts[i].pos, {j, j - 1});
        if (ok) {
            inter.load_gift(i);
            taken[i] = 1;
        }
    }
    int total = 1;
    while (inter.pos.real() < KEK) {
        inter.idle(1);
        total += 1;
        for (int i = 0; i < prb.gifts.size(); i++) {
            if (taken[i] == 1 && prb.check_distance(prb.gifts[i].pos, inter.pos)) {
                taken[i] = -1;
                inter.deliver_gift(i);
            }
        }
    }
    inter.acc_down(2);
    inter.idle(1);
    inter.acc_left(2);
    total += 1;
    while (total < prb.tl) {
        inter.idle(1);
        total += 1;
        if (prb.check_distance(inter.pos, {0, 0}))
            for (int i = 0; i < prb.gifts.size(); i++)
                if (taken[i] == 0) {
                    inter.load_gift(i);
                    taken[i] = 1;
                }
        for (int i = 0; i < prb.gifts.size(); i++) {
            if (taken[i] == 1 && prb.check_distance(prb.gifts[i].pos, inter.pos)) {
                taken[i] = -1;
                inter.deliver_gift(i);
            }
        }
    }
    cerr << inter.get_score() << endl;
    
    inter.write(cout);
	return 0;
}
