#/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

#purchase_history=${datapath}/marginal_net_utility/purchase-40-55-1year-top7.5w-sort-id-uniq
#purchase_history=${datapath}/top_pop/purchase-40-55-1year-top7.5w-sort-id-uniq
#purchase_history=${datapath}/matrix_factorization/purchase-40-55-1year-top7.5w-sort-id-uniq
#purchase_history=${datapath}/markov_trans/purchase-40-55-1year-top7.5w-sort-id-uniq
#purchase_history=${datapath}/person_top/purchase-40-55-1year-top7.5w-sort-id-uniq
#purchase_history=${datapath}/cross_selling/purchase-40-55-1year-top7.5w-sort-id-uniq
purchase_history=${datapath}/cross_selling/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/cross_selling_weight/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/markov_trans/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/marginal_net_utility/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/test/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/person_top/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/top_pop/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/matrix_factorization/purchase-40-55-1year-top1w-sort-id-uniq
#purchase_history=${datapath}/cross_selling_factorization/purchase-40-55-1year-top1w-sort-id-uniq

echo "user > n"
${binpath}/evaluation $purchase_history-test.predict "1*" "*"
echo "user > n re-purchase"
${binpath}/evaluation $purchase_history-test.predict "11" "*"
echo "user > n new purchase"
${binpath}/evaluation $purchase_history-test.predict "10" "*"
echo "user > n && product > n"
${binpath}/evaluation $purchase_history-test.predict "1*" "1"
echo "user > n && product > n re-purchase"
${binpath}/evaluation $purchase_history-test.predict "11" "1"
echo "user > n && product > n new purchase"
${binpath}/evaluation $purchase_history-test.predict "10" "1"

