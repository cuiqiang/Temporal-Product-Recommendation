#include "marginal_net_utility.h"

class PurchaseInterval : public MarginalNetUtility{
    int*** d;
    int** T;
    int now;
    double* mu;
public:
    void initT();
    void initMu();
    double maxPI(int, int, int);
    void load_purchase_interval();
    void SVD_pi(int step_count=10000, double lambda1=0.05, double lambda2=0.01, double lambda3=0.01, double beta1=0.015, double beta2 = 0.035, double beta3 = 0.035, double gamma0 = 1, double mu0 = 1, double threshold=0.001);
    void svdpi_evaluation();
};
