#include "PauliString.h"
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <complex>  
#include <vector>

using namespace std;

namespace pyQEC {

    const vector<string> PauliString::pauli_group = {"I", "X", "Y", "Z", "iI", "iX", "iY", "iZ"};
    const map<string, string> PauliString::pauli_prods = {{"IX", "X"}, {"IY", "Y"}, {"IZ", "Z"}, {"XI", "X"}, {"YI", "Y"}, {"ZI", 
    "Z"}, {"XX", "I"}, {"XY", "iZ"}, {"XZ", "-iY"}, {"YX", "-iZ"}, {"YY", "I"}, {"YZ", "iX"},{"ZX", "iY"}, {"ZY", "-iX"}, {"ZZ", "I"}, {"II", "I"}};
    
    PauliString::PauliString(string paulioperator) {
        op = paulioperator;
    }

    int is_pauli(string s) {
        for (int i = 0; i < PauliString::pauli_group.size(); i++) {
            if (PauliString::pauli_group[i].compare(s) == 0) {
                return 0;
            }
        }
        return 1;
    }
    
    int PauliString::len() {
        return this->op.length();
    }

    int PauliString::eq(PauliString p) {
        int i = p.op.compare(this->op);
        if (i == 0)
            return 0;
        else
            return 1;
    }

    vector<string> PauliString::breakString() {
        vector<string> arr;
        for (int i = 0; i < this->op.size(); i++) {
            if (is_pauli(this->op.substr(i,1)) == 0 && arr.size() == 0) {
                arr.push_back(this->op.substr(0,i));
                arr.push_back(this->op.substr(i,1));
            }
            else if (is_pauli(this->op.substr(i,1)) == 0) {
                arr.push_back(this->op.substr(i,1));
            }
        }

        return arr;
    }

    string PauliString::determinePhase(string phase1, string phase2) {
        if (phase1 == "") {
            return phase2;
        }
        else if (phase2 == "") {
            return phase1;
        }
        else if (phase1 == "-") {
            if (phase2 == "-") {
                return "";
            }
            else if (phase2 == "i") {
                return "-i";
            }
            else if (phase2 == "-i") {
                return "i";
            }
        }
        else if (phase2 == "-") {
            if (phase1 == "i") {
                return "-i";
            }
            else if (phase1 == "-i") {
                return "i";
            }
        }
        else if (phase1 == "i") {
            if (phase2 == "i") {
                return "-";
            }
            else if (phase2 == "-i") {
                return "";
            }
        }
        else if (phase2 == "i") {
            if (phase1 == "-i") {
                return "";
            }
        }
        else if (phase1 == "-i") {
            if (phase2 == "-i") {
                return "-";
            }
        }
        return "";
    }

    string PauliString::cu2str(complex<int> c) {
        if (c.real() == 1 && c.imag() == 0) {
            return "";
        }
        else if (c.real() == -1 && c.imag() == 0) {
            return "-";
        }
        else if (c.real() == 0 && c.imag() == 1) {
            return "i";
        }
        else {
            return "-i";
        }
    }

    PauliString PauliString::add(PauliString p) {
        string phase1, phase2, op1, op2;

        for (int i = 0; i < this->op.size(); i++) {
            if (is_pauli(this->op.substr(i,1)) == 0) {
                phase1 = this->op.substr(0,i);
                op1 = this->op.substr(i);
                break;
            }
        }

        for (int i = 0; i < p.op.size(); i++) {
            if (is_pauli(p.op.substr(i,1)) == 0) {
                phase2 = p.op.substr(0,i);
                op2 = p.op.substr(i);
                break;
            }
        }

        return PauliString(PauliString::determinePhase(phase1, phase2) + op1 + op2);
    }

    PauliString PauliString::mult(PauliString p) {
        vector<string> s1 = this->breakString();
        vector<string> s2 = p.breakString();
        vector<string> prodlist;

        if (s1.size() >= s2.size())
            prodlist = s1;
        else prodlist = s2;
        


        prodlist[0] = PauliString::determinePhase(s1[0], s2[0]);

        for (int i = 1; i < min(s1.size(), s2.size()); i++) {
            string temp = s1[i] + s2[i];
            prodlist[i] = PauliString::pauli_prods.at(temp);
        }

        PauliString *prod = new PauliString(prodlist[1]);
        prod->op = PauliString::determinePhase(prodlist[0], prod->breakString()[0]) + prod->breakString()[prod->breakString().size() - 1];

        for (int i = 2; i < prodlist.size(); i ++) {
            *prod = prod->add(prodlist[i]);
        }
        return *prod;
    }

    int PauliString::comm(PauliString p) {
        vector<string> s1 = this->breakString();
        vector<string> s2 = p.breakString();

        int points = 0;

        for (int i = 1; i < min(s1.size(), s2.size()); i++) {
            if ((s1[i].compare("I") != 0 && s2[i].compare("I") != 0) && s1[i].compare(s2[i]) != 0) {
                points +=1;
            }
        }
        if (points % 2 == 0)
            return 0;
        else return 1;
    }

}
