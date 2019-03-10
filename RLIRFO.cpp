#include <iostream>
#include <stack>
#include <optional>
using namespace std;

template <class T, unsigned int n>
struct baseType {
	typedef typename baseType<T, n - 1>::type type_prev;
	typedef stack<type_prev> type;
};

template <class T>
struct baseType<T, 1> {
	typedef stack<pair<stack<T>, optional<T>>> type;
};

template <class T, unsigned int n>
struct historyStack {
	typedef typename baseType<T, n>::type type;
	type datos;

	void push(T val) {
		if (datos.empty()) datos = initialize<n>(val);
		else pushn<n>(val, datos);
		isEmpty = false;
	}

	void pop() {
		if (isEmpty) return;
		else popn<n>(datos);
	}
	
	bool isCorrect() {
		return isCorrectn<n>(datos);
	}
	

private:
	bool isEmpty = true;

	template <unsigned int depth>
	typename baseType<T, depth>::type initialize(T val) {
		typename baseType<T, depth>::type st;
		if constexpr (depth > 1) {
			typename baseType<T, depth - 1>::type st2 = initialize<depth - 1>(val);
			st.push(st2);
		}
		else {
			stack<T> st2;
			st2.push(val);
			st.push(make_pair(st2, nullopt));
		}
		return st;
	}

	template <unsigned int depth>
	void pushn(T val, typename historyStack<T, depth>::type &data) {
		if constexpr (depth > 1) {
			data.push(data.top());
			pushn<depth - 1>(val, data.top());
		}
		else {
			auto tmp = data.top().first;
			tmp.push(val);
			data.push(make_pair(tmp, nullopt));
		}
	}

	template <unsigned int depth>
	void popn(typename historyStack<T, depth>::type &data) {
		if constexpr (depth > 1) {
			data.push(data.top());
			popn<depth - 1>(data.top());
		}
		else {
			auto tmp = data.top().first;
			auto popVal = tmp.top();
			tmp.pop();
			data.push(make_pair(tmp, popVal));
			if (tmp.empty()) isEmpty = true;
		}
	}
	
	template <unsigned int depth>
	bool isCorrectn(typename historyStack<T, depth>::type data) {
		bool correct = true;
		if constexpr (depth > 1) {
			auto data2 = data.top();
			// check current level (same for push & pop)
			while (!data.empty() && correct) {
				auto tmp = data.top();
				data.pop();
				tmp.pop();
				if (data.empty()) {
					if (!tmp.empty()) correct = false;
					continue;
				}
				if (data.top() != tmp) correct = false;
			}
			// recursion
			correct = correct && isCorrectn<depth - 1>(data2);
		}
		// case for base level
		else{
			while (!data.empty() && correct) {
				auto tmp = data.top().first;
				auto tmpopt = data.top().second;
				data.pop();
				// if push detected
				if (tmpopt == nullopt) {
					tmp.pop();
					if (data.empty()) {
						if (!tmp.empty()) correct = false;
						continue;
					}
					if (data.top().first != tmp) correct = false;
				}
				// if pop detected
				else {
					tmp.push(tmpopt.value());
					if (data.top().first != tmp) correct = false;
				}
			}
		}
		return correct;
	}
};

int main() {
	const int n = 2;
	int x, num=10;
	char mander;
	historyStack<int, n> history;

	/*
	while (0, 1) {
		cin >> mander >> x;
		if (mander == 'i') history.push(x);
		else if (mander == 'o') history.pop();
		else continue;
		if (history.datos.empty()) continue;
		num--;
		if (num == 0) {
			cout << "--------------" << endl;
			while (!history.datos.empty()) {
				auto tmp = history.datos.top().first;
				if (tmp.empty()) cout << "NA";
				while (!tmp.empty()) {
					cout << tmp.top() << " ";
					tmp.pop();
				}
				cout << endl;
				history.datos.pop();
			}
		}
	}
	*/
	
	while (0, 1) {
		cin >> mander >> x;
		if (mander == 'i') history.push(x);
		else if (mander == 'o') history.pop();
		else continue;
		if (history.datos.empty()) continue;
		cout << history.isCorrect() << endl;
		num--;
		if (num == 0) {
			cout << "--------------" << endl;
			cout << (history.isCorrect() == 1 ? "No cosmic ray found" : "Cosmic ray found") << endl;
			cout << "--------------" << endl;
			while (!history.datos.empty()) {
				auto tmp = history.datos.top();
				while (!tmp.empty()) {
					auto tmp2 = tmp.top().first;
					if (tmp2.empty()) cout << "NA";
					while (!tmp2.empty()) {
						cout << tmp2.top() << " ";
						tmp2.pop();
					}
					cout << endl;
					tmp.pop();
				}
				cout << endl;
				history.datos.pop();
			}
		}
	}
	
	/*
	while (0, 1) {
		cin >> mander >> x;
		if (mander == 'i') history.push(x);
		else if (mander == 'o') history.pop();
		else continue;
		if (history.datos.empty()) continue;
		num--;
		if (num == 0) {
			cout << "--------------" << endl;
			cout << (history.isCorrect() == 1 ? "No cosmic ray found" : "Cosmic ray found") << endl;
			cout << "--------------" << endl;
			while (!history.datos.empty()) {
				auto tmp = history.datos.top();
				while (!tmp.empty()) {
					auto tmp2 = tmp.top();
					while (!tmp2.empty()) {
						auto tmp3 = tmp2.top().first;
						if (tmp3.empty()) cout << "NA";
						while (!tmp3.empty()) {
							cout << tmp3.top() << " ";
							tmp3.pop();
						}
						cout << endl;
						tmp2.pop();
					}
					cout << endl;
					tmp.pop();
				}
				cout << endl;
				history.datos.pop();
			}
		}
	}
	*/
}

// Hacer check