#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <set>
#include <fstream>

using ll = long long;
using ld = long double;

using namespace std;

ofstream logout("log.txt");

struct AccelerationRange {
	ll min_w;
	ll max_w;
	ll speed;

	AccelerationRange(ll w = -1) {
		min_w = w;
		max_w = numeric_limits<ll>().max();
		speed = 0;
	}

	bool in_range(ll w) {
		return min_w < w && w <= max_w;
	}
};

struct AccelerationRanges {
	vector<AccelerationRange> accel;

	AccelerationRanges() {
		accel.push_back(AccelerationRange());
	}

	void insert_range(ll w, ll s) {
		accel.back().max_w = w;
		accel.back().speed = s;
		accel.push_back(AccelerationRange(w));
	}

	ll get_speed(ll w) {
		for (auto r : accel) {
			if (r.in_range(w)) {
				return r.speed;
			}
		}
	}
};

struct Point {
	ll x, y;

	Point(ll pos_x = 0, ll pos_y = 0) {
		x = pos_x;
		y = pos_y;
	}
};

Point operator+(const Point& a, const Point& b) {
	return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b) {
	return Point(a.x - b.x, a.y - b.y);
}

Point operator*(const Point& a, ll b) {
	return Point(a.x * b, a.y * b);
}

Point operator*(ll b, const Point& a) {
	return a * b;
}

istream& operator>>(istream& in, Point& a) {
	return in >> a.x >> a.y;
}

