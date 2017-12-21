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


/* local includes */
#include <integer.hxx>

/* namespaces */
using namespace std;

#define OUT_BITCNT 46 //depth 12

void readKeyAndRunKreyvium(void)
{

	Bit key[128],iv[128],s[288];

	Bit RKEY[128],RIV[128]; // The two additional LFSR.
	
	for(int i=0;i<128;i++)
		cin>>key[i];
	for(int i=0;i<128;i++)
		cin>>iv[i];
		
	// Internal state setup.
	for(int i=0;i<288;i++)
		s[i]=true;
	// Insert the first 93 key bits @ position 0 (1 in the paper).
	for(int i=0;i<93;i++)
		s[i]=key[i];
	// Insert the first 84 iv bits @ position 93 (94 in the paper).
	for(int i=0;i<84;i++)
		s[i+93]=iv[i];
	// Insert the remaining iv bits @ position 177 (178 in the paper).
	for(int i=0;i<44;i++)
		s[i+177]=iv[i+84];
	// Set pos 287 to 0 (288 in the paper).	
	s[287]=false;
	
	// RKEY register set up.
	for(int i=0;i<128;i++)
		RKEY[i]=key[128-i-1];
	// RIV register setup.
	for(int i=0;i<128;i++)
		RIV[i]=iv[128-i-1];

	// Internal state warm up.
	for(int i=0;i<4*288;i++)
	{
		Bit t1=s[65]+s[90]*s[91]+s[92]+s[170]+RIV[127-0]; // t1 = s66+s91*s92+s93+s171+RIV[0] (paper).
		Bit t2=s[161]+s[174]*s[175]+s[176]+s[263]; // t2 = s162+s175*s176+s177+s264 (paper).
		Bit t3=s[242]+s[285]*s[286]+s[287]+s[68]+RKEY[127-0]; // t3 = s243+s286*s287+s288+s69+RKEY[0] (paper).
		//Bit t4=RKEY[127-0]+RKEY[127-51]+RKEY[127-93]+RKEY[127-117];
		//Bit t5=RIV[127-0]+RIV[127-121]+RIV[127-126]+RIV[127-127];
		Bit t4=RKEY[127-0];
		Bit t5=RIV[127-0];
		// Shift the internal state from 1 to the right.
		for(int j=0;j<287;j++)
			s[288-j-1]=s[288-j-2];
		// Then inject the ti's.
		s[0]=t3; // s1 = t3 (paper).
		s[93]=t1; // s94 = t1 (paper).
		s[177]=t2; // s178 = t2 (paper).
		// Shift RKEY and RIV from 1 to the right.
		for(int j=0;j<127;j++)
		{
			RKEY[128-j-1]=RKEY[128-j-2];
			RIV[128-j-1]=RIV[128-j-2];
		}
		// Then injects t4 and t5.
		RKEY[0]=t4;
		RIV[0]=t5;
	}
	
	// Keystream generation.
	for(int i=0;i<OUT_BITCNT;i++)
	{
		Bit t1=s[65]+s[92]; // t1 = s66+s93 (paper).
		Bit t2=s[161]+s[176]; // t2 = s162+s177 (paper).
		Bit t3=s[242]+s[287]+RKEY[127-0]; // t3 = s243+s288+RKEY[0] (paper).
		Bit z=t1+t2+t3; // zi = t1+t2+t3 (paper).
		t1=t1+s[90]*s[91]+s[170]+RIV[127-0]; // t1 = t1+s91*s92+s171 (paper).
		t2=t2+s[174]*s[175]+s[263]; // t2 = t2+s175*s176+s264 (paper).
		t3=t3+s[285]*s[286]+s[68]; // t3 = t3+s286*s287+s69 (paper).
		//Bit t4=RKEY[127-0]+RKEY[127-51]+RKEY[127-93]+RKEY[127-117];
		//Bit t5=RIV[127-0]+RIV[127-121]+RIV[127-126]+RIV[127-127];
		Bit t4=RKEY[127-0];
		Bit t5=RIV[127-0];
		// Shift the internal state from 1 to the right.
		for(int j=0;j<287;j++)
			s[288-j-1]=s[288-j-2];
		// Then inject the ti's.
		s[0]=t3; // s1 = t3 (paper).
		s[93]=t1; // s94 = t1 (paper).
		s[177]=t2; // s178 = t2 (paper).	
		// Shift RKEY and RIV from 1 to the right.
		for(int j=0;j<127;j++)
		{
			RKEY[128-j-1]=RKEY[128-j-2];
			RIV[128-j-1]=RIV[128-j-2];
		}
		// Then injects t4 and t5.
		RKEY[0]=t4;
		RIV[0]=t5;

		cout<<z;
	}
}



int main() 
{
  readKeyAndRunKreyvium();

  FINALIZE_CIRCUIT("kreyvium.blif");  
}

