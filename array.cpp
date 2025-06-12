#include <iostream>
using namespace std;

int main() {
    const int SIZE = 5;            // Ukuran array
    int angka[SIZE];               // Deklarasi array

    // Input data
    cout << "Masukkan " << SIZE << " angka:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << "Angka ke-" << (i + 1) << ": ";
        cin >> angka[i];
    }

    // Menampilkan data
    cout << "\nAngka yang Anda masukkan:" << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << "angka[" << i << "] = " << angka[i] << endl;
    }

    return 0;
}
