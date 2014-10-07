#/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

#purchase_history=${datapath}/person_top/purchase-40-55-1year-top7.5w-sort-id-uniq
purchase_history=${datapath}/person_top/purchase-40-55-1year-top1w-sort-id-uniq

#${binpath}/person_top_generator $purchase_history

${binpath}/person_top_predict $purchase_history-train $purchase_history-test
