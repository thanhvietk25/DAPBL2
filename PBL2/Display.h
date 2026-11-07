#ifndef DISPLAY_UTIL_H
#define DISPLAY_UTIL_H

#include <iostream>
#include <string>
using namespace std;

const int max_ktu = 50;

inline void printline(const string &kytu, const int solan = max_ktu)
{
    for (int i = 0; i < solan; i++)
        cout << kytu;
    cout << "\n";
}

inline void printgiua(const string &Tbao, const string &kytu, const int solan = max_ktu)
{
    int conlai = solan - (int)Tbao.size();
    if (conlai < 0)
        conlai = 0;

    int trai = conlai / 2;
    int phai = conlai - trai;

    cout << "\n";
    for (int i = 0; i < trai; i++)
        cout << kytu;

    cout << Tbao;

    for (int i = 0; i < phai; i++)
        cout << kytu;

    cout << "\n";
}

inline void printtb(const string &Tbao, const string &kytu, const int solan = max_ktu)
{
    int trong = solan - 2 - (int)Tbao.size();
    if (trong < 0)
        trong = 0;

    int trai = trong / 2;
    int phai = trong - trai;

    cout << "\n";
    printline(kytu, solan);

    cout << kytu;
    for (int i = 0; i < trai; i++)
        cout << " ";

    cout << Tbao;

    for (int i = 0; i < phai; i++)
        cout << " ";

    cout << kytu << "\n";
    printline(kytu, solan);
}

#endif