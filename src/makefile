CPP = g++
OFLAG = -o
COMPILE.cxx= $(CPP) -c

MODEL_PATH=./model
EXP_PATH=./experiment
EVAL_PATH=./evaluation
COMMON_PATH=./common
GEN_PATH=./generator
SOURCES= $(wildcard $(COMMON_PATH)/*.cpp) $(wildcard $(MODEL_PATH)/*.cpp) $(wildcard $(EVAL_PATH)/*.cpp) $(wildcard $(EXP_PATH)/*.cpp) $(wildcard $(GEN_PATH)/*.cpp)
OBJS_NAME=$(notdir $(patsubst %.cpp,%.o,$(SOURCES)))
OBJS=$(addprefix $(DEBUG_PATH)/,$(notdir $(patsubst %.cpp,%.o,$(SOURCES))))
BIN_PATH=../bin
DEBUG_PATH=../debug

TP_MODEL_TRAIN = util.o top_pop.o top_pop_train.o
TP_MODEL_PREDICT = util.o top_pop.o	top_pop_predict.o 
MF_MODEL_TRAIN = util.o matrix_factorization.o matrix_factorization_train.o
MF_MODEL_PREDICT = util.o matrix_factorization.o matrix_factorization_predict.o
UTIL_MODEL_TRAIN = util.o matrix_factorization.o marginal_net_utility.o marginal_net_utility_train.o 
UTIL_MODEL_PREDICT = util.o matrix_factorization.o marginal_net_utility.o marginal_net_utility_predict.o 
CROSS_MODEL_TRAIN = util.o matrix_factorization.o cross_selling.o cross_selling_train.o 
CROSS_MODEL_PREDICT = util.o matrix_factorization.o cross_selling.o cross_selling_predict.o
CROSS_WEIGHT_TRAIN = util.o matrix_factorization.o cross_selling_weight.o cross_selling_weight_train.o 
CROSS_WEIGHT_PREDICT = util.o matrix_factorization.o cross_selling_weight.o cross_selling_weight_predict.o
CSF_MODEL_TRAIN = util.o matrix_factorization.o cross_selling_factorization.o cross_selling_factorization_train.o 
CSF_MODEL_PREDICT = util.o matrix_factorization.o cross_selling_factorization.o cross_selling_factorization_predict.o
CF_MODEL_TRAIN = util.o collaborative_filtering.o collaborative_filtering_train.o 
CF_MODEL_PREDICT = util.o collaborative_filtering.o collaborative_filtering_predict.o
MT_MODEL_TRAIN = util.o markov_trans.o markov_trans_train.o
MT_MODEL_PREDICT = util.o markov_trans.o markov_trans_predict.o
PT_MODEL_PREDICT = util.o person_top.o person_top_predict.o
PI_MODEL = util.o matrix_factorization.o marginal_net_utility.o purchase_interval.o purchase_interval_main.o
EVAL = util.o evaluation.o evaluation_main.o
TP_GEN = util.o top_pop_generator.o top_pop_generator_main.o
CF_GEN = util.o collaborative_filtering_generator.o collaborative_filtering_generator_main.o
MT_GEN = util.o markov_trans_generator.o markov_trans_generator_main.o
PT_GEN = util.o person_top_generator.o person_top_generator_main.o
UTIL_GEN = util.o matrix_factorization_generator.o marginal_net_utility_generator.o marginal_net_utility_generator_main.o
CROSS_GEN = util.o matrix_factorization_generator.o cross_selling_generator.o cross_selling_generator_main.o
MF_GEN = util.o matrix_factorization_generator.o matrix_factorization_generator_main.o
PI_GEN = util.o PurchaseIntervalGenerator.o PurchaseIntervalGenerator_main.o 
STAT = util.o dataset_analysis.o dataset_analysis_main.o
SAMPLE = util.o sample_product.o sample_product_main.o
DATA_PRE = util.o data_preprocessing.o data_preprocessing_main.o
HIS_INFLUENCE = util.o NewPurchaseDataAnalysis.o NewPurchaseDataAnalysis_main.o
MODEL_CMP = util.o model_analysis.o model_analysis_main.o

PRE=dataset_analysis sample_product data_preprocessing history_influence_analysis model_analysis
TP=top_pop_train top_pop_predict top_pop_generator
MF=matrix_factorization_train matrix_factorization_predict matrix_factorization_generator 
UTIL=marginal_net_utility_train marginal_net_utility_predict marginal_net_utility_generator
CROSS=cross_selling_train cross_selling_predict cross_selling_generator
CROSS_WEIGHT=cross_selling_weight_train cross_selling_weight_predict cross_selling_generator
CSF=cross_selling_factorization_train cross_selling_factorization_predict cross_selling_generator
CF=collaborative_filtering_train collaborative_filtering_predict collaborative_filtering_generator
EVA=evaluation
PI=purchase_interval_generator
MT=markov_trans_train markov_trans_predict markov_trans_generator
PT=person_top_predict person_top_generator

pre:$(addprefix $(BIN_PATH)/, $(PRE))
tp:$(addprefix $(BIN_PATH)/, $(TP))
mf:$(addprefix $(BIN_PATH)/, $(MF))
util:$(addprefix $(BIN_PATH)/, $(UTIL))
cross:$(addprefix $(BIN_PATH)/, $(CROSS))
cw:$(addprefix $(BIN_PATH)/, $(CROSS_WEIGHT))
csf:$(addprefix $(BIN_PATH)/, $(CSF))
cf:$(addprefix $(BIN_PATH)/, $(CF))
eval:$(addprefix $(BIN_PATH)/, $(EVA))
pi:$(addprefix $(BIN_PATH)/, $(PI))
mt:$(addprefix $(BIN_PATH)/, $(MT))
pt:$(addprefix $(BIN_PATH)/, $(PT))

# Rules for generating object files

$(OBJS):$(SOURCE)
	$(CPP) $(OFLAG) $@ -c $(filter %$(*F).cpp,$(SOURCES))

#
$(BIN_PATH)/top_pop_train: $(addprefix $(DEBUG_PATH)/, $(TP_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/top_pop_predict: $(addprefix $(DEBUG_PATH)/, $(TP_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/matrix_factorization_train: $(addprefix $(DEBUG_PATH)/, $(MF_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/matrix_factorization_predict: $(addprefix $(DEBUG_PATH)/, $(MF_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/marginal_net_utility_train: $(addprefix $(DEBUG_PATH)/, $(UTIL_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/marginal_net_utility_predict: $(addprefix $(DEBUG_PATH)/, $(UTIL_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_train: $(addprefix $(DEBUG_PATH)/, $(CROSS_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_predict: $(addprefix $(DEBUG_PATH)/, $(CROSS_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_weight_train: $(addprefix $(DEBUG_PATH)/, $(CROSS_WEIGHT_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_weight_predict: $(addprefix $(DEBUG_PATH)/, $(CROSS_WEIGHT_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_factorization_train: $(addprefix $(DEBUG_PATH)/, $(CSF_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_factorization_predict: $(addprefix $(DEBUG_PATH)/, $(CSF_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/collaborative_filtering_train: $(addprefix $(DEBUG_PATH)/, $(CF_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/collaborative_filtering_predict: $(addprefix $(DEBUG_PATH)/, $(CF_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/markov_trans_train: $(addprefix $(DEBUG_PATH)/, $(MT_MODEL_TRAIN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/markov_trans_predict: $(addprefix $(DEBUG_PATH)/, $(MT_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/person_top_predict: $(addprefix $(DEBUG_PATH)/, $(PT_MODEL_PREDICT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/purchase_interval: $(addprefix $(DEBUG_PATH)/, $(PI_MODEL))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/evaluation: $(addprefix $(DEBUG_PATH)/, $(EVAL))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/collaborative_filtering_generator: $(addprefix $(DEBUG_PATH)/, $(CF_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/top_pop_generator: $(addprefix $(DEBUG_PATH)/, $(TP_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/person_top_generator: $(addprefix $(DEBUG_PATH)/, $(PT_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/markov_trans_generator: $(addprefix $(DEBUG_PATH)/, $(MT_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/marginal_net_utility_generator: $(addprefix $(DEBUG_PATH)/, $(UTIL_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/cross_selling_generator: $(addprefix $(DEBUG_PATH)/, $(CROSS_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/matrix_factorization_generator: $(addprefix $(DEBUG_PATH)/, $(MF_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/purchase_interval_generator: $(addprefix $(DEBUG_PATH)/, $(PI_GEN))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/dataset_analysis: $(addprefix $(DEBUG_PATH)/, $(STAT))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/sample_product: $(addprefix $(DEBUG_PATH)/, $(SAMPLE))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/data_preprocessing: $(addprefix $(DEBUG_PATH)/, $(DATA_PRE))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/history_influence_analysis: $(addprefix $(DEBUG_PATH)/, $(HIS_INFLUENCE))
	$(CPP) $(OFLAG) $@ $^
$(BIN_PATH)/model_analysis: $(addprefix $(DEBUG_PATH)/, $(MODEL_CMP))
	$(CPP) $(OFLAG) $@ $^

clean:
	rm $(DEBUG_PATH)/*.o
