#include <iostream>
#include <string>

using namespace std;

struct Pipe {
	string name; // километровая отметка (название)
	double l; // длина (в км)
	int d; // диаметр (в мм)
	bool flag; // признак "в ремонте"
};

struct CompressionStation {
	string name; // название
	int counts; // количество цехов
	string classStation; // класс станции
};


int main() {
	int flag;
	
	while (true) {
		cout << "Menu\n\nPrint number for continue\n\n1. Add pipe \n2. Add KC \n3. Check all objects \n4. Edit pipe \n5. Edit KC \n6. Save \n7. Upload \n0. Exit\n" << endl;
		cin >> flag;
		switch (flag) {
			case 0:
				return 0;
			case 1:
				cout << "Add pipe:\n" << endl;
				break;

			case 2:
				cout << "Add KC:\n" << endl;
				break;

			case 3:
				cout << "Check all objects:\n" << endl;
				break;

			case 4:
				cout << "Edit pipe:\n" << endl;
				break;

			case 5:
				cout << "Edit KC:\n" << endl;
				break;

			case 6:
				cout << "Save\n" << endl;
				break;

			case 7:
				cout << "Upload\n" << endl;
				break;

			default:
				cout << "Print number of exist function!" << endl;
				break;
	}
	}
}