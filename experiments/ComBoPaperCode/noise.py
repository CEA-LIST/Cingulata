from math import sqrt
from math import pi
from scipy.special import erfc
from scipy.special import erf
from math import log2
from math import floor

# Use test_parameter_set to get log2 of probability of error #

n=1024  
N=1024
l=5
k=1
Bg=16
sigma=5.6*10**(-8)    # standard deviation
varBK=sigma*sigma   # variance clé de BS
varKS=sigma*sigma
mod=8
t=2
base=1024

# Basic boostrapping noise
# Works for half Torus, TOTA and Composition even or odd
def BSnoise(n, N, l, k, Bg, varBK):
	return(n*((k+1)*l*N*((Bg**2)/4)*varBK+(1+k*N)*(Bg**(-2*l))/4))

# n taille du vecteur input. N taille de l'output (TLWE de taille n to TRLWE de taille N)
def keySwitchAddedNoise(n, N, t, base, varKS):
	return(n*N*(t*varKS + (base**(-2*t))/4))  # varKS*(base/2)**2 depending on type of keyswitch


def FDFBnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol):
	norm_squared = 0
	for coef in pol:
		norm_squared += coef**2
	return((BSnoise(n, N, l, k, Bg, varBK)+keySwitchAddedNoise(N, 1, t, base, varKS))*(norm_squared+1)+keySwitchAddedNoise(n, N, t, base, varBK)*(norm_squared))

def MultValnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol):
	norm_squared = 0
	for coef in pol:
		norm_squared += coef**2
	return(BSnoise(n, N, l, k, Bg, varBK)*(norm_squared)+keySwitchAddedNoise(N, 1, t, base, varKS))

def probError(mod, var):
	return(erfc(1/(2*mod*sqrt(var)*sqrt(2))))

#variance dûe au rescaling du BS considérée sur le tore [0,1[
def varScal(n, N):
	return((n+1)/(48*N**2))

#Valable pour BS et FDFB
def pbErrBs(n, N, mod, var):
	return(probError(mod, var+varScal(n,N)))

#Valable pour half Torus
def pbErrHT(n, N, mod, var):
	return(probError(2*mod, var+varScal(n,N)))

#Valable pour TOTA
def pbErrTOTA(n, N, l, t, k, Bg, base, varBK, varKS, mod, var):
	x=probError(2*mod, var/4+BSnoise(n, N, l, k, Bg, varBK)+varScal(n,N)+keySwitchAddedNoise(N, 1, t, base, varKS))
	y=pbErrBs(n, N, 2, var)
	return(x+y+x*y)

# varBK = variance de la clé de BS
# var = variance du chiffré
# Works for standard ComBo and ComBoP
def pbErrComBo(n, N, l, t, k, Bg, base, varBK, varKS, mod, var):
	x=pbErrBs(n, N, mod, var)
	y=pbErrBs(n, N, mod, BSnoise(n, N, l, k, Bg, varBK)+keySwitchAddedNoise(N, 1, t, base, varKS))
	return(x+2*y+x*y*y-2*x*y-y*y)   # 1-(1-x)*(1-y)*(1-y)=x+2y+xyy-2xy-yy

def pbErrComBoMV(n, N, l, t, k, Bg, base, varBK, varKS, mod, var, pol1, pol2):
	x=pbErrBs(n, N, mod, var)
	y=pbErrBs(n, N, mod, MultValnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol1))
	z=pbErrBs(n, N, mod, MultValnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol2))
	return(x+y+z+x*y*z-x*y-x*z-y*z)

def pbErrComBoMVReLUspe(n, N, l, t, k, Bg, base, varBK, varKS, mod, var, pol):
	x=pbErrBs(n, N, mod, var)
	y=pbErrBs(n, N, mod, MultValnoise(n, N, l, k, Bg, varBK, pol))
	return(x+y-x*y)


def test_parameter_set(n, N, l, t, k, Bg, base, varBK, varKS, mod):
	var1 = BSnoise(n, N, l, k, Bg, varBK) +  keySwitchAddedNoise(N, 1, t, base, varKS)   # bruit de base
	
	pol = [1 for i in range((int)(mod/2))]
	pol[0] = mod/2-1
	var2 = FDFBnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol) # bruit relu fdfb
	
	pol = [-2 for i in range((int)(mod/2-1))]
	var3 = FDFBnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol) # bruit id fdfb
	
	pol = [mod-1 for i in range((int)(mod/2))]
	pol[0] = mod
	var4 = FDFBnoise(n, N, l, t, k, Bg, base, varBK, varKS, pol) # bruit worst case fdfb
	
	pol1 = [1 for i in range((int)(mod/2))]
	pol1[0]=mod/2                                # pol id
	
	pol2 = [1 for i in range((int)(mod/2))]
	pol2[0]=0                                    # pol abs
	
	print("round noise = ", varScal(n, N))
	print("noise gate bootstrap = ", var1)
	print("pbErrBs (gate bootstrap) = ", log2(pbErrBs(n, N, mod, var1)))
	print("pbErrBs (FDFB ReLU) = ", log2(pbErrBs(n, N, mod, var2)))
	print("pbErrBs (FDFB Id) = ", log2(pbErrBs(n, N, mod, var3)))
	print("pbErrBs (FDFB worst case) = ", log2(pbErrBs(n, N, mod, var4)))
	print("pbErrHT = ", log2(pbErrHT(n, N, mod, var1)))
	print("pbErrTOTA = ", log2(pbErrTOTA(n, N, l, t, k, Bg, base, varBK, varKS, mod, var1)))
	print("pbErrComBo \"worst case\" = ", log2(pbErrComBo(n, N, l, t, k, Bg, base, varBK, varKS, mod, 2*var1)))
	print("pbErrComBo ReLU Multival = ", log2(pbErrComBoMV(n, N, l, t, k, Bg, base, varBK, varKS, mod, 2*var1, pol1, pol2)))
