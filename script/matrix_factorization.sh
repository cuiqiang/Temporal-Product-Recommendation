#!/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

#purchase_history=${datapath}/matrix_factorization/purchase-40-55-1year-top7.5w-sort-id-uniq
purchase_history=${datapath}/matrix_factorization/purchase-40-55-1year-top1w-sort-id-uniq
example=${datapath}/matrix_factorization/example

#${binpath}/matrix_factorization_generator $purchase_history 50

#${binpath}/matrix_factorization_train $purchase_history-train 

${binpath}/matrix_factorization_train $example

#{binpath}/matrix_factorization_predict $purchase_history-train.model $purchase_history-test
