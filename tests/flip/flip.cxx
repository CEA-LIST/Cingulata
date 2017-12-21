/*
    (C) Copyright 2017 CEA LIST. All Rights Reserved.
    Contributor(s): Cingulata team (formerly Armadillo team)

    This software is governed by the CeCILL-C license under French law and
    abiding by the rules of distribution of free software.  You can  use,
    modify and/ or redistribute the software under the terms of the CeCILL-C
    license as circulated by CEA, CNRS and INRIA at the following URL
    "http://www.cecill.info".

    As a counterpart to the access to the source code and  rights to copy,
    modify and redistribute granted by the license, users are provided only
    with a limited warranty  and the software's author,  the holder of the
    economic rights,  and the successive licensors  have only  limited
    liability.

    The fact that you are presently reading this means that you have had
    knowledge of the CeCILL-C license and that you accept its terms.
*/

/* 
    This code is a Cingulata port of the FLIP reference code provided by 
    Anthony Journault, Pierrick Meaux and François-Xavier Standaert and is 
    included here with their agreement. 

    Thanks for that folks :-)
*/

/* compiler includes */
#include <iostream>
#include <fstream>

#include <integer.hxx>

/* namespaces */
using namespace std;

#include "aes.c"

//***********************************************************************
//Forward secure PRNG using AES
static unsigned char state_random[16] = {0xab,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf};
static unsigned char ciphertext[16] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
static unsigned char flag = 0;
uint32_t aes_forward_secure()
{
	uint32_t rand;
	
	if (flag != 0)
	{
		switch (flag)
		{
			case 1: rand = ciphertext[4] ^ (ciphertext[5]<<8) ^ (ciphertext[6]<<16) ^ (ciphertext[7]<<24);
				flag++;
			break;
			case 2: rand = ciphertext[8] ^ (ciphertext[9]<<8) ^ (ciphertext[10]<<16) ^ (ciphertext[11]<<24);
				flag++;
			break;
			case 3: rand = ciphertext[12] ^ (ciphertext[13]<<8) ^ (ciphertext[14]<<16) ^ (ciphertext[15]<<24);
				flag = 0;
			break;
			default : printf("Error in the flag of aes forward secure prng");
			
		}
	}
	else
	{
		unsigned char plaintext0[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
		unsigned char plaintext1[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; 
	
 
		uint key_schedule[44];

		KeyExpansion(state_random,key_schedule,128);
		aes_encrypt(plaintext0,state_random,key_schedule,128);
		aes_encrypt(plaintext1,ciphertext,key_schedule,128);
		rand = ciphertext[0] ^ (ciphertext[1]<<8) ^ (ciphertext[2]<<16) ^ (ciphertext[3]<<24);
		flag++;
	}
	return rand;
}
//***********************************************************************
//Knuth shuffle taken from https://www.rosettacode.org/wiki/Knuth_shuffle
/* random integer from 0 to n-1 */
void shuffle(long * list, long len) 
{		
	long j;						
	long tmp;
	while(len) {
									
		j = aes_forward_secure(); 						
		j = j % len;			
		if (j != len - 1) {
			tmp = list[j];		
			list[j] = list[len - 1];	
			list[len - 1] = tmp;
		}						
		len--;					
	}						
}						
//***********************************************************************
//FLIP code
//state is the key register
//indice is here to have free-fhe shuffle
//mf is the vector describing the filtering function as follows: m[i] is the number of monomials of degree i 
//size is the number of variables
Bit FLIP (Bit* state, long * indice, long * mf, long size)
{
	Bit temp,output;
	long i,j,k;
	long nb = 0;
	
	output = 0;
	//Computation of Linear monomials
	for (i = 0 ; i < mf[0] ; i++)
	{
		output ^=  state[indice[nb++]];
	}
	
	//Computation of higher degree monomials
	for ( i = 1 ; i < size ; i++)
	{
		for ( j = 0 ; j < mf[i] ; j++)
		{
			temp = state[indice[nb++]] * state[indice[nb++]];
			
			for ( k = 2 ; k < (i+1) ; k++ )
			{
				temp *= state[indice[nb++]];
			}
			
			output ^= temp;
		}
	}
	return output;
}

int main()
{

	long i,j;
	long size = 9;//algebraic degree of the filtering function
	long mf[9] = {50,72,8,8,8,8,8,8,8};//description of the filtering function
	long nb_var = 0;
	Bit out;
	
	//Initialisation phase
	for ( i = 0 ; i < size ; i++ )
	{
		nb_var += (i+1)*mf[i];
	}
	
	long* indice = (long*)malloc(nb_var*sizeof(long));
	
	for ( i = 0 ; i < nb_var ; i++ )
	{
		indice[i] = i;
	}
	
	Bit* state = (Bit*)malloc(nb_var*sizeof(Bit));
	
	for(i=0;i<nb_var;i++)
		cin>>state[i];
	/*for ( i = 0 ; i < nb_var/2 ; i++ )
	{
		state[i] = 0;
	}
	for ( i = nb_var/2 ; i < nb_var ; i++ )
	{
		state[i] = 1;
	}*/
	
	//Computation of FLIP
	for ( i = 0; i < 10 ; i++)
	{	
		shuffle(indice,nb_var);
		out = FLIP( state, indice, mf, size);
		//printf("%ld",out);
		cout<<out;
	}
	
  	FINALIZE_CIRCUIT("flip.blif");
}

