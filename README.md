This is the directory containing all the codes and scripts used for the realization of the work "The Avalanches Size Statistics in Innovation Processes".

In the folder UTILS you find the python code to compute avalanches of novelties, avalanches of non novelties and compute their distributions.

In the folder SCALING you find a file SCALING.ipynb that contains the code used to perform the scaling.

In the folder URN_MODELS you find the C++ code for the Urn Model with Triggering (UMT), Urn Model with Triggering in its exchangeable version (UMT-E),
Urn Model with Semantic Triggering (UMST) and the Urn Model with Semantic Triggering in its Exchangeable version (UMST-E). 

In the folder THEORETICAL you find the code to compute the fit for the UMT-E and the probability distribution for both the avalanches of novelties and non novelties
predicted by the UMT-E.

In the folder MAPPING you find the procedures adopted for  the mapping process shown in the paper.
It starts with the jupyter notebook named fit_heaps.ipynb that takes as input a folder of HEAPS sequences and returns a .txt file with the parameters of the UMT-E 
estimated.
It excludes automatically sequences for which the fits don't work. Than the UMT-E code is suitable modified to take the txt file and generate the UMT-E
sequences corresponding to the written texts. Then the mapping.ipynb performs the mapping between the UMT-E sequences and the written texts.
