import numpy as np 
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt
import numpy as np
import sys
import math
from scipy.integrate import fixed_quad
from scipy.integrate import quad
from scipy.special import hyp2f1
from scipy.special import beta
import os

#Compute Avalanches of Non novelties from the file obtained from novs.sh
def delays(novelties,flag):
    delay = np.diff(novelties)
    delay = np.array([i for i in delay if i!=1])
    delay = np.array([i-1 for i in delay])
    if(flag == True):
        return np.log(delay)
    else:
        return delay
#Compute Avalanches of Novelties from the file obtained from novs.sh
def avalanches_of_novelties(sequence,flag):
    interval_length = 1
    interval_lengths = [] 
    for j in range(1,len(sequence)):
        if sequence[j]-sequence[j-1] == 1:
            interval_length += 1
        else:
            if(flag == True):
                interval_lengths.append(np.log(interval_length))
            else:
                interval_lengths.append(interval_length)
            interval_length = 1
    #if(flag==True):
    #    distribution = np.unique(interval_lengths,return_counts=True)
    #    return distribution
    else:
        return interval_lengths
#Compute Heaps Exchange Parameters from the sequences obtained from heaps.sh and derive nu,rho,N0
def heaps_exchange(x, alfa, theta):
    return (theta/alfa)*((1+x/theta)**alfa) - theta/alfa

#Choose the resolution on which you want to estimate nu and rho since it holds the relation alpha = nu/rho
nu = np.arange(1, 50)
rho = np.arange(2, 51)
popt, pcov = curve_fit(heaps_exchange, x_heaps, y_heaps, p0=p0, maxfev=5000)
alfa_fit, theta_fit = popt

# Trova nu e rho tali che nu/rho sia il più vicino possibile a alfa
best_diff = float('inf')
best_nu = None
best_rho = None

for n in nu:
    for r in rho:
        ratio = n / r
        diff = abs(ratio - alfa_fit)
        if diff < best_diff:
            best_diff = diff
            best_nu = n
            best_rho = r

n0= int(best_rho*theta_fit)

# Compute the theoretical PDF for the Avalanches of Non Novelties (UMT-E):
# x is the length of the sequence, L is the largest avalanche at which you want to compute the pdf, theta and alpha are  estimated
# A is t=0, B= is the length of the sequence
def pLtris_con_cond0(x, L, theta, alpha):
    esponente=(alpha-1.0)
    prob=(1.0-((1.0+x/theta)**esponente))**(L)*(1.0+x/theta)**esponente
    return prob

def pLtristris_con_cond0(x, L, theta, alpha):
    esponente=(alpha-1.0)
    prob=(1.0-((1.0+x/theta)**esponente))**(L)*((1.0+x/theta)**esponente)**2.0
    return prob

def calculate_pdf(L_max,theta,alpha,A,B):
    x_ = []
    pdf = []
    norm,err = quad(pLtris_con_cond0, A, B, args=(1,theta,alpha))
    for L in range(1,L_max+1):
        pdf_,err = quad(pLtristris_con_cond0, A, B, args=(L,theta,alpha))
        pdf.append(pdf_/norm)
        x_.append(L)
    pdf = np.array(pdf)
    x_ = np.array(x_)
    return x_, pdf

# Compute the theoretical PDF for the Avalanches of Novelties (UMT-E)
# x is the length of the sequence, L is the largest avalanche at which you want to compute the pdf, theta and alpha are  estimated
# A is t=0, B= is the length of the sequence
def pLden_con_cond0_conalpha(x, theta, alpha):
    esponente=(alpha-1.0)
    prob=(1.0-((1.0+x/theta)**esponente))*(1.0+x/theta)**esponente
    return prob

def pLnum_con_cond0_conalpha(x, L, theta, alpha):
    esponente=(alpha-1.0)
    prob=(1.0-((1.0+x/theta)**esponente))**(2.0)*((1.0+x/theta)**esponente)**L
    return prob

def calculate_pdf(L_max,theta,alpha,A,B):
    x_ = []
    pdf = []
    norm,err = quad(pLden_con_cond0_conalpha, A, B, args=(theta,alpha))
    for L in range(1,L_max+1):
        pdf_,err = quad(pLnum_con_cond0_conalpha, A, B, args=(L,theta,alpha))
        pdf.append(pdf_/norm)
        x_.append(L)
    pdf = np.array(pdf)
    x_ = np.array(x_)
    return x_, pdf

