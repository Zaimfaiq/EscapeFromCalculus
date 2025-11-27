#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <string>
using namespace std;
using namespace std::chrono;

const int Max_Y = 100;
const int Max_X = 100;
const int MAX_ANOMALI = 10; //jumlah maksimum anomalinya

char map[Max_X][Max_Y];
int X = 0;
int Y = 0;

void resetMap() {
    for (int i = 0; i < Max_X; i++) {
        for (int j = 0; j < Max_Y; j++) {
            map[i][j] = ' ';   // ngosongin map
        }
    }
    X = 0;
    Y = 0;
}

char afterLevelMenu(int level, bool menang) {
    char pilih;

    if (menang) {
        if (level < 3) {
            cout << "\n=== LEVEL " << level << " SELESAI! ===\n";
            cout << "[1] Lanjut ke Level " << (level+1) << endl;
            cout << "[2] Ulangi Level " << level << endl;
            cout << "[3] Kembali ke Menu\n";
        } else {
            cout << "\n=== SELAMAT! GAME SELESAI ===\n";
            cout << "[1] Ulangi Level 3\n";
            cout << "[2] Kembali ke Menu\n";
        }
    } else {
        cout << "\n=== KAMU KALAH DI LEVEL " << level << " ===\n";
        cout << "[1] Ulangi Level " << level << endl;
        cout << "[2] Kembali ke Menu\n";
    }
    pilih = _getch();
    return pilih;
}

void tampilkanInstruksi() {
    cout << "\n========================================" << endl;
    cout << "       PANDUAN CARA BERMAIN" << endl;
    cout << "========================================" << endl;
    cout << "Misi: Ambil Kunci (K) dan pergi ke Exit (E)." << endl;
    cout << "Jangan Tertangkap Anomali!" << endl;
    cout << "Kontrol: W (Atas), A (Kiri), S (Bawah), D (Kanan)." << endl;
    cout << "Hati Hati dan Perhatikan Map!!!" << endl;
    cout << "========================================\n" << endl;
    cout << "(Tekan tombol apa saja untuk melanjutkan...)";
    _getch();
}

void lose(){
        cout << "==========================================\n";
        cout << "oooooooooooo oooooooo oooooooooo oooooooo" << endl;
        cout << "oooooooooooo oo    oo oooooooooo oooooooo" << endl;
        cout << "ooo      ooo       oo oo  oo  oo oo    oo" << endl;
        cout << "ooo      ooo oooooooo oo  oo  oo oo    oo" << endl;
        cout << "oooooooooooo oooooooo oo  oo  oo oooooooo" << endl;
        cout << "oooooooooooo oo    oo oo  oo  oo oooooooo" << endl;
        cout << "         ooo oo    oo oo  oo  oo oo      " << endl;
        cout << "         ooo oo    oo oo  oo  oo oo    oo" << endl;
        cout << "oo       ooo oooooooo oo  oo  oo oo    oo" << endl;
        cout << "oooooooooooo oooooooo oo  oo  oo oooooooo" << endl;
        cout << "                                         " << endl;
        cout << "oooooooooooo o      o oooooooooo ooooooo " << endl;
        cout << "ooo      ooo o      o oo      oo oo    oo" << endl;
        cout << "ooo      ooo o      o oo      oo oo    oo" << endl;
        cout << "oo       ooo o      o oooooooooo oooooooo" << endl;
        cout << "oo       ooo o      o oooo       ooooooo " << endl;
        cout << "oo       ooo oo    oo oo         oo    o " << endl;
        cout << "oo       ooo  oo  oo  oo         o     oo" << endl;
        cout << "oo       ooo   oooo   oo      oo o      o" << endl;
        cout << "oooooooooooo    oo    oooooooooo o       o" << endl;
        cout << "==========================================\n";
}
void win(){
        cout << "\n==========================================\n";
        cout << "ooooooooooo    oooo       ooo      oooooooo" << endl;
        cout << "oooooooooooo oo    oo     ooo      oooooooo" << endl;
        cout << "oo        oo o      oo    ooo      o      oo" << endl;
        cout << "oo        oo o      ooo oooooooooo o      oo" << endl;
        cout << "oooooooooooo o      ooo oooooooooo oooooooo" << endl;
        cout << "ooooooooooo  o      ooo   ooo      o       " << endl;
        cout << "oo           o      ooo   ooo      oo      " << endl;
        cout << "o            o      ooo   ooo      oo     o" << endl;
        cout << "oo         o o      ooo    ooooo   o      o" << endl;
        cout << " oooooooooo  o      ooo     oooooo  ooooooo" << endl;
        cout << "                                           " << endl;
        cout << " o      oo      o ooo   oooooo         o   " << endl;
        cout << " oo    oooo    oo ooo  ooooooooo      ooo  " << endl;
        cout << " oo   oooooo   oo      oo      oo    ooooo " << endl;
        cout << " oo   oooooo   oo ooo  oo       oo    ooo  " << endl;
        cout << " ooo  oooooo  ooo ooo  oo       oo    ooo  " << endl;
        cout << " oooooooooooooooo ooo  oo       oo    ooo  " << endl;
        cout << "  oooooooooooooo  ooo  oo       oo         " << endl;
        cout << "                                      ooo  " << endl;
        cout << "                                     ooooo " << endl;
        cout << "                                      ooo  " << endl;
        cout << "                                           " << endl;
        cout << "MAHKOTAMU SEDANG DIPOLES KING! " << endl;
        cout << "============================================\n";
}
struct waktu{
    int detik=0;
    int menit=5;
};

//Untuk dapetin waktu saat ini (milidetik)
long long currentTime(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

//Struct posisi
struct position{
    int y, x;
};

//Struct entitas, kunci
struct entitas {
    int x, y;
    string simbol;
    bool bawakunci;
};

//Struct anomali dengan rute paten
struct anomali{
    int x, y;
    bool active;

    position route[200];
    int routeSize;
    int routeIndex;
    bool move;

    long long lastMoveTime;
    long long speed;

    string anomaliSymbol;
};

//Inisialisasi posisi anomali
void initiateAnomali(anomali &a, position route[], int routeSize, long long speed, string symbol){
    a.routeSize = routeSize;
    for (int i = 0; i < routeSize; i++) {
        a.route[i] = route[i];
    }

    a.y = route[0].y;
    a.x = route[0].x;

    a.routeIndex = 0;
    a.move = true;

    a.lastMoveTime = currentTime();
    a.speed = speed;
    a.anomaliSymbol = symbol;
    a.active = true;
}

//Pergerakan si anomali
void moveAnomali(anomali &a, long long currentT){
    if (!a.active) return;

    long long deltaT = currentT - a.lastMoveTime;
    if (deltaT < a.speed) return;

    if (a.move == true){
        a.routeIndex++;
        if (a.routeIndex == a.routeSize - 1) {
            a.move = false;
        }
    } else {
        a.routeIndex--;
        if (a.routeIndex == 0) {
            a.move = true;
        }
    }
    a.y = a.route[a.routeIndex].y;
    a.x = a.route[a.routeIndex].x;

    a.lastMoveTime = currentT;
}
//Baca file txt external map 
void bacaFilemap(string namaFile, entitas &player){
    ifstream file(namaFile.c_str());
    if (!file.is_open()) {
        cout << "Error: File " << namaFile << " tidak ditemukan!" << endl;
        exit(1);
    }
    string line;
    while (getline(file, line) && X < Max_X){
        for(int i = 0; i < line.length() && i < Max_Y; i++){
            map[X][i] = line[i];
            if (map[X][i] == 'R' || map[X][i] == 'S' || map[X][i] == 'T') {
                map[X][i] = ' '; //rute si anomali dibuat invisible
            }
            if (map[X][i] == 'O') {
                player.x = i;
                player.y = X; //koordinat playernya
                map[X][i] = ' ';
            } else if (map[X][i] == 'A') {
                //anomali ditimpa player kalau koordinatnya sama
                map[X][i] = ' ';
            }
        }
        if (line.length() > Y) {
            Y = line.length();
        }
        X++;
    }
    file.close();
}
//Nmampilin map dengan anomali lebih dari satu
void tampilkanmap(entitas player, anomali anomaliList[], int jumlahAnomali) {
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            bool anomaliDitemukan = false;

            //cek posiisi anomali 
            for (int k = 0; k < jumlahAnomali; k++) {
                if (anomaliList[k].active && i == anomaliList[k].y && j == anomaliList[k].x) {
                    cout << anomaliList[k].anomaliSymbol;
                    anomaliDitemukan = true;
                    break;
                }
            }

            if (!anomaliDitemukan) {
                if (i == player.y && j == player.x){
                    cout << player.simbol;
                } else {
                    cout << map[i][j];
                }
            }
        }
        cout << endl;
    }
}

//Cek collision anomali (kalau papasan)
bool cekCollisionAnomali(entitas &player, anomali anomaliList[], int jumlahAnomali, int &nyawa, bool &game_berjalan, bool &menang){
    for (int i = 0; i < jumlahAnomali; i++) {
        if (anomaliList[i].active && player.x == anomaliList[i].x && player.y == anomaliList[i].y) {
            nyawa--; //nyawa berkurang kalau mereka koordinatnya sama
            if (nyawa > 0){
                player.x = 2;
                player.y = 1; //balik ke posisi awal
                return true; 
            } else {
                game_berjalan = false;
                menang = false;
                return true; //game over kalau nyawanya habis atau kalau sudha ketangkap 2x
            }
        }
    }
    return false;
}

//Gerakan player
void gerakanplayer(entitas &player, anomali anomaliList[], int jumlahAnomali, bool &game_berjalan, bool &menang, int &nyawa, char input){
    int dx = 0;
    int dy = 0;

    switch (input){
        case 'w':
        case 'W':
            dy = -1; //naik 1 langkah
            break;
        case 's':
        case 'S':
            dy = 1; //turun 1 langkah
            break;
        case 'a':
        case 'A':
            dx = -2; //kiri 2 langkah
            break;
        case 'd':
        case 'D':
            dx = 2;
            break; //kanan 2 langkah
    }

    int Xsetelah = player.x + dx;
    int Ysetelah = player.y + dy;

    if (map[Ysetelah][Xsetelah] == '+' || map[Ysetelah][Xsetelah] == '-' || map[Ysetelah][Xsetelah] == '|'){
        return;
    }

    char pemain = map[Ysetelah][Xsetelah];
    if (pemain == 'K'){
        player.bawakunci = true;
        map[Ysetelah][Xsetelah] = ' '; //status bawa kunci
    }
    if (pemain == 'E'){
        if(player.bawakunci){
            menang = true;
            game_berjalan = false; //bisa memang kalau udah di koordinat dan status bawa kuncinya true.
            //kalau ga bawa kunci. gabisa exit
        }
    }

// Cek tabrakan sama all anomali
    for (int i = 0; i < jumlahAnomali; i++) {
        if (Xsetelah == anomaliList[i].x && Ysetelah == anomaliList[i].y) {
            nyawa--;
            if (nyawa > 0){
                player.x = 2;
                player.y = 1;
                return;
            } else {
                game_berjalan = false;
                menang = false;
                return;
            }
        }
    }

    player.x = Xsetelah;
    player.y = Ysetelah;
}

// Tampilin HUD (display judul, nyawa, dan status kunci di atas map)
void tampilkanHUD(int nyawa, bool bawakunci, entitas player) {
    cout << "+--------------------------------------------+\n";
    cout << "|    ESCAPE FROM CALCULUS    |    NYAWA : ";
    cout << nyawa;
    cout << "  |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|         KUNCI : " 
         << (bawakunci ? "V" : "X")
         << "          |  POS : (" << player.x << ", " << player.y << ")";
    
    int panjang = 12 + (bawakunci ? 1 : 1) + 12;
    for (int i = panjang; i < 26; i++) {
        cout << " ";
    }
    
    cout << "|\n";
    cout << "+--------------------------------------------+\n\n";
    }

void resetPlayer(entitas &player, int &nyawa) {
    player.x = 2;
    player.y = 1;
    player.bawakunci = false;
    nyawa = 2; //playernya ke reset dan balik ke awal
}

int main(){
char input;
    entitas player;
    player.x = 2;
    player.y = 1;
    player.simbol = 'O';
    player.bawakunci = false;
    int nyawa = 2;

    menu:{
        system("cls"); //clear screen, bersihin layar
        bool menang=false;
        char milih,n,milihdeui;
        int s;
        cout << "===============ESCAPE FROM CALCULUS=============="<<endl;
        cout << "[1]Main yuk!"<<endl;
        cout << "[2]Atau mau keluar?"<<endl;
        milih = getch();
        system("cls");
        switch(milih){
            case '1':{
                cout << "===============ESCAPE FROM CALCULUS=============="<<endl;
                cout << "Silakan untuk memilih level"<<endl;
                cout << "[1]Level 1"<<endl;
                cout << "[2]Level 2"<<endl;
                cout << "[3]Level 3"<<endl;
                milihdeui=getch();
                switch(milihdeui){
                    case '1':{
                        hiji:
                        system("cls");
                        {
                            tampilkanInstruksi();
                            system("cls");
                            anomali anomaliList[0];
                            int jumlahAnomali = 0;
                            
                            resetMap();
                            resetPlayer(player, nyawa);
                            bacaFilemap("level1.txt", player);

                            bool game_berjalan = true;
                            bool menang = false;

                            while (game_berjalan) {
                                cekCollisionAnomali(player, anomaliList, jumlahAnomali, nyawa, game_berjalan, menang);
                                
                                tampilkanHUD(nyawa, player.bawakunci, player);
                                tampilkanmap(player, anomaliList, jumlahAnomali);
                                
                                input = _getch(); //biar inputnya ga muncul di layar
                                gerakanplayer(player, anomaliList, jumlahAnomali, game_berjalan, menang, nyawa, input);

                                system("cls");    
                            }

                            if (menang) win();
                            else lose();

                            char pilih = afterLevelMenu(1, menang);

                            // proses pilihan disini, karena di sini ada labelnya
                            if (menang) {
                                if (pilih == '1') goto dua;      // lanjut level 2
                                if (pilih == '2') goto hiji;     // ulangi level 1
                                if (pilih == '3') goto menu;     // balik ke menu
                                system("cls");
                            } else {
                                if (pilih == '1') goto hiji;     // ulangi level
                                if (pilih == '2') goto menu;     // balik ke menu
                                system("cls");
                            }
                            break;                        
                        }
                    }
                    case '2':{dua:{
                        system("cls");
                        {
                            tampilkanInstruksi();
                            system("cls");
                            //rute paten untuk n anomali
                            position routeA1[] = {
                                {1,42}, {1,40}, {1,38}, {1,36}, {1,34}, {1,32}, {1,30}, {1,28},
                                {1,26}, {1,24}, {1,22}, {1,20}, {1,18}, {1,16}, {1,14}, {1,12},
                                {1,10}, {2,10}, {3,10}, {4,10}, {5,10}, {5,8}, {5,6}, {6,6},
                                {7,6}, {7,8}, {7,10}, {7,12}, {7,14}, {7,16}, {7,18}, {7,20},
                                {7,22}
                            };

                            position routeA2[] = {
                                {11,2}, {11,4}, {11,6}, {11,8}, {11,10}, {11,12}, {11,14}, {11,16},
                                {11,18}, {12,18}, {13,18}, {13,20}, {13,22}, {12,22}, {11,22}, {11,24},
                                {11,26}, {12,26}, {13,26}, {13,28}, {13,30}, {13,32}, {13,34}
                            };


                            //array anomali
                            anomali anomaliList[2];
                            int jumlahAnomali = 2;

                            //inisialisasi anomali dengan rute dan kecepatannya
                            initiateAnomali(anomaliList[0], routeA1, sizeof(routeA1)/sizeof(routeA1[0]), 50, u8"∫");
                            initiateAnomali(anomaliList[1], routeA2, sizeof(routeA2)/sizeof(routeA2[0]), 50, u8"∫");

                            resetMap();
                            resetPlayer(player, nyawa);
                            bacaFilemap("level2.txt", player);

                            bool game_berjalan = true;
                            bool menang = false;

                            while (game_berjalan) {
                                //update posisi anomali secara realtime
                                for (int i = 0; i < jumlahAnomali; i++) {
                                    moveAnomali(anomaliList[i], currentTime());
                                }

                                // kolisi si anomali stlh bergerak
                                cekCollisionAnomali(player, anomaliList, jumlahAnomali, nyawa, game_berjalan, menang);

                                tampilkanHUD(nyawa, player.bawakunci, player);
                                tampilkanmap(player, anomaliList, jumlahAnomali);

                                input = _getch();
                                gerakanplayer(player, anomaliList, jumlahAnomali, game_berjalan, menang, nyawa, input);

                                system("cls");
                                }


                                if (menang) win();
                                else lose();

                                char pilih = afterLevelMenu(2, menang);

                                //milih lagi
                                if (menang) {
                                    if (pilih == '1') goto tilu;   // lanjut ke level 3
                                    if (pilih == '2') goto dua;    // ulangi level 2
                                    if (pilih == '3') goto menu;   // balik ke ke menu
                                    system("cls");
                                } else {
                                    if (pilih == '1') goto dua;    // ulangi level 2
                                    if (pilih == '2') goto menu;   // balik ke menu
                                    system("cls");
                                }
                
                            }
                        break;
                        }
                    }
                        case '3':{tilu:
                        system("cls");
                            {//rute paten anomali
                        tampilkanInstruksi();
                        system("cls");
                        position routeA1[] = {
                            {5,54}, {5,52}, {5,50}, {5,48}, {5,46}, {5,44}, {5,42}, {5,40},
                            {5,38}, {5,36}, {5,34}, {6,34}, {7,34}, {7,32}, {7,30}, {8,30},
                            {9,30}, {9,28}, {9,26}, {9,24}, {9,22}, {10,22}, {11,22}, {12,22},
                            {13,22}, {13,20}, {13,18}, {14,18}, {15,18}, {15,16}, {15,14}, {14,14},
                            {13,14}, {13,12}, {13,10}, {12,10}, {11,10}, {11,12}, {11,14}, {11,16},
                            {11,18}, {10,18}, {9,18}, {8,18}, {7,18}, {7,16}, {7,14}, {8,14},
                            {9,14}, {9,12}, {9,10}, {8,10}, {7,10}, {7,8}, {7,6}
                        };
                        position routeA2[] = {
                            {27,62}, {27,60}, {27,58}, {27,56}, {27,54}, {27,52}, {27,50}, {26,50},
                            {25,50}, {24,50}, {23,50}, {23,52}, {23,54}, {24,54}, {25,54}, {25,56},
                            {25,58}, {25,60}, {25,62}, {25,64}, {25,66}, {24,66}, {23,66}, {23,64},
                            {23,62}, {23,60}, {23,58}, {22,58}, {21,58}, {21,56}, {21,54}, {21,52},
                            {21,50}, {21,48}, {21,46}, {22,46}, {23,46}, {23,44}, {23,42}, {23,40},
                            {23,38}, {23,36}, {23,34}
                        };

                        position routeA3[] = {
                            {25,30}, {26,30}, {27,30}, {28,30}, {29,30}, {29,28}, {29,26}, {29,24},
                            {29,22}, {29,20}, {29,18}, {28,18}, {27,18}, {27,20}, {27,22}, {27,24},
                            {27,26}, {26,26}, {25,26}, {24,26}, {23,26}, {23,24}, {23,22}, {23,20},
                            {23,18}
                        };

                        //array anomali
                        anomali anomaliList[3];
                        int jumlahAnomali = 3;

                        //inisialisasi setiap anomali dengan route dan kecepatan
                        initiateAnomali(anomaliList[0], routeA1, sizeof(routeA1)/sizeof(routeA1[0]), 50, u8"∫");
                        initiateAnomali(anomaliList[1], routeA2, sizeof(routeA2)/sizeof(routeA2[0]), 50, u8"∫");
                        initiateAnomali(anomaliList[2], routeA3, sizeof(routeA3)/sizeof(routeA3[0]), 50, u8"∫");

                        resetMap();
                        resetPlayer(player, nyawa);
                        bacaFilemap("level3.txt", player);
                        bool game_berjalan = true;
                        bool menang = false;

                        while (game_berjalan) {
                            //updtae posisi semua anomali secara real time
                            for (int i = 0; i < jumlahAnomali; i++) {
                                moveAnomali(anomaliList[i], currentTime());

                            }

                            // Collision semua anomali stlh mereka bergerak
                            cekCollisionAnomali(player, anomaliList, jumlahAnomali, nyawa, game_berjalan, menang);

                            tampilkanHUD(nyawa, player.bawakunci, player);
                            tampilkanmap(player, anomaliList, jumlahAnomali);

                            input = _getch();
                            gerakanplayer(player, anomaliList, jumlahAnomali, game_berjalan, menang, nyawa, input);

                            system("cls");

                            }

                            if (menang) win();
                            else lose();

                            char pilih = afterLevelMenu(3, menang);

                            //milih lagi, tapi gabisa next karena udah akhir
                            if (menang) {
                                if (pilih == '1') goto tilu;    // ulangi level 3
                                if (pilih == '2') goto menu;   // kembali menu
                                system("cls");
                            } else {
                                if (pilih == '1') goto tilu;    // ulangi level 3
                                if (pilih == '2') goto menu;   // kembali menu
                                system("cls");
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}
