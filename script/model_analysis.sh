#!/bin/bash

project_home=$(readlink -f $(dirname $0))/..
datapath=$project_home/data/
binpath=$project_home/bin/

model1=${datapath}/cross_selling/purchase-40-55-1year-top1w-sort-id-uniq-train.model
model2=${datapath}/markov_trans/purchase-40-55-1year-top1w-sort-id-uniq-train.model
purchase_history=${datapath}/cross_selling/purchase-40-55-1year-top1w-sort-id-uniq-train


${binpath}/model_analysis $model1 $model2 $purchase_history > tmp

