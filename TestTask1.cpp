#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cstdlib>

using namespace std;
mutex mtx;

bool comp(int a, int b) { return (a > b); }
bool comp2(int a) { return (a % 2 == 0); }

void correctness(string &str) {
	if (str.size() > 64) {
		cout << "Ошибка: Кол-во символом больше 64.\n";
		exit(0);
	}
	else
	{   
		for (int i = 0; i < str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') {
				cout << "Ошибка: Данные введены некорректно.\n";
				exit(0);
			}
		}
	}
}

void FirstStream(string &str, string &buffer) {
	string str2 = "K";
	correctness(str);
	sort(str.begin(), str.end(), comp);
	cout << str << endl << endl;

	replace_if(str.begin(), str.end(), comp2, 'K');
	cout << str << endl << endl;

	for (size_t i = 0; i < str.size(); i++) {
		for (auto s : str2) {
			if (str[i] == s) {
				str.insert(i + 1, "B");
			}
		}
	}
	mtx.lock();
	buffer = str;
	cout << str << endl;
	cout << buffer << endl;
	mtx.unlock();
}

void SecondStream(string *buffer) {
	int result = 0;

	mtx.lock();
	string str1 = *buffer;
	free(buffer);
	//buffer = nullptr;
	mtx.unlock();
	
	cout << str1 << endl << endl;
	for (int i = 0; str1[i] != '\0'; i++) {
		if (str1[i] > '0' && str1[i] <= '9') {
			result += (str1[i] - '0');
		}
	}
	cout << result << endl << endl;
}

int main() {
	setlocale(LC_ALL, "ru");
	string str;
	cout << "Введите строку: " << endl;
	cin >> str;
	
	const int BUFFER_SIZE = 256; // Define the buffer size
	string buffer[BUFFER_SIZE];
	//string* buffer = (string*)malloc(BUFFER_SIZE);
	thread th1(FirstStream, ref(str), ref(buffer));
	//thread th2;
	th1.join();
	mtx.lock();
	//if (buffer != nullptr)
		//SecondStream(buffer);
	mtx.unlock();
	//thread th2(SecondStream, ref(buffer));

	//FirstStream(str, *buffer);
	
	//th2.join();
	//free(buffer);
	cout << str << endl;
	return 0;
}