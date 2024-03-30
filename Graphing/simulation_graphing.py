import nltk as nl
import SG
import numpy as np
import math

file = open("D:\Spring 2024\Digital Design\Final version of the code\DD_1-Project\Graphing\out.sim" , "r")
files_tokens = file.readlines()
files_tokens_of_tokens = []

for i in range(len(files_tokens)):
    files_tokens_of_tokens.append(nl.word_tokenize(files_tokens[i]))

z = 0
for i in range(len(files_tokens_of_tokens)):
    if int(files_tokens_of_tokens[i][0]) > int(z):
        z = files_tokens_of_tokens[i][0]

t = np.arange(math.floor(int(z)*1.5))
SG.graph_all_outs(t,files_tokens_of_tokens, len(files_tokens) , int(int(z)*1.5) , 5)
