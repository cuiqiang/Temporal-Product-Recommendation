#include "matrix_factorization.h"

struct purchase{
    int t;
    int u;
    int i;
    int Xuit;
    int ruit;
};

class MarginalNetUtility : public MatrixFactorization {
protected:
    std::vector<purchase> H;
    double* gamma;
public:
    void initGamma();
    void train(char*);
    void load_purchase(char*);
    void load_model(char*);
    void SVD_util(int step_count=10000, double lambda1=0.05, double lambda2=0.01, double beta1=0.015, double beta2 = 0.035, double gamma0 = 1, double threshold=0.001);
    void model_output(char*);
    void predict(char*,char*);
};
