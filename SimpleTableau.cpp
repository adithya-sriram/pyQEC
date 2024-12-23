#include "SimpleTableau.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;


namespace pyQEC {


    SimpleTableau::SimpleTableau(int N){
        m_N = N;
        current_rank = N;
        for (int i = 0; i < N; i++) {
            vector<bool> temp(2 * N + 1, false);
            matrix.push_back(temp);
            matrix[i][i+N] = true;
        }
        matrix[N - 1][2 * N] = false;
    }

    void SimpleTableau::rowReduce() {
        if (this->rank() == 0) {
            vector<vector<bool>> reduced_state;
            reduced_state.push_back(this->matrix[0]);
            this->matrix = reduced_state;

        }
        else {
            vector<int> id_elements;
            for (int i = 0; i < this->matrix.size(); i++) {
                int non_id = 0;
                for (int j = 0; j < this->matrix[0].size(); j++) {
                    if (this->matrix[i][j] == 1) {
                        non_id = 1;
                        break;
                    }
                }
                if (non_id == 0) {
                    id_elements.push_back(i);
                }
            }

            for (int i = 0; i < id_elements.size(); i++) {
                this->matrix.erase(this->matrix.begin() + (id_elements[i] - i));
            }
        }
    }

    SimpleTableau SimpleTableau::initialize_mixed_on_r_qubits(int n, int r){
        SimpleTableau *s = new SimpleTableau(n);
        for (int i = r; i < n; i++) {
            for (int j = 0; j < (2 * n + 1); j++) {
                s->matrix[i][j] = false;
            }
        }
        s->current_rank = 0;
        s->rowReduce();
        return *s;
    }
    
    // This method might be broken
    SimpleTableau* SimpleTableau::copy(){
        SimpleTableau *s = new SimpleTableau(this->m_N);
        for (int i = 0; i < this->m_N; i++) {
            for (int j = 0; j < 2 * this->m_N + 1; j++) {
                s->matrix[i][j] = this->matrix[i][j];
            }
        }
        return s;
    }

    vector<vector<int>> SimpleTableau::row_echelon(vector<vector<bool>> mat){
        int N = mat.size();
        int M = mat[0].size();

        vector<vector<int>> rref_mat (N, vector<int>(M, 0));

        
        // we search for non-zero element in the first column while populating the 2D array
        int first_non_zero_element_in_first_column = -1;

        for (int i = 0; i < N; i++){

            if (mat[i][0] == true && first_non_zero_element_in_first_column == -1) {
                first_non_zero_element_in_first_column = i;
            }

            for (int j = 0; j < M; j++) {
                if (mat[i][j] == 1)
                    rref_mat[i][j] = 1;
                else rref_mat[i][j] = 0;
            }
        }  


        // if all elements in the first column is zero,
        // we perform REF on matrix from second column
        if (first_non_zero_element_in_first_column == -1) {
            if (N - 1 == 0 || M - 1 == 0) {
                return rref_mat;
            }
            else {
                vector<vector<bool>> v(N,vector<bool> (M-1, false));
                for (int i = 0; i < N; i++) {
                    for (int j = 1; j < M; j++) {
                        v[i][j-1] = mat[i][j];
                    }
                }
                vector<vector<int>> B = row_echelon(v);
                for (int i = 0; i < N; i++) {
                    for (int j = 1; j < M; j++) {
                        rref_mat[i][j] = B[i][j-1];
                    }
                }
                return rref_mat;
            }
        }
        
        // if non-zero element happens not in the first row,
        // we switch rows
        if (first_non_zero_element_in_first_column != 0) {
            vector<int> temp = rref_mat[0];
            for (int i = 0; i < M; i++){
                rref_mat[0][i] = rref_mat[first_non_zero_element_in_first_column][i];
                rref_mat[first_non_zero_element_in_first_column][i] = temp[i];
            }
        }


        // Add first row to all other rows mod 2 that have a 1 in the 1st column
        for (int i = 1; i < N; i++){
            if (rref_mat[i][0] == 1) {
                for (int j = 0; j < M; j++) {
                    rref_mat[i][j] = (rref_mat[i][j] + rref_mat[0][j]) % 2;
                }
            }
        }

        // we perform REF on matrix from second row, from second column
        if (N - 1 == 0 || M - 1 == 0) {
            return rref_mat;
        }
        else {
            vector<vector<bool>> v(N-1,vector<bool> (M-1, false));
            for (int i = 1; i < N; i++) {
                for (int j = 1; j < M; j++) {
                    v[i-1][j-1] = rref_mat[i][j];
                }
            }
            vector<vector<int>> B = row_echelon(v);
            for (int i = 1; i < N; i++) {
                for (int j = 1; j < M; j++) {
                    rref_mat[i][j] = B[i-1][j-1];
                }
            }
            return rref_mat;

        }
        
        

    }

    vector<vector<int>> SimpleTableau::row_echelon_vector(vector<vector<bool>> mat) {
        return SimpleTableau::row_echelon(mat);;
    }

    PauliString rowToPauliString(vector<bool> row) {
            string s = "";
            int n = (row.size() - 1) / 2;
            if (row[2 * n] == true)
                s = s + "-";
            for (int i = 0; i < n; i++) {
                if (row[i] == false && row[i+n] == false)
                    s += "I";
                else if (row[i] == false && row[i+n] == true)
                    s += "Z";
                else if (row[i] == true && row[i+n] == false)
                    s += "X";
                else if (row[i] == true && row[i+n] == true)
                    s += "Y";
            }
            return PauliString(s);
    }

    vector<bool> pauliStringtoRow(PauliString p) {
        vector<string> s1 = p.breakString();
        int n = s1.size() - 1;
        vector<bool> row (2 * n + 1, false);

        if (s1[0].compare("-") == 0)
            row[2 * n] = true;
        for (int i = 1; i < s1.size(); i++) {
            if(s1[i].compare("I") == 0) {
                row[i - 1] = false;
                row[n + i - 1] = false;
            }
            else if (s1[i].compare("X") == 0) {
                row[i - 1] = true;
                row[n + i - 1] = false;
            }
            else if (s1[i].compare("Y") == 0) {
                row[i - 1] = true;
                row[n + i - 1] = true;
            }
            else if (s1[i].compare("Z") == 0) {
                row[i - 1] = false;
                row[n + i - 1] = true;
            }
            else {}
        }

        return row;
    }
    
    void SimpleTableau::swap_rows(int i, int j) {
        int ncols = this->matrix[0].size();
        for (int k=0; k<ncols; k++) {
            bool tmp = this->matrix[i][k];
            this->matrix[i][k] = this->matrix[j][k];
            this->matrix[j][k] = tmp;
        }
    }

    void SimpleTableau::xor_rows(int i, int j) {
        // adds second entry to first in place
        int ncols = this->matrix[0].size();
        for (int k=0; k<ncols; k++)
            this->matrix[i][k] = this->matrix[i][k] xor this->matrix[j][k];
    }

    void SimpleTableau::upper_triangular() {
        int nrows = this->matrix.size();
        int ncols = this->matrix[0].size();
        int row0 = 0;
        for (int col=0; col<ncols; col++) {
            int row=row0;
            while (row<nrows and not this->matrix[row][col])
                row++;
            if (row<nrows) { // found something
                this->swap_rows(row0,row);
                for (row=row0+1; row<nrows; row++)
                    if (this->matrix[row][col])
                        this->xor_rows(row,row0); // replace row with row+row0
                row0++;
            }
        }
    }

    int SimpleTableau::rank() {
        int N = this->matrix.size();
        int M = this->matrix[0].size();
        SimpleTableau *copysystem = new SimpleTableau(N);
        copysystem->matrix = this->matrix;

        for (int i = 0; i < N; i++)
            copysystem->matrix[i][M-1] = false;

        copysystem->upper_triangular();

        int col = 0;
        for (int row=0; row<N; row++) {
            while (col<M and not copysystem->matrix[row][col])
                col++;
            if (col==M)
                return row;
        }
        delete copysystem;
        return N;
    }

    int symplectic_inner_product(vector<bool> p1, vector<bool> p2, int N) {
        int sum = 0;
        for (int i = 0; i < N; i++) {
            sum = (sum + p1[i] * p2[i + N]);
        }
        for (int i = 0; i < N; i++) {
            sum = (sum + p1[i + N] * p2[i]);
        }
        return sum % 2;

    }

    void SimpleTableau::measure(PauliString p) {
        vector<int> anticommuting_rows;
        int N = this->matrix.size();
        int M = this->matrix[0].size();
        int r1 = this->current_rank;
        vector<bool> p_row = pauliStringtoRow(p);

        // Find anti-commuting rows
        for(int i = 0; i < N; i++){
            if (symplectic_inner_product(p_row, this->matrix[i], this->m_N) == 1) {
                anticommuting_rows.push_back(i);
            }
                
        }
        // for (int i = 0; i < anticommuting_rows.size(); i++)
        //     cout << anticommuting_rows[i];
        // cout << endl;


        // All rows commute with p
        if (anticommuting_rows.size() == 0) {
            // cout << "All rows commute with p" << endl;
            // Do nothing if it is a pure state
            if (r1 == this->m_N) { }

            else {
                this->matrix.push_back(p_row);
                int r2 = this->rank();
                // p is a stabilizer
                if (r1 == r2) { 
                    this->matrix.pop_back();
                }
                // p commutes with all the stabilizers but is not a stabilizer
                else {
                    // cout << "p commutes with all the stabilizers but is not a stabilizer" << endl;
                    this->current_rank = r2;
                    if (rand() % 2 == 0)
                        this->matrix[this->matrix.size() - 1][this->matrix[0].size() - 1] = true;
                    else
                        this->matrix[this->matrix.size() - 1][this->matrix[0].size() - 1] = false;
                }
            }
        }
        // If one row anti-commutes with p, replace that row with p
        else if (anticommuting_rows.size() == 1) {
            // cout << "One row anti-commutes with p" << endl;
            this->matrix[anticommuting_rows[0]] = p_row;
        }
        // If more than one row anti-commutes with p, form products that commute with g, say {r_1r_2, r_1r_3, … r_1r_k}, then drop r_1 and replace it with p
        else {
            // cout << "More than one row anti-commmutes with p" << endl;
            PauliString op1 = rowToPauliString(this->matrix[anticommuting_rows[0]]);
            this->matrix[anticommuting_rows[0]] = p_row;
            for (int i = 1; i < anticommuting_rows.size(); i++) {
                PauliString op2 = rowToPauliString(this->matrix[anticommuting_rows[i]]);
                PauliString new_op = op1.mult(op2);
                this->matrix[anticommuting_rows[i]] = pauliStringtoRow(new_op);
            }
        }  
        this->rowReduce();


    }


    int SimpleTableau::entanglement_entropy(int cut) {
        if (this->m_N != this->matrix.size())
            throw std::invalid_argument( "not a pure state" );

        int N = this->m_N;
        SimpleTableau *subsystem = new SimpleTableau(N);
        for (int i = 0; i < N; i++) {
            subsystem->matrix[i].resize(2 * cut);
            for (int j = 0; j < cut; j++) {
                subsystem->matrix[i][j] = this->matrix[i][j];
                subsystem->matrix[i][j + cut] = this->matrix[i][j + N];
            }
        }
        
        subsystem->upper_triangular();


        int rank = 0;
        for(int i = 0; i < N; i++) {
            for (int j = 0; j < subsystem->matrix[0].size(); j++) {
                if (subsystem->matrix[i][j]) {
                    rank = rank + 1;
                    break;
                }
            }
        }


        return rank - cut;

    }

    void SimpleTableau::cnot(int a, int b) {
        //control is a, target is b
        int nrows = this->matrix.size();
        int ncols = this->matrix[0].size();
        int N = this->m_N;

        for (int k = 0; k < nrows; k++) {
            bool val = (this->matrix[k][a] * this->matrix[k][N+b]) * (this->matrix[k][b] xor this->matrix[k][a+N] xor true);
            this->matrix[k][b] = this->matrix[k][b] xor this->matrix[k][a];
            this->matrix[k][a+N] = this->matrix[k][a+N] xor this->matrix[k][b+N];
        }
    }

    void SimpleTableau::hadamard(int a) {
        int M = this->matrix[0].size();
        for (int i = 0; i < this->matrix.size(); i++) {
            bool val = this->matrix[i][M - 1] xor (this->matrix[i][a] * this->matrix[i][a + this->m_N]);
            // this->matrix[i][2 * this->m_N] = val;
            // bool temp = this->matrix[i][a];
            // this->matrix[i][a] = this->matrix[i][a + this->m_N];
            // this->matrix[i][a + this->m_N] = temp;
        }
    }

    void SimpleTableau::phase(int a) {
        for (int i = 0; i < this->matrix.size(); i++) {
            bool val = this->matrix[i][2 * this->m_N] xor (this->matrix[i][a] * this->matrix[i][a + this->m_N]);
            this->matrix[i][2 * this->m_N] = val;
            this->matrix[i][a + this->m_N] = this->matrix[i][a + this->m_N] xor  this->matrix[i][a];
        }
    }
}
