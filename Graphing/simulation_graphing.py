import nltk as nl
import graphing_functions
import numpy as np

file = open("D:\Spring 2024\Digital Design\Project\project-1\output.sim" , "r")
files_tokens = file.readlines()
files_tokens_of_tokens = []

for i in range(len(files_tokens)):
    files_tokens_of_tokens.append(nl.word_tokenize(files_tokens[i]))

t = np.arange(1500)
graphing_functions.set_y_lim(len(files_tokens_of_tokens))
graphing_functions.graph_all_outs(t,files_tokens_of_tokens)
