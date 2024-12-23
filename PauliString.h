#include <vector>
#include <string>
#include <map>
#include <complex>  

using namespace std;

namespace pyQEC {


class PauliString {
    public:
        static const vector<string> pauli_group;
        static const map<string, string> pauli_prods;
        string op;

        PauliString(string paulioperator);
        int len();
        int eq(PauliString p);
        vector<string> breakString();
        static string determinePhase(string phase1, string phase2);
        string cu2str(complex<int> c);
        PauliString add(PauliString p);
        PauliString mult(PauliString p);
        int comm(PauliString p);
};

}