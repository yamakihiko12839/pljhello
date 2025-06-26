#include <iostream>
#include <limits>   // Diperlukan untuk std::numeric_limits
#include <vector>   // Masih dibutuhkan untuk std::numeric_limits (meskipun kita tidak pakai std::vector untuk keranjang)
#include <string>
#include <iomanip>  // Diperlukan untuk std::setw, std::fixed, std::setprecision

#ifdef _WIN32
#include <windows.h> // Untuk SetConsoleTextAttribute dan system("cls")
#else
#include <unistd.h>  // Untuk sleep() di Linux/Unix jika diperlukan, meski tidak dipakai langsung di sini
#endif

using namespace std;

// --- Struktur Produk ---
struct Produk {
    string nama;
    double harga;
    int kuantitas;

    // Default constructor
    Produk() : nama(""), harga(0.0), kuantitas(0) {}

    // Parameterized constructor
    Produk(string n, double h, int q) : nama(n), harga(h), kuantitas(q) {}
};

// --- Struktur Node untuk Linked List ---
struct Node {
    Produk data;
    Node* next;

    Node(Produk p) : data(p), next(nullptr) {}
};

// --- Kelas Keranjang Belanja (Menggunakan Linked List) ---
class KeranjangBelanja {
private:
    Node* head; // Pointer ke node pertama (kepala) linked list
    double totalBelanjaCache; // Cache untuk total belanja agar tidak hitung ulang terus

public:

    KeranjangBelanja() : head(nullptr), totalBelanjaCache(0.0) {}

    // Destruktor untuk membersihkan memori
    ~KeranjangBelanja() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr; // Pastikan head null setelah dihapus semua
    }

    // Menambahkan produk ke keranjang (ke akhir linked list)
    void tambahProduk(Produk p) {
        Node* newNode = new Node(p);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        totalBelanjaCache += p.harga * p.kuantitas;
    }

    
    bool hapusProduk(int index) {
        if (head == nullptr) {
            return false; // Keranjang kosong
        }

        Node* current = head;
        Node* prev = nullptr;
        int count = 1;

        // Cari node yang akan dihapus
        while (current != nullptr && count < index) {
            prev = current;
            current = current->next;
            count++;
        }

        if (current == nullptr) {
            return false; // Indeks tidak ditemukan (index terlalu besar)
        }

        // Hapus node
        totalBelanjaCache -= current->data.harga * current->data.kuantitas;
        if (prev == nullptr) { // Menghapus head
            head = current->next;
        } else {
            prev->next = current->next;
        }
        delete current;
        return true;
    }

    // Mengembalikan total belanja yang di-cache
    double getTotalBelanjaCache() const {
        return totalBelanjaCache;
    }

    // Mengecek apakah keranjang kosong
    bool isEmpty() const {
        return head == nullptr;
    }

    
    // Fungsi ini dibuat agar tampilan tetap sama.
    Produk getProdukAt(int index) const {
        Node* current = head;
        int count = 0;
        while (current != nullptr) {
            if (count == index) {
                return current->data;
            }
            current = current->next;
            count++;
        }
        
        return Produk();
    }

    // Mendapatkan ukuran keranjang (jumlah produk)
    size_t getSize() const {
        size_t count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    // Mengosongkan keranjang
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        totalBelanjaCache = 0.0;
    }
};

// Objek keranjang belanja global
KeranjangBelanja keranjang; // Objek dari kelas KeranjangBelanja

// Membersihkan input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Fungsi untuk membersihkan layar
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    
    cout << "\033[2J\033[H";
#endif
}

// Fungsi pewarnaan teks di terminal (disesuaikan agar lebih mudah dibaca)
void warna(int kode) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), kode);
#else
    
    cout << "\033[1;" << kode << "m"; // 1; untuk bold
#endif
}

// Reset warna ke default
void resetWarna() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Default color (White)
#else
    cout << "\033[0m"; // Reset to default
#endif
}

// --- Implementasi Fungsi Menu ---

void tambahProduk() {
    clearScreen();
    Produk produk;
    warna(33); cout << "--- Tambah Produk ---" << endl; resetWarna(); // Yellow
    cout << "Masukkan nama produk: ";
    cin.ignore(); // Clear buffer before getline
    getline(cin, produk.nama);

    cout << "Masukkan harga produk: Rp ";
    while (!(cin >> produk.harga) || produk.harga <= 0) {
        warna(31); // Red
        cout << "Harga tidak valid. Coba lagi: Rp ";
        resetWarna();
        clearInputBuffer();
    }

    cout << "Masukkan jumlah produk: ";
    while (!(cin >> produk.kuantitas) || produk.kuantitas <= 0) {
        warna(31); // Red
        cout << "Jumlah tidak valid. Coba lagi: ";
        resetWarna();
        clearInputBuffer();
    }

    keranjang.tambahProduk(produk); // Memanggil method dari objek keranjang

    warna(32); // Green
    cout << "\nProduk berhasil ditambahkan!\n";
    resetWarna();
    cin.ignore(); cin.get(); // pause
}

void tampilkanKeranjang() {
    clearScreen();
    warna(36); cout << "=== Isi Keranjang Belanja ===" << endl; resetWarna(); // Cyan

    if (keranjang.isEmpty()) { // Memanggil method isEmpty() dari objek keranjang
        warna(31); // Red
        cout << "Keranjang masih kosong.\n";
        resetWarna();
        cin.ignore(); cin.get();
        return;
    }

    cout << left << setw(4) << "No"
         << left << setw(25) << "Nama Produk"
         << right << setw(10) << "Harga"
         << right << setw(10) << "Qty"
         << right << setw(15) << "Subtotal" << endl;
    cout << string(64, '-') << endl;

    double total = 0;
    // Iterasi melalui linked list untuk menampilkan produk
    // Perbaikan: Gunakan keranjang.getSize() dan keranjang.getProdukAt()
    for (size_t i = 0; i < keranjang.getSize(); ++i) {
        Produk p = keranjang.getProdukAt(i);
        double subtotal = p.harga * p.kuantitas;
        total += subtotal;

        cout << left << setw(4) << (i + 1)
             << left << setw(25) << p.nama
             << right << setw(10) << fixed << setprecision(0) << p.harga
             << right << setw(10) << p.kuantitas
             << right << setw(15) << subtotal << endl;
    }

    cout << string(64, '-') << endl;
    warna(36); // Cyan
    cout << right << setw(59) << "Total Belanja: Rp " << fixed << setprecision(0) << total << endl;
    resetWarna();

    // Diskon
    double diskon = 0.0;
    if (total >= 20000) {
        diskon = total * 0.2;
        warna(33); // Yellow
        cout << "Diskon 20%: Rp " << fixed << setprecision(0) << diskon << endl;
        resetWarna();
    }

    double totalSetelahDiskon = total - diskon;
    warna(32); // Green
    cout << "Total Setelah Diskon: Rp " << totalSetelahDiskon << endl;
    resetWarna();

    // Pembayaran
    double uangDibayar;
    cout << "\nMasukkan uang dibayarkan: Rp ";
    // totalSetelahDiskon untuk validasi
    while (!(cin >> uangDibayar) || uangDibayar < totalSetelahDiskon) {
        warna(31); // Red
        cout << "Uang tidak cukup. Masukkan nominal yang cukup: Rp ";
        resetWarna();
        clearInputBuffer();
    }

    double kembalian = uangDibayar - totalSetelahDiskon;
    warna(33); // Yellow
    cout << "Kembalian: Rp " << fixed << setprecision(0) << kembalian << endl;
    resetWarna();

    // Setelah pembayaran, keranjang dikosongkan
    keranjang.clear(); 

    cin.ignore(); cin.get(); // pause
}

void hapusProduk() {
    clearScreen();
    if (keranjang.isEmpty()) { // Memanggil method isEmpty() dari objek keranjang
        warna(31); // Red
        cout << "Keranjang kosong.\n";
        resetWarna();
        cin.ignore(); cin.get();
        return;
    }

    // Tampilkan keranjang agar pengguna tahu indeksnya
    warna(36); cout << "=== Isi Keranjang Belanja ===" << endl; resetWarna(); // Cyan
    cout << left << setw(4) << "No"
         << left << setw(25) << "Nama Produk"
         << right << setw(10) << "Harga"
         << right << setw(10) << "Qty"
         << right << setw(15) << "Subtotal" << endl;
    cout << string(64, '-') << endl;

    // Tampilkan item keranjang yang ada sebelum meminta input
    for (size_t i = 0; i < keranjang.getSize(); ++i) {
        Produk p = keranjang.getProdukAt(i);
        double subtotal = p.harga * p.kuantitas;
        cout << left << setw(4) << (i + 1)
             << left << setw(25) << p.nama
             << right << setw(10) << fixed << setprecision(0) << p.harga
             << right << setw(10) << p.kuantitas
             << right << setw(15) << subtotal << endl;
    }
    cout << string(64, '-') << endl;


    cout << "\nMasukkan nomor produk yang akan dihapus: ";
    int idx;
    //Validasi input indeks dengan keranjang.getSize()
    while (!(cin >> idx) || idx < 1 || idx > static_cast<int>(keranjang.getSize())) {
        warna(31); // Red
        cout << "Nomor tidak valid. Masukkan nomor yang benar (1-" << keranjang.getSize() << "): ";
        resetWarna();
        clearInputBuffer();
    }

    if (keranjang.hapusProduk(idx)) { // Memanggil method hapusProduk() dari objek keranjang
        warna(32); // Green
        cout << "\nProduk berhasil dihapus.\n";
        resetWarna();
    } else {
        warna(31); // Red
        cout << "\nGagal menghapus produk. Mungkin nomor tidak valid.\n";
        resetWarna();
    }
    cin.ignore(); cin.get();
}

int main() {
    int pilihan;

    do {
        clearScreen();
        warna(36); // Cyan (ANSI)
        cout << "=== MENU UTAMA ===" << endl;
        resetWarna();
        cout << "1. Tambah Produk" << endl;
        cout << "2. Lihat Keranjang & Bayar" << endl;
        cout << "3. Hapus Produk" << endl;
        cout << "4. Keluar" << endl;
        cout << "Pilih menu (1-4): ";
        // Perbaikan: Tambahkan validasi input untuk menu utama
        while (!(cin >> pilihan)) {
            warna(31); // Red
            cout << "Input tidak valid. Masukkan angka (1-4): ";
            resetWarna();
            clearInputBuffer();
        }

        switch (pilihan) {
            case 1: tambahProduk(); break;
            case 2: tampilkanKeranjang(); break;
            case 3: hapusProduk(); break;
            case 4:
                warna(33); // Yellow (ANSI)
                cout << "\nTerima kasih telah menggunakan aplikasi!\n";
                resetWarna();
                break;
            default:
                warna(31); // Red (ANSI)
                cout << "Pilihan tidak tersedia.\n";
                resetWarna();
                cin.ignore(); cin.get();
        }
    } while (pilihan != 4);

    return 0;
}