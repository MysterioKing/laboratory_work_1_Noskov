#include <iostream>

using namespace std;


int main() {
	int flag;
	cout << "Menu\n\nPrint number for continue\n\n1. Add pipe \n2. Add KC \n3. Check all objects \n4. Edit pipe \n5. Edit KC \n6. Save \n7. Upload \n0. Exit\n" << endl;
	cin >> flag;
	while (flag != 0) {
		if (flag == 1) {
			cout << "Add pipe:\n" << endl;
		}
		else if (flag == 2) {
			cout << "Add KC:\n" << endl;
		}
		else if (flag == 3) {
			cout << "Check all objects:\n" << endl;
		}
		else if (flag == 4) {
			cout << "Edit pipe:\n" << endl;
		}
		else if (flag == 5) {
			cout << "Edit KC:\n" << endl;
		}
		else if (flag == 6) {
			cout << "Save\n" << endl;
		}
		else if (flag == 7) {
			cout << "Upload\n" << endl;
		}
		else {
			cout << "Error, this comand doesn't exist!\n" << endl;
		}
		cout << "Menu\n\nPrint number for continue\n\n1. Add pipe \n2. Add KC \n3. Check all objects \n4. Edit pipe \n5. Edit KC \n6. Save \n7. Upload \n0. Exit\n" << endl;
		cin >> flag;
	}
}