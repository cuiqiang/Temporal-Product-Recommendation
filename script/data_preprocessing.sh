#!/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

purchase_history=${datapath}/raw_data/purchase-40-55-1year-top1w
#purchase_history=${datapath}/raw_data/purchase-100-120-1year

sort -n -k1 -t  $purchase_history > $purchase_history-sort

${binpath}/data_preprocessing $purchase_history-sort

mv $purchase_history-sort-* ${datapath}/id_data
