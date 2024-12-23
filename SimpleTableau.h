#include <vector>
#include "PauliString.h"
using namespace std;

namespace pyQEC {

class SimpleTableau {
    public:
        int m_N;
        int current_rank;
        vector<vector<bool>> matrix;
        SimpleTableau(int N);

        static SimpleTableau initialize_mixed_on_r_qubits(int n, int r);
        static vector<vector<int>> row_echelon(vector<vector<bool>> mat);
        static vector<vector<int>> row_echelon_vector(vector<vector<bool>> mat);

        SimpleTableau* copy();
        void rowReduce();
        void swap_rows(int i, int j);
        void swap_cols(int i, int j);
        void xor_rows(int i, int j);
        void xor_cols(int i, int j);
        void upper_triangular();
        int rank();
        void measure(PauliString p);
        int entanglement_entropy(int cut);
        void cnot(int i, int j);
        void hadamard(int i);
        void phase(int i);

};

}

