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

/* compiler includes */
#include <iostream>
#include <fstream>

#include <integer.hxx>

/* namespaces */
using namespace std;


/*Integer8 hammingWeight0(Bit *x, unsigned int N){
	Integer8 weight=0;
	for (unsigned index_v=0; index_v < N; ++index_v) {
		weight=weight+Integer8((x[index_v] ^ Bit(false)));
	}
	return weight;
}
*/


/* Rippler-carry adder (for bits) */
Bit * add
  (Bit * left_io, 
   const Bit * right_i, unsigned int size)
{
  /* carry flag for current 1-bit addition */
  Bit carry_v;
  Bit *sum=new Bit[size+1];
   for (unsigned index_v=0; index_v < size; ++index_v){
		sum[index_v]=Bit(false);
	}
  for (unsigned index_v=0; index_v < size; ++index_v)
  { /* optimized addition adding only useful bits */
    /* full 1-bit addition taking carry into account */
    Bit xor_v1 = left_io[index_v] ^ carry_v;

    if (index_v < size-1)
    { /* case: must update the carry */
    Bit xor_v2 = right_i[index_v] ^ carry_v;
      carry_v = (xor_v1 & xor_v2) ^ carry_v;
    }

    /* update result of in-place addition */
    sum[index_v] = right_i[index_v] ^ xor_v1;
  }
  return sum;
}



Bit* hammingWeightTest(Bit *x, unsigned int &l, unsigned int size) {
	if (l==1) {
			Bit *r=new Bit[size+1];
			for(unsigned int i=0; i<size; i++){
					r[i]=Bit(false);
			}
			for(unsigned int i=0; i<l; i++) {
					r[i]=x[i]^Bit(false);
			}
			//printf("here stop");
			return r;
	}
	//printf("here %d", size);
	unsigned int m=round(l/2);
	unsigned int n=l-m;
	Bit *left=new Bit[m+1];
	Bit *right=new Bit[n+1];
	for(unsigned int i=0; i<m; i++){
		left[i]=x[i];
	}
	for(unsigned int i=m; i<m+n; i++){
		right[i-m]=x[i];
	}
		return (add(hammingWeightTest(left, m, size), hammingWeightTest(right, n, size), size));
	/*
	Bit *inter=new Bit[l+1];
	for(unsigned int i=0; i<l; i++){
		inter[i]=x[i];
	} 
	
	
	unsigned int le=l;
	while(le != ceil(log2(l))){
			printf("inter \n");
			for(unsigned int i=0; i<le; i++){
			cout<<inter[i];
			} 
			unsigned int m=round(le/2);
			unsigned int n=le-m;
			Bit *left=new Bit[m+1];
			Bit *right=new Bit[n+1];
			printf("left_part ÷\n");
	
			for(unsigned int i=0; i<m; i++){
				left[i]=inter[i];
				cout<<left[i];
			}
			printf("right_part \n");
	
			for(unsigned int i=m; i<m+n; i++){
				right[i-m]=inter[i];
				cout<<right[i-m];
			}
			printf("zeeddkjz");
			
			Bit carry_v;
			for (unsigned index_v=0; index_v < le; ++index_v){
				inter[index_v]=Bit(false);
			}
			for (unsigned index_v=0; index_v < m; ++index_v)
			  { 
				Bit xor_v1 = left[index_v] ^ carry_v;

				if (index_v < m-1)
				{ 
				Bit xor_v2 = right[index_v] ^ carry_v;
				  carry_v = (xor_v1 & xor_v2) ^ carry_v;
				}
				
				inter[index_v] = right[index_v] ^ xor_v1;
			  }
				
			le=ceil(le/2);
			printf("---------------");
	}
	return inter;
	*/
}

Integer8 hammingWeight(Bit *x, unsigned int N){
	if(N==1) {
//		printf("\n n=1");
//		cout<<x[N-1];
//printf("----------");
	//	cout<<Integer8(x[N-1] ^ Bit(false)); 
		return (Integer8(x[N-1] ^ Bit(false)));
	}
	unsigned int m=round(N/2);
	Bit left[m];
	Bit right[N-m];
/*	for (unsigned index_v=0; index_v <m+1; ++index_v) {
			left[index_v]=x[index_v];
			cout<<left[index_v]<<endl;
	}
	printf("\n here");
	cout<<endl;
	for (unsigned index_v=m+1; index_v < N; ++index_v) {
			right[index_v]=x[index_v];
			cout<<right[index_v]<<endl;
	}
	printf("\n here 2");
	*/
	return (hammingWeight(x, m) + hammingWeight(x+m, N-m));
}

/* transform current_value parameter in a binary representation on size bits */
Bit * transform(unsigned int current_value, unsigned int size){
	
  Bit *current=new Bit[size];
  
  for (unsigned index_v=0; index_v < size; ++index_v)
  {/* set all bits at zero */
    current[index_v] = Bit(false);
  }
  
  /* update with the new current value */
  if (current_value)
  { /* case: some bits are not zero */
    for (unsigned index_v=0; index_v < size; ++index_v)
    { /* set initial value of each bit */
      if ((current_value>>index_v) & 1)
      { /* case: bit at current index is set in input value */
        current[index_v] = Bit(true);
      }
    }
  }
	return current;
}

