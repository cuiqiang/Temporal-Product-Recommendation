#/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

purchase_history=${datapath}/collaborative_filtering/purchase-40-55-1year-top1w-sort-id-uniq

${binpath}/collaborative_filtering_generator $purchase_history

${binpath}/collaborative_filtering_train $purchase_history-train 

#${binpath}/collaborative_filtering_predict $purchase_history-train.model $purchase_history-test
