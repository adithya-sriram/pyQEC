#include "HoneycombLayout.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <algorithm>
#include <stdlib.h>
#include <map>

using namespace std;

namespace pyQEC {

    tuple<int,int> pbc(int d, int x, int y) {
            if (x == d && y < d)
                return make_tuple(0,y);
            else if (x < d && y == d)
                return make_tuple(x,0);
            else 
                return make_tuple(x,y);
    }


    HoneycombLayout::HoneycombLayout(int d) {
        L = d;
        complex<double> a1 (1, 0);
        complex<double> a2 (0.5, sqrt(3) / 2);
        complex<double> tau (0.5, 1 / (sqrt(3) * 2));

        int qubit_index = 0;
    
        for (int n = 0; n < d; n++) {
            for (int m = 0; m < d; m++) {
                qubits.insert({make_tuple(m,n,0),  qubit_index});
                qubits.insert({make_tuple(m,n,1),  qubit_index + 1});
                qubit_index += 2;
                points.push_back((m * 1.0) * a1 + (n * 1.0) * a2);
                points.push_back((m * 1.0) * a1 + (n * 1.0) * a2 + tau);
                int category = (m % 3 - n % 3) % 3;
                tuple<double, double> hex_coord = make_tuple(m * 1.0 * a1.real() + n * 1.0 * a2.real() + 2.0 * tau.real(), 
                                                        m * 1.0 * a1.imag() + n * 1.0 * a2.imag() + 2.0 * tau.imag());
                hex_centers.insert({make_tuple(get<0>(hex_coord), get<1>(hex_coord)), category});

            }
        }

        for (int n = 0; n < d; n++) {
            for (int m = 0; m < d; m++) {
                tuple<int, int> p1 = make_tuple( qubits.at( make_tuple(m,n,0) ) , qubits.at( make_tuple(m,n,1) ) );
                tuple<int, int> p2 = make_tuple( qubits.at( make_tuple(m,n,0) ) , qubits.at( make_tuple(get<0>(pbc(d,m,n+1)), get<1>(pbc(d,m,n+1)), 0) ));
                tuple<int, int> p3 = make_tuple( qubits.at( make_tuple(m,n,0) ) , qubits.at( make_tuple(get<0>(pbc(d,m+1,n)), get<1>(pbc(d,m+1,n)), 0) ));
                complex<double> hex_coord = (m * 1.0) * a1 + (n * 1.0) * a2 + 2.0 * tau;
                int c1 = hex_centers.at( make_tuple(hex_coord.real(), hex_coord.imag()) );
                int c2 = (c1 + 1) % 3;
                int c3 = (c1 + 2) % 3;   
                bonds.insert( {p1, make_tuple(c1, 'X')} );
                bonds.insert( {p2, make_tuple(c2, 'Y')} );
                bonds.insert( {p3, make_tuple(c3, 'Z')} );

             
            }
        }

    }


    vector<tuple<double, double, int>> HoneycombLayout::get_qubit_coords() {
        complex<double> a1 (1, 0);
        complex<double> a2 (0.5, sqrt(3) / 2);
        complex<double> tau (0.5, 1 / (sqrt(3) * 2));

        vector<tuple<double, double, int>> qubit_points;

        map<tuple<int, int, int>, int>::iterator it;

        for (it = this->qubits.begin(); it != this->qubits.end(); ++it) {
            tuple<int, int, int> c = it->first;
            double m = get<0>(c);
            double n = get<1>(c);
            double t = get<2>(c);

            double x = m * a1.real() + n * a2.real() + t * tau.real();
            double y = m * a1.imag() + n * a2.imag() + t * tau.imag();

            tuple<double, double, int> cpoint = make_tuple(x, y, it->second);
            qubit_points.push_back(cpoint);

        }

        return qubit_points;

    }

    vector<tuple<int, int, int, char>> HoneycombLayout::get_bonds() {
        vector<tuple<int, int, int, char>> b;

        map<tuple<int, int>, tuple<int, char>>::iterator it;

        for (it = this->bonds.begin(); it != this->bonds.end(); ++it) {
            tuple<int, int> p = it->first;
            tuple<int, char> c = it->second;
            b.push_back( make_tuple(get<0>(p), get<1>(p), get<0>(c), get<1>(c)) );
        }

        return b;

    }

}
