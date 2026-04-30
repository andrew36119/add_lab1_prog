#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

vector<string> split(const string& s) {
    vector<string> words;
    stringstream ss(s);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}


vector<string> readNames(ifstream& fin) {
    string firstLine;
    getline(fin, firstLine);
    vector<string> parts = split(firstLine);

    vector<string> names;
    for (int i = 1; i < parts.size(); i++) {
        names.push_back(parts[i]);
    }
    return names;
}


set<string> getExcludedParticipants(const string& line) {
    set<string> excluded;
    size_t slashPos = line.find('/');
    if (slashPos != string::npos) {
        string afterSlash = line.substr(slashPos + 1);
        vector<string> excludedNames = split(afterSlash);
        for (string ex : excludedNames) {
            excluded.insert(ex);
        }
    }
    return excluded;
}

vector<string> getIncludedParticipants(const vector<string>& allNames, const set<string>& excluded) {
    vector<string> included;
    for (string name : allNames) {
        if (excluded.find(name) == excluded.end()) {
            included.push_back(name);
        }
    }
    return included;
}

void distributeExpense(const vector<string>& included, double amount, map<string, double>& should) {
    if (included.empty()) return;

    double sharePerPerson = amount / included.size();
    for (string name : included) {
        should[name] += sharePerPerson;
    }
}

void processExpenseLine(const string& line,
    const vector<string>& allNames,
    map<string, double>& spent,
    map<string, double>& should) {
    vector<string> tokens = split(line);
    if (tokens.size() < 2) return;

    string who = tokens[0];
    double amount = stod(tokens[1]);

    spent[who] += amount;

    set<string> excluded = getExcludedParticipants(line);
    vector<string> included = getIncludedParticipants(allNames, excluded);

    distributeExpense(included, amount, should);
}


void calculateBalances(const vector<string>& names,
    const map<string, double>& spent,
    const map<string, double>& should,
    vector<pair<string, double>>& debtors,
    vector<pair<string, double>>& creditors) {
    for (string name : names) {
        double balance = should.at(name) - spent.at(name);

        if (balance > 0.001) {
            creditors.push_back(make_pair(name, balance));
        }
        else if (balance < -0.001) {
            debtors.push_back(make_pair(name, -balance));
        }
    }
}

void sortByAmount(vector<pair<string, double>>& debtors, vector<pair<string, double>>& creditors) {
    sort(debtors.begin(), debtors.end(),
        [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second > b.second;
        });
    sort(creditors.begin(), creditors.end(),
        [](const pair<string, double>& a, const pair<string, double>& b) {
            return a.second > b.second;
        });
}


void makeTransactions(vector<pair<string, double>>& debtors,
    vector<pair<string, double>>& creditors,
    ofstream& fout) {
    int i = 0, j = 0;
    while (i < debtors.size() && j < creditors.size()) {
        double sum = min(debtors[i].second, creditors[j].second);

        fout << fixed << setprecision(1);
        fout << debtors[i].first << " " << sum << " " << creditors[j].first << endl;

        debtors[i].second -= sum;
        creditors[j].second -= sum;

        if (debtors[i].second < 0.001) i++;
        if (creditors[j].second < 0.001) j++;
    }
}


void printExpensesAndNorms(const vector<string>& names,
    const map<string, double>& spent,
    const map<string, double>& should,
    ofstream& fout) {
    for (string name : names) {
        fout << fixed << setprecision(1);
        fout << name << " " << spent.at(name) << " " << should.at(name) << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    if (!fin.is_open()) {
        cout << "Не найден файл input.txt" << endl;
        return 1;
    }

    vector<string> names = readNames(fin);

    map<string, double> spent;
    map<string, double> should;

    for (string name : names) {
        spent[name] = 0;
        should[name] = 0;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        processExpenseLine(line, names, spent, should);
    }
    fin.close();

    printExpensesAndNorms(names, spent, should, fout);

    vector<pair<string, double>> debtors;
    vector<pair<string, double>> creditors;
    calculateBalances(names, spent, should, debtors, creditors);

    sortByAmount(debtors, creditors);

    makeTransactions(debtors, creditors, fout);

    fout.close();

    return 0;
}





/*#include <iostream>
#include <string>

using namespace std;
string new_digit(int a, int b) {
	string A = to_string(a);
	char B = b + '0';
	string res = "";
	for (char i : A) {
		if (i != B) res += i;
	}
	return res;
}
int main()
{
	int a,b;
	cin >> a>>b;
	cout << new_digit(a, b);
}*/










/*
bool lucky_ticket(int x) {
	int num1 = x / 100000;
	int num2 = (x / 10000) % 10;
	int num3 = (x / 1000) % 10;
	int num4 = (x / 100) % 10;
	int num5 = (x / 10) % 10;
	int num6 = x % 10;
	return ((num1 + num2 + num3) == (num4 + num5 + num6));

}

int main() {
	int x;
	cin >> x;
	cout << std::boolalpha<< lucky_ticket(x);
}*/

