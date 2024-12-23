#include <vector>
#include <complex>
#include <map>


using namespace std;

namespace pyQEC {

class HoneycombLayout {
    public:
        int L;
        vector<complex<double>> points;
        map<tuple<int, int, int>, int> qubits;
        map<tuple<int, int>, int> hex_centers;
        map<tuple<int, int>, tuple<int, char>> bonds;
        HoneycombLayout(int d);

        vector<tuple<double, double, int>> get_qubit_coords();
        vector<tuple<int, int, int, char>> get_bonds();

};

}