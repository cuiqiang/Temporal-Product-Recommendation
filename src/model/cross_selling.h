#include "matrix_factorization.h"
#include "../common/common.h"
#include <map>

class CrossSelling : public MatrixFactorization {
protected:
    std::vector<cross_purchase> H;
    std::map<int, std::map<int, double> > trans;
    double* gamma;
public:
    void initGamma();
    void train(char*);
    void load_purchase(char*);
    void load_model(char*);
    void SVD_cross(int step_count=500, double lambda1=0.05, double lambda2=0.01, double beta1=0.015, double beta2 = 0.035, double gamma0 = 1, double threshold=0.001);
    void model_output(char*);
    void predict(char*,char*);
};
