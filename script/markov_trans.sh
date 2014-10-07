#/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

#purchase_history=${datapath}/markov_trans/purchase-40-55-1year-top7.5w-sort-id-uniq
purchase_history=${datapath}/markov_trans/purchase-40-55-1year-top1w-sort-id-uniq

${binpath}/markov_trans_generator $purchase_history

head -n 1  $purchase_history-train > $purchase_history-train.stat
sed -i '1d' $purchase_history-train

sort -k2,2n -k1,1n -t " " $purchase_history-train > $purchase_history-train-sort

cat $purchase_history-train.stat $purchase_history-train-sort > $purchase_history-train

${binpath}/markov_trans_train $purchase_history-train
${binpath}/markov_trans_predict $purchase_history-train.model $purchase_history-test