ld dist(const Point& a, const Point& b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

ld manhattan_dist(const Point& a, const Point& b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void naive_move_coord(vector<pair<string, int>>& commands, ll& time_left, ll& carrots_left, ll start_coord, ll end_coord, string move_right, string move_left, ll accel = 100) {
	if (start_coord > end_coord) naive_move_coord(commands, time_left, carrots_left, end_coord, start_coord, move_left, move_right, accel);
	if (start_coord == end_coord) return;
	if (end_coord - start_coord > 9 * accel) {
		ll dist_sqrt = 0;
		while (end_coord - start_coord >= dist_sqrt * dist_sqrt * accel) dist_sqrt++;
		dist_sqrt--;
		for (int i = 0; i < dist_sqrt; i++) {
			commands.emplace_back(move_right, accel);
			commands.emplace_back("Float", 1);
			carrots_left -= 1;
			time_left -= 1;
		}
		for (int i = 0; i < dist_sqrt; i++) {
			commands.emplace_back(move_left, accel);
			commands.emplace_back("Float", 1);
			carrots_left -= 1;
			time_left -= 1;
		}
		start_coord += dist_sqrt * dist_sqrt * accel;
	}
	if (start_coord + accel <= end_coord) {
		commands.emplace_back(move_right, accel);
		commands.emplace_back("Float", (end_coord - start_coord) / accel);
		time_left -= (end_coord - start_coord) / accel;
		carrots_left -= 1;
		start_coord += ((end_coord - start_coord) / accel) * accel;
		commands.emplace_back(move_left, accel - (end_coord - start_coord));
		commands.emplace_back("Float", 1);
		time_left -= 1;
		carrots_left -= 1;
		if (start_coord != end_coord) {
			commands.emplace_back(move_left, end_coord - start_coord);
			commands.emplace_back("Float", 1);
			time_left -= 1;
			carrots_left -= 1;
		}
		start_coord = end_coord;
	}
	if (start_coord < end_coord) {
		commands.emplace_back(move_right, end_coord - start_coord);
		commands.emplace_back("Float", 1);
		commands.emplace_back(move_left, end_coord - start_coord);
		commands.emplace_back("Float", 1);
		time_left -= 2;
		carrots_left -= 2;
		start_coord = end_coord;
	}
}

void naive_move(vector<pair<string, int>>& commands, ll& time_left, ll& carrots_left, Point& start_pos, const Point& end_pos, ll accel = 100) {
	naive_move_coord(commands, time_left, carrots_left, start_pos.x, end_pos.x, "AccRight", "AccLeft", accel);
	naive_move_coord(commands, time_left, carrots_left, start_pos.y, end_pos.y, "AccUp", "AccDown", accel);
	start_pos = end_pos;
	/*	if (start_pos.x + accel <= end_pos.x) {
		commands.emplace_back("AccRight", accel);
		commands.emplace_back("Float", (end_pos.x - start_pos.x) / accel);
		time_left -= (end_pos.x - start_pos.x) / accel;
		carrots_left -= 1;
		start_pos.x += ((end_pos.x - start_pos.x) / accel) * accel;
		commands.emplace_back("AccLeft", accel - (end_pos.x - start_pos.x));
		commands.emplace_back("Float", 1);
		time_left -= 1;
		carrots_left -= 1;
		if (start_pos.x != end_pos.x) {
			commands.emplace_back("AccLeft", end_pos.x - start_pos.x);
			commands.emplace_back("Float", 1);
			time_left -= 1;
			carrots_left -= 1;
		}
		start_pos.x = end_pos.x;
	}

	if (start_pos.y + accel <= end_pos.y) {
		commands.emplace_back("AccUp", accel);
		commands.emplace_back("Float", (end_pos.y - start_pos.y) / accel);
		time_left -= (end_pos.y - start_pos.y) / accel;
		carrots_left -= 1;
		start_pos.y += ((end_pos.y - start_pos.y) / accel) * accel;
		commands.emplace_back("AccDown", accel - (end_pos.y - start_pos.y));
		commands.emplace_back("Float", 1);
		time_left -= 1;
		carrots_left -= 1;
		if (start_pos.y != end_pos.y) {
			commands.emplace_back("AccDown", end_pos.y - start_pos.y);
			commands.emplace_back("Float", 1);
			time_left -= 1;
			carrots_left -= 1;
		}
		start_pos.y = end_pos.y;
	}

	if (start_pos.x - accel >= end_pos.x) {
		commands.emplace_back("AccLeft", accel);
		commands.emplace_back("Float", (start_pos.x - end_pos.x) / accel);
		time_left -= (start_pos.x - end_pos.x) / accel;
		carrots_left -= 1;
		start_pos.x -= ((start_pos.x - end_pos.x) / accel) * accel;
		commands.emplace_back("AccRight", accel - (start_pos.x - end_pos.x));
		commands.emplace_back("Float", 1);
		time_left -= 1;
		carrots_left -= 1;
		if (start_pos.x != end_pos.x) {
			commands.emplace_back("AccRight", start_pos.x - end_pos.x);
			commands.emplace_back("Float", 1);
			time_left -= 1;
			carrots_left -= 1;
		}
		start_pos.x = end_pos.x;
	}

	if (start_pos.y - accel >= end_pos.y) {
		commands.emplace_back("AccDown", accel);
		commands.emplace_back("Float", (start_pos.y - end_pos.y) / accel);
		time_left -= (start_pos.y - end_pos.y) / accel;
		carrots_left -= 1;
		start_pos.y -= ((start_pos.y - end_pos.y) / accel) * accel;
		commands.emplace_back("AccUp", accel - (start_pos.y - end_pos.y));
		commands.emplace_back("Float", 1);
		time_left -= 1;
		carrots_left -= 1;
		if (start_pos.y != end_pos.y) {
			commands.emplace_back("AccUp", start_pos.y - end_pos.y);
			commands.emplace_back("Float", 1);
			time_left -= 1;
			carrots_left -= 1;
		}
		start_pos.y = end_pos.y;
	}

	if (start_pos.x < end_pos.x) {
		commands.emplace_back("AccRight", end_pos.x - start_pos.x);
		commands.emplace_back("Float", 1);
		commands.emplace_back("AccLeft", end_pos.x - start_pos.x);
		commands.emplace_back("Float", 1);
		time_left -= 2;
		carrots_left -= 2;
		start_pos.x = end_pos.x;
	}

	if (start_pos.y < end_pos.y) {
		commands.emplace_back("AccUp", end_pos.y - start_pos.y);
		commands.emplace_back("Float", 1);
		commands.emplace_back("AccDown", end_pos.y - start_pos.y);
		commands.emplace_back("Float", 1);
		time_left -= 2;
		carrots_left -= 2;
		start_pos.y = end_pos.y;
	}

	if (start_pos.x > end_pos.x) {
		commands.emplace_back("AccLeft", start_pos.x - end_pos.x);
		commands.emplace_back("Float", 1);
		commands.emplace_back("AccRight", start_pos.x - end_pos.x);
		commands.emplace_back("Float", 1);
		time_left -= 2;
		carrots_left -= 2;
		start_pos.x = end_pos.x;
	}

	if (start_pos.y > end_pos.y) {
		commands.emplace_back("AccDown", start_pos.y - end_pos.y);
		commands.emplace_back("Float", 1);
		commands.emplace_back("AccUp", start_pos.y - end_pos.y);
		commands.emplace_back("Float", 1);
		time_left -= 2;
		carrots_left -= 2;
		start_pos.y = end_pos.y;
	}*/
}

ll move_carrot_cost(Point start_pos, Point end_pos, ll accel) {
	ll carrots = 0;
	ll time_left = 0;
	vector<pair<string, int>> commands;
	naive_move(commands, time_left, carrots, start_pos, end_pos, accel);
	return -carrots;
}

ll move_time_cost(Point start_pos, Point end_pos, ll accel) {
	ll carrots = 0;
	ll time_left = 0;
	vector<pair<string, int>> commands;
	naive_move(commands, time_left, carrots, start_pos, end_pos, accel);
	return -time_left;
}

ld delivery_score(Point cur_pos, Point delivery_pos, ll score, ll wt, ll accel) {
	return score * 1.0 / (wt * move_time_cost(cur_pos, delivery_pos, accel));
}

void solve(string in_file, string out_file, ll sleight_weight, ll max_accel, ll carrot_quota) {
	ifstream fin(in_file);
	ofstream fout(out_file);

	ll T, D, W, G;
	fin >> T >> D >> W >> G;

	AccelerationRanges accel;

	for (int i = 0; i < W; i++) {
		ll w, s;
		fin >> w >> s;
		accel.insert_range(w, s);
	}

	vector<string> names(G);
	vector<ll> score(G);
	vector<ll> wt(G);
	vector<Point> loc(G);

	for (int i = 0; i < G; i++) {
		fin >> names[i] >> score[i] >> wt[i] >> loc[i];
	}

	ll best_score = 0;

	ll current_carrots = 0;
	Point pos;

	vector<bool> delivered(G);
	ll time_left = T;
	vector<pair<string, int>> commands;

	while (time_left > 0) {
		ll free_wt = sleight_weight - carrot_quota;

		vector<pair<string, int>> temp_commands;

		while (free_wt > 0) {
			int best_delivery = -1;
			ld best_delivery_score = -1;
			for (int i = 0; i < G; i++) {
				if (!delivered[i] && wt[i] + move_carrot_cost(pos, loc[i], max_accel) <= free_wt && delivery_score(pos, loc[i], score[i], wt[i], max_accel) > best_delivery_score) {
					best_delivery = i;
					best_delivery_score = delivery_score(pos, loc[i], score[i], wt[i], max_accel);
				}
			}
			if (best_delivery == -1) break;
			free_wt -= wt[best_delivery] + move_carrot_cost(pos, loc[best_delivery], max_accel);
			vector<pair<string, int>> temp_temp_commands;
			naive_move(temp_temp_commands, time_left, current_carrots, pos, loc[best_delivery], max_accel);
			if (time_left < 0) break;
			for (auto c : temp_temp_commands) {
				temp_commands.push_back(c);
			}

			commands.emplace_back("LoadGift", best_delivery);
			temp_commands.emplace_back("DeliverGift", best_delivery);
			best_score += score[best_delivery];
			delivered[best_delivery] = true;
		}

		if (free_wt == sleight_weight - carrot_quota) break;

		commands.emplace_back("LoadCarrots", - current_carrots + carrot_quota);
		current_carrots = carrot_quota;

		for (auto c : temp_commands) {
			commands.push_back(c);
		}
		temp_commands.clear();

		naive_move(temp_commands, time_left, current_carrots, pos, Point(), max_accel);
		if (time_left < 0) break;
		for (auto c : temp_commands) {
			commands.push_back(c);
		}

		if (current_carrots < 0) logout << "Not enough carrots!" << endl;

	}

	logout << best_score << endl;
	fout << commands.size() << endl;
	for (auto c : commands) {
		fout << c.first << ' ';
		if (c.first == "LoadGift" || c.first == "DeliverGift") fout << names[c.second] << endl;
		else fout << c.second << endl;
	}
}


void analyze(string in_file, string out_file) {
	ifstream fin(in_file);
	ofstream fout(out_file);

	ll T, D, W, G;
	fin >> T >> D >> W >> G;

	AccelerationRanges accel;

	for (int i = 0; i < W; i++) {
		ll w, s;
		fin >> w >> s;
		accel.insert_range(w, s);
	}

	vector<string> names(G);
	vector<ll> score(G);
	vector<ll> wt(G);
	vector<Point> loc(G);

	for (int i = 0; i < G; i++) {
		fin >> names[i] >> score[i] >> wt[i] >> loc[i];
	}

	for (int i = 0; i < G; i++) {
		if (loc[i].x > 0 && loc[i].y > 0) {
			fout << score[i] << '\t' << 1.1 * loc[i].x - loc[i].y << endl;
		}
	}
}


int main() {
	//analyze("../b_better_hurry.in.txt", "b_analyze.txt");

	solve("../a_an_example.in.txt", "a_semenar.txt", 8, 15, 5);
	solve("../b_better_hurry.in.txt", "b_semenar.txt", 2000, 20, 100);
	solve("../c_carousel.in.txt", "c_semenar.txt", 10000, 4, 100);
	solve("../d_decorated_houses.in.txt", "d_semenar.txt", 1000, 100, 100);
	solve("../e_excellent_weather.in.txt", "e_semenar.txt", 16000, 10, 100);
	solve("../f_festive_flyover.in.txt", "f_semenar.txt", 10000, 4, 100);

	return 0;
}