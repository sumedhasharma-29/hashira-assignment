#include <bits/stdc++.h>
#include "json.hpp"  // nlohmann/json single header
using namespace std;
using json = nlohmann::json;

struct Point {
    long long x;
    long double y;
};

// Convert string `val` in given `base` to decimal
long double baseToDecimal(const string &val, int base) {
    long double num = 0;
    for (char c : val) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'z') digit = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'Z') digit = 10 + (c - 'A');
        else digit = 0;
        if (digit >= base) {
            cerr << "Invalid digit '" << c << "' for base " << base << endl;
            exit(1);
        }
        num = num * base + digit;
    }
    return num;
}

// Lagrange interpolation at x=0 → gives constant term c
long double lagrangeConstant(vector<Point>& points, int k) {
    long double c = 0.0;
    for (int i = 0; i < k; i++) {
        long double term = points[i].y;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - points[j].x) / (long double)(points[i].x - points[j].x);
            }
        }
        c += term;
    }
    return c;
}

int main() {
    // Read JSON input from stdin
    string input, line;
    while (getline(cin, line)) {
        input += line;
    }

    json data = json::parse(input);

    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    vector<Point> points;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it.key() == "keys") continue;
        long long x = stoll(it.key()); // x-coordinate from JSON key
        int base = stoi(it.value()["base"].get<string>());
        string val = it.value()["value"].get<string>();
        long double y = baseToDecimal(val, base);
        points.push_back({x, y});
    }

    // Sort by x (in case keys are unordered)
    sort(points.begin(), points.end(), [](auto &a, auto &b){ return a.x < b.x; });

    // Use first k points only
    vector<Point> usePoints(points.begin(), points.begin() + k);

    long double c = lagrangeConstant(usePoints, k);

    cout << fixed << setprecision(0) << c << endl;
    return 0;
}
