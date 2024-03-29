import nltk as nl
import SG
import numpy as np

file = open("D:\Spring 2024\Digital Design\Project\The Final Project\DD_1-Project\Graphing\output.sim" , "r")
files_tokens = file.readlines()
files_tokens_of_tokens = []

for i in range(len(files_tokens)):
    files_tokens_of_tokens.append(nl.word_tokenize(files_tokens[i]))

t = np.arange(1500)
SG.graph_all_outs(t,files_tokens_of_tokens, len(files_tokens))