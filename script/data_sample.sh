#!/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

purchase_history=${datapath}/raw_data/purchase-40-55-1year

${binpath}/sample_product -top $purchase_history $purchase_history-ptop10000

#${binpath}/sample_product -rand $purchase_history 8  