/* optimsed sorting with optimised Hamming weight representation */
void sort_opt(Integer8 *x, Integer8 *y, unsigned int N){
	Bit m[N][N];
	for(unsigned int i=0; i<N; i++) {
			m[i][i]=Bit(false);
			for(unsigned int j=i+1; j<N; j++) {
					m[i][j]=x[i] < x[j];
					m[j][i]=m[i][j] ^ Bit(true);
			}
	}
	
	
	//compute transpose of the matrix
	for(unsigned int i=0; i<N; i++) {
		m[i][i]=Bit(false);
		for(unsigned int j=i+1; j<N; j++) {
					Bit aux=m[i][j];
					m[i][j]=m[j][i];
					m[j][i]=aux;
		}
	}
	
	
/*	for(unsigned int i=0; i<N; i++) {
		for(unsigned int j=0; j<N; j++)
			cout<<m[i][j]<<endl;
		cout<<endl;
	}
	printf("\n hamming weights");
	*/
	//compute Hamming weight
	unsigned int size=ceil(log2(N))+1;
	Bit * hamming_weights[N];
	for(unsigned int i=0; i<N; i++) {
		hamming_weights[i]=hammingWeightTest(m[i],N, size);
		//printf("\n hamming weight");
	//	cout<<hamming_weights[i]<<endl;
	}
	
	
	for(unsigned int i=0; i<N; i++) {
		y[i]=Integer8(0);
		for(unsigned int j=0; j<N; j++) {
				Bit z;
				
				Bit *bi=transform(i, size);
				z=(is_equal(hamming_weights[j], bi, 0,size-1));
				y[i] = y[i] ^ (x[j] & z); 
				/*printf("haming \n");
				for(unsigned int k=0; k< size; k++){
						cout<<hamming_weights[j][k]; 
				}
				printf("binary i \n");
				for(unsigned int k=0; k< size; k++){
						cout<<bi[k]; 
				}
				
				printf("z \n");cout << z <<endl;*/
				
				free(bi);
		}
		
		//cout<<endl;
	}
	//cout<<endl;
	//for(unsigned int i=0; i<N; i++) cout<<y[i]<<endl;
}



void sort(Integer8 *x, Integer8 *y, unsigned int N){
	//compute initial comparison matrix m
	Bit m[N][N];
	for(unsigned int i=0; i<N; i++) {
			m[i][i]=Bit(false);
			for(unsigned int j=i+1; j<N; j++) {
					m[i][j]=x[i] < x[j];
					m[j][i]=m[i][j] ^ Bit(true);
			}
	}
	
	
	//compute transpose of the matrix
	for(unsigned int i=0; i<N; i++) {
		m[i][i]=Bit(false);
		for(unsigned int j=i+1; j<N; j++) {
					Bit aux=m[i][j];
					m[i][j]=m[j][i];
					m[j][i]=aux;
		}
	}
	
	
/*	for(unsigned int i=0; i<N; i++) {
		for(unsigned int j=0; j<N; j++)
			cout<<m[i][j]<<endl;
		cout<<endl;
	}
	printf("\n hamming weights");
	*/
	//compute Hamming weight
	Integer8 hamming_weights[N];
	for(unsigned int i=0; i<N; i++) {
		hamming_weights[i]=hammingWeight(m[i],N);
		//printf("\n hamming weight");
		//cout<<hamming_weights[i]<<endl;
	}
	
	
	for(unsigned int i=0; i<N; i++) {
		y[i]=Integer8(0);
		for(unsigned int j=0; j<N; j++) {
				Bit z;
				z=(Integer8(i)==hamming_weights[j]);
				//cout << z <<endl;
				//cout<<y[i]<<endl;
				
				//cout<<select( (z & Bit(true)), x[j], Integer8(0))<<endl; 
				
				//y[i] = (y[i] ^ select( (z & Bit(true)), x[j], Integer8(0))); 
				y[i] = y[i] ^ (x[j] & z); 
				
				//cout<<y[i] <<endl;
		}
		
		//cout<<endl;
	}
	//cout<<endl;
	//for(unsigned int i=0; i<N; i++) cout<<y[i]<<endl;
}

void test_sorting(){
	// suppose the array to be sorted has a predifined fixed size N
	unsigned int N; //=4;
	cin>>N;
	
	Integer8 x[N];
	for(unsigned int i=0; i<N; i++) {
		cin>>x[i];
	}
	
	Integer8 y[N];
	for(unsigned int i=0; i<N; i++) y[i]=Integer8(0);
	//call function to sort tables 
	sort_opt(x,y,N);
	
	for (unsigned int i = 0 ; i <N ; i++)	{
		cout << y[i];// << endl;
	}

	FINALIZE_CIRCUIT(blif_name);
}

int main()
{
	test_sorting();
}

