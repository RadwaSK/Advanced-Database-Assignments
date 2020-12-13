#include "Directory.h"

int main(){
    Directory dir;
    dir.insert(2369, 1);
    dir.insert(3760, 2);
    dir.insert(4692, 3);
    dir.insert(4871, 4);
    dir.insert(5659, 5);
    dir.insert(1821, 6);
    dir.insert(1074, 7);
    dir.insert(7115, 8);
    dir.insert(1620, 9);
    dir.insert(2428, 10);
	dir.insert(3943, 11);
	dir.insert(4750, 12);
	dir.insert(6975, 13);

	dir.display();
	cout << endl;
	

	dir.search(1620);
	dir.search(5555);
	dir.search(3760);
	cout << endl;

	dir.remove(4692); // rec3 - no changes
	dir.remove(6975); //rec13 - merge then shrink

	dir.display();

	system("pause");
    return 0;
}