#!/usr/bin/python3

#
#    (C) Copyright 2017 CEA LIST. All Rights Reserved.
#    Contributor(s): Cingulata team (formerly Armadillo team)
#
#    This software is governed by the CeCILL-C license under French law and
#    abiding by the rules of distribution of free software.  You can  use,
#    modify and/ or redistribute the software under the terms of the CeCILL-C
#    license as circulated by CEA, CNRS and INRIA at the following URL
#    "http://www.cecill.info".
#
#    As a counterpart to the access to the source code and  rights to copy,
#    modify and redistribute granted by the license, users are provided only
#    with a limited warranty  and the software's author,  the holder of the
#    economic rights,  and the successive licensors  have only  limited
#    liability.
#
#    The fact that you are presently reading this means that you have had
#    knowledge of the CeCILL-C license and that you accept its terms.
#



"""@package genParams

[Disclaimer]
This script generates parameters for the Fan-Vercauteren cryptosystem following the original publication (IACR ePrint report 2012/144). It is provided on an AS IS basis. If you need up-to-date security levels you should consider more recent complexity estimates for known attacks on RLWE.

[Short description]
This script is used to generate parameters for Fan-Vercauteren (FV for short) leveled homomorphic encryption following the original publication by these two authors (IACR ePrint report 2012/144).

[First use] 
To use this script properly, type:
python3 genParams.py -h

This command describes input parameters and how to use them. It also gives default parameters

[Toy example] 
To use this script with default values, type:
python3 genParams.py

[Input/Output]
Input parameters are displayed on the standard output.
Generated parameters describes probability distributions and indicates moduli bitsize.
Output is a xml file in which generated parameters and input parameters are written.

[Additional info]
The script define two parameters sets. 
The first parameter set is used during key generation, encryption.
The second one is used during relinearisation (version 2).

Source: Fan, Junfeng, and Frederik Vercauteren. "Somewhat Practical Fully Homomorphic Encryption." IACR Cryptology ePrint Archive 2012 (2012): 144.
URL: https://eprint.iacr.org/2012/144.pdf

\author CEA-List, Embedded Real-Time System foundations Lab (DACLE/LaSTRE)
"""
import sys
import os
import argparse
import mpmath as mpm
from xml.dom import minidom
import numpy as np

#######
# Read config file with name=value pairs, ConfigParse module

class _Parameters:
  def __init__(self):
    None
    
  def __getitem__(self, key):
    return self.__dict__[key]

  def updateParams(self, params_dict):
    self.__dict__.update(params_dict)

  def _parse_param(self, xmldoc, paramName):
    a = xmldoc.getElementsByTagName(paramName)
    if len(a) > 0:
      self.__dict__[paramName] = float(a[0].firstChild.data)
      
  def parseXml(self, xmlStr):
    xmldoc = minidom.parseString(xmlStr)

    for key in self._params_defaults.keys():
      self._parse_param(xmldoc, key)
    
  def write(self):
    print ("FHE scheme parameters:")
    for key,val in self.__dict__.items():
      print ('\t{0}: {1}'.format(key, val))

class _ParametersGenerator:
  def __init__(self, params):
    self._eps_exp = params['eps_exp'] # Exponent of adversary's advantage (i.e. success probability) in distinguishing attack on decision-LWE described by Lindner and Peikert. https://eprint.iacr.org/2010/613.pdf
    self._h = params['sk_hamm'] # Hamming weight of the secret key
    self._lambda_p = params['lambda_p'] # Security level
    self.t = params['pt_base'] # Maximum plaintext coefficient
    self.L = params['mult_depth'] # Circuit multiplicative depth
    self.k = params['relin_k']
    self.cyclotomic_poly_order = params['cyclotomic_poly_order']
    self.ring_degree_log2 = int(np.log2(self.cyclotomic_poly_order)) - 1 
    
    mpm.mp.prec = 128
    self.comp_init_params()

  def comp_init_params(self):
    self._alfa = self._comp_alfa(self._eps_exp)
    self._beta = self._comp_beta(self._eps_exp)
    self._log2_delta = self._comp_log2_delta(self._lambda_p)
    self.size_t = mpm.ceil(mpm.log(self.t,2) + 0.5)

  def _round_up(self, a, dps):
    m = mpm.power(10, dps)
    return mpm.ceil(a * m) / m

  def _comp_alfa(self, eps_exp):
    eps = mpm.power(2, eps_exp)
    alfa = mpm.sqrt(mpm.ln(1 / eps) / mpm.pi)
    alfa = self._round_up(alfa, 3)
    return alfa

  def _comp_beta(self, eps_exp):
    eps = mpm.power(2, eps_exp)
    beta = mpm.erfinv(1-eps) * mpm.sqrt(2)
    return beta

  def _comp_B(self, beta, sigma):
    return mpm.ceil(beta * sigma)

  def _comp_log2_delta(self, _lambda_p):
    log_delta = 1.8 / (_lambda_p + 110)
    return log_delta

  def _comp_sigma_k(self, sigma, q, k):
    sigma_k = mpm.power(self._alfa, 1-mpm.sqrt(k))
    sigma_k *= mpm.power(q, k-mpm.sqrt(k))
    sigma_k *= mpm.power(sigma, mpm.sqrt(k))
    sigma_k = mpm.ceil(sigma_k)
    return sigma_k

  def comp_min_depth(self, t, h):
    """ Compute minimal circuit depth needed for FHE,
        thus circuit depth needed for decryption circuit.
        The depth is applicable only in case when t (plain-text base)
        divides q (cypher-text base).
    """
    _gamma = 2.01     # constant: 2 < _gamma < 3
    _H_f = 1          # computed using equation (5) from the article
    v = mpm.ceil(t * _gamma * (_H_f * h + 1))
    L = mpm.ceil(mpm.log(v + 0.5, 2))
    return L

  def comp_log2_sigma_lb_given_modulo_degree(self, log2_modulo, log2_degree):
    """
    Compute the lower bound of noise function sigma (log 2 base).
    This method uses formula (6) from the article.

    Arguments:
    log2_modulo -- log base 2 of ring modulo
    log2_degree -- log base 2 of polynomial degree d
    """
    log2_sigma_lb = mpm.log(self._alfa, 2) + \
                    log2_modulo - \
                    2 * mpm.sqrt(mpm.power(2,log2_degree) * log2_modulo * self._log2_delta)
    return log2_sigma_lb

  def comp_log2_sigma_ub_given_modulo_degree(self, log2_modulo, log2_degree):
    """
    Compute the upper bound of noise function sigma (log 2 base).
    This method uses formula (7) from the article.

    Arguments:
    log2_modulo -- log base 2 of ring modulo
    log2_degree -- log base 2 of polynomial degree d
    """
    
    degree = mpm.power(2,log2_degree)

    B = 2 + \
        mpm.log(self._beta, 2) + \
        self.L * log2_degree + \
        (self.L + 1) * mpm.log(degree + 1.25, 2) + \
        (self.L - 1) * mpm.log(self.t, 2)
    
    log2_sigma_up = log2_modulo - B

    return log2_sigma_up

  def comp_log2_modulo_lb_given_degree(self, log2_degree):
    """
    Compute the base 2 logarithm of polynomial ring coeficients modulo.
    This method uses formula (6) and (7) from the article.

    Arguments:
    log2_degree -- log base 2 of polynomial degree d
    """        
    degree = mpm.power(2,log2_degree)

    A = 2 + \
        mpm.log(self._alfa, 2) + \
        mpm.log(self._beta, 2) + \
        self.L * log2_degree + \
        (self.L + 1) * mpm.log(degree + 1.25, 2) + \
        (self.L - 1) * mpm.log(self.t, 2)

    log2_modulo_lb = mpm.power(A, 2) / (4 * degree * self._log2_delta)

    return log2_modulo_lb

  def comp_all_params_given_degree(self):
    log2_modulo_lb = self.comp_log2_modulo_lb_given_degree(self.ring_degree_log2)
    self.log2_q = mpm.ceil(log2_modulo_lb)
    #print self.log2_q
    log2_sigma_lb = self.comp_log2_sigma_lb_given_modulo_degree(self.log2_q, self.ring_degree_log2)
    self.sigma = mpm.power(2, log2_sigma_lb)
    self.B = self._comp_B(self._beta, self.sigma)

    self._comp_relin_v2_params()

    return self.log2_q, self.sigma, log2_sigma_lb

  def out_mpf_C_str(self, mpf):
    prec = int(mpm.nstr(mpf, 0).split('e')[-1]) + 1
    mpf_str = mpm.nstr(mpf, prec)
    mpf_int_str = mpf_str.split('.')[0]

    r = range(0, len(mpf_int_str), 76) + [len(mpf_int_str)]
    res = '  "'

    for i in xrange(0, len(mpf_int_str), 76):
      res += mpf_int_str[i:(i+76)]
      if i+78 < len(mpf_int_str):
        res += '\\\n  '

    res += '"'

    return res

  def _comp_relin_v2_params(self):
    self.log2_p = int(self.log2_q) * (self.k-1)
    self.log2_pq = int(self.log2_q) * self.k
    self.sigma_k = self._comp_sigma_k(self.sigma, self.log2_q, self.k)
    self.B_k = self._comp_B(self._beta, self.sigma_k)
    

  def substitute_params(self, tempStr):
    tempStr = tempStr.replace("@FHE_PARAM_T@", "{0}".format(int(self.t)))
    tempStr = tempStr.replace("@FHE_PARAM_SIZE_T@", "{0}".format(int(self.size_t)))
    tempStr = tempStr.replace("@FHE_PARAM_LOG2_Q@", "{0}".format(int(self.log2_q)))
    tempStr = tempStr.replace("@FHE_PARAM_LOG2_P@", "{0}".format(int(self.log2_p)))
    tempStr = tempStr.replace("@FHE_PARAM_LOG2_PQ@", "{0}".format(int(self.log2_pq)))
    tempStr = tempStr.replace("@FHE_PARAM_LOG2_D@", "{0}".format(int(self.ring_degree_log2)))
    tempStr = tempStr.replace("@FHE_PARAM_D@", "{0}".format(int(2 ** self.ring_degree_log2)))
    tempStr = tempStr.replace("@FHE_PARAM_SIGMA_STR@", "{0}".format(self.out_mpf_C_str(self.sigma)))
    tempStr = tempStr.replace("@FHE_PARAM_B_STR@", "{0}".format(self.out_mpf_C_str(self.B)))
    tempStr = tempStr.replace("@FHE_PARAM_SIGMA_K_STR@", "{0}".format(self.out_mpf_C_str(self.sigma_k)))
    tempStr = tempStr.replace("@FHE_PARAM_B_K_STR@", "{0}".format(self.out_mpf_C_str(self.B_k)))
    tempStr = tempStr.replace("@FHE_PARAM_SK_H@", "{0}".format(int(self._h)))
    tempStr = tempStr.replace("@FHE_PARAM_L@", "{0}".format(int(self.L)))

    return tempStr


  def mpf2str(self, mpf):
    prec = int(mpm.nstr(mpf, 0).split('e')[-1]) + 1
    mpf_str = mpm.nstr(mpf, prec)
    mpf_int_str = mpf_str.split('.')[0]
    return mpf_int_str


  def createPolynomialRingNode(self, doc):
    def createCoeffNode(doc, value, degree):
      coeff = doc.createElement("coeff")

      v = doc.createElement("value")
      v.appendChild(doc.createTextNode(str(int(value))))
          
      d = doc.createElement("degree")
      d.appendChild(doc.createTextNode(str(int(degree))))

      coeff.appendChild(v)
      coeff.appendChild(d)

      return coeff

    def createPolyNode(doc, poly):
      cfs = doc.createElement("coeffs")
      for degree, coeff in poly.dict().items():
        cfs.appendChild(createCoeffNode(doc, coeff, degree))
      return cfs    

    def createFactorsNode(doc, poly_factors):
      fsn = doc.createElement("factors")
      
      for order in xrange(len(poly_factors)):        
        factor, exponent = poly_factors[order]
        assert(exponent == 1)

        fn = doc.createElement("factor")
        
        #fn.setAttribute("order", str(order))
        on = doc.createElement("order")
        on.appendChild(doc.createTextNode(str(order)))
        fn.appendChild(on)       

        #fn.setAttribute("exponent", str(exponent))
        on = doc.createElement("exponent")
        on.appendChild(doc.createTextNode(str(exponent)))
        fn.appendChild(on)       

        fn.appendChild(createPolyNode(doc, factor))

        fsn.appendChild(fn)
        
      return fsn
    
    pr = doc.createElement("polynomial_ring")
    mp = doc.createElement("cyclotomic_polynomial")

    #write cyclotomic polynomial order
    on = doc.createElement("order")
    on.appendChild(doc.createTextNode(str(int(2 * 2 ** self.ring_degree_log2))))
    mp.appendChild(on)
    
    pr.appendChild(mp)

    return pr

  def createPlaintextNode(self, doc):
    pt = doc.createElement("plaintext")
    cm = doc.createElement("coeff_modulo")
    
    cm.appendChild(doc.createTextNode(str(int(self.t))))
    pt.appendChild(cm)

    return pt

  def createNormalDistributionNode(self, doc, sigma, bound):
    nd = doc.createElement("normal_distribution")

    sn = doc.createElement("sigma")
    nd.appendChild(sn)

    sn.appendChild(doc.createTextNode(sigma))    

    bn = doc.createElement("bound")
    nd.appendChild(bn)

    bn.appendChild(doc.createTextNode(bound))    
      
    return nd
  
  def createCiphertextNode(self, doc):
    ct = doc.createElement("ciphertext")
    cm = doc.createElement("coeff_modulo_log2")
    ct.appendChild(cm)

    cm.appendChild(doc.createTextNode(str(int(self.log2_q))))

    ct.appendChild(self.createNormalDistributionNode(doc, self.mpf2str(self.sigma), self.mpf2str(self.B)))
    
    return ct
  
  def createLinearizationNode(self, doc):
    ln = doc.createElement("linearization")

    #write linearization version
    n = doc.createElement("version")
    ln.appendChild(n)

    n.appendChild(doc.createTextNode("2"))

    #write linearization coefficient modulo
    n = doc.createElement("coeff_modulo_log2")
    ln.appendChild(n)

    n.appendChild(doc.createTextNode(str(int(self.log2_p))))
    
    #write linearization normal distribution
    ln.appendChild(self.createNormalDistributionNode(doc, self.mpf2str(self.sigma_k), self.mpf2str(self.B_k)))

    return ln

  def createSecretKeyNode(self, doc):
    skn = doc.createElement("secret_key")

    n = doc.createElement("hamming_weight")
    skn.appendChild(n)

    n.appendChild(doc.createTextNode(str(int(self._h))))

    return skn

  def getXml(self):
    doc = minidom.Document()

    params = doc.createElement("fhe_params")

    params.appendChild(self.createPolynomialRingNode(doc))
    params.appendChild(self.createPlaintextNode(doc))    
    params.appendChild(self.createCiphertextNode(doc))    
    params.appendChild(self.createLinearizationNode(doc))    
    params.appendChild(self.createSecretKeyNode(doc))    

    doc.appendChild(params)    

    return doc


# Parse command line arguments
parser = argparse.ArgumentParser(
  description='Generate parameters (XML file) for FV homomorphic encryption scheme',
  formatter_class=argparse.ArgumentDefaultsHelpFormatter,
  argument_default=argparse.SUPPRESS)

groupFile = parser.add_argument_group("configuration file input")
groupFile.add_argument('-c', '--config_file', help='Configuration file (XML)', type=argparse.FileType('r'))

groupArgs = parser.add_argument_group("command line input", "has priority over configuration file")
groupArgs.add_argument('--sk_hamm', help='Secret key hamming weight', default = 63, type = int)
groupArgs.add_argument('--lambda_p', help='Security level', default = 128, type = int)
groupArgs.add_argument('--pt_base', help='Plain text base', default = 2, type = int)
groupArgs.add_argument('--mult_depth', help='Multiplicative depth', default = 5, type = int)
groupArgs.add_argument('--relin_k', help='Parameter k for the relinearization', default = 4, type = int, choices=[4,5])
groupArgs.add_argument('--eps_exp', help='Epsilon exponent', default = -64, type = int)

groupPoly = parser.add_argument_group("polynomial ring quotient", "cyclotomic polynomial Phi_m(x) parameters, for the moment only m=2^n polynomials are supported")
groupPoly.add_argument('--cyclotomic_poly_order', help='Cyclotomic polynomial order, m', default = 2**10, type = int)

groupOut = parser.add_argument_group("output")
groupOut.add_argument('--output_xml', help='Output parameters file', default = "fhe_params.xml", type=str)

try:
  values = parser.parse_args(sys.argv[1:])
except IOError as msg:
  parser.error(str(msg))

#Build parameters object
params = _Parameters()
if 'config_file' in values:
  params.parseXml(values.config_file.readlines())
params.updateParams(values.__dict__)
params.write()

#Generate homomorphic encryption scheme parameters
paramsGen = _ParametersGenerator(params)
paramsGen.comp_all_params_given_degree()

xmlStr = paramsGen.getXml().toprettyxml()

f = open(params.output_xml, "w")
f.write(xmlStr)
f.close()





