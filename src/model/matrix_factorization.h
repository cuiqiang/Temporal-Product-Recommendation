#include <iostream>
#include <vector>
#include <map>
#include <string>

struct element{
    int i;
    int j;
    double rate;
};


class MatrixFactorization{
protected:
    double** Pu_z;
    double** Qv_z;
    std::vector<element> R;
    int row;
    int column;
    int K;
public:
    void init();
    void load_matrix(char*);
    void load_model(char*);
    void matrix_factorization(int step_count=5000, double alpha=0.0002, double beta=0.02, double threshold=0.01);
    double dot(int, int);//P[i][:] dot Q[:][j] 
    void model_output(char*);
    void train(char*);
    void predict(char*,char*);
    void debug_model_para();
};
