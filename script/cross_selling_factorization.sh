#/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

#purchase_history=${datapath}/cross_selling/purchase_history

#purchase_history=${datapath}/cross_selling/purchase-40-55-1year-top7.5w-sort-id-uniq
purchase_history=${datapath}/cross_selling_factorization/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/test/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/marginal_net_utility/purchase-40-55-1year-uniq-rand
#purchase_history=${datapath}/marginal_net_utility/purchase-100-120-1year-uniq-top7.5w
#purchase_history=${datapath}/marginal_net_utility/purchase-100-120-1year-uniq-rand

#${binpath}/cross_selling_generator $purchase_history 50

#${binpath}/cross_selling_factorization_train $purchase_history-train 

${binpath}/cross_selling_factorization_predict $purchase_history-train.model $purchase_history-test

