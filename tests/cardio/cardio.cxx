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

#define SEX_FIELD 0
#define ANTECEDENT_FIELD 1
#define SMOKER_FIELD 2
#define DIABETES_FIELD 3
#define PRESSURE_FIELD 4

int main()
{
	Bit flags[8];

	for (int i = 0; i < 8; i++)
		cin >> flags[8-i-1];

	Integer8 age, hdl, height=2, weight, physical_act, drinking;

	cin >> age;
	cin >> hdl;
	cin >> height;
	cin >> weight;
	cin >> physical_act;
	cin >> drinking;

	Integer8 keystream[7];
	// Read the pre-calculated keystream.
	for(int i=0;i<7;i++)
			cin>>keystream[i];

	for(int i=0;i<8;i++)
		flags[i] ^= keystream[0][i];
	age ^= keystream[1];
	hdl ^= keystream[2];
	height ^= keystream[3];
	weight ^= keystream[4];
	physical_act ^= keystream[5];
	drinking ^= keystream[6];


	Integer8 risk = 0;	

	risk = risk + select((flags[SEX_FIELD]) && (age > Integer8(50)), Integer8(1), Integer8(0));

	risk = risk + select((!flags[SEX_FIELD]) && (age > Integer8(60)), Integer8(1), Integer8(0));

	risk = risk + Integer8(flags[ANTECEDENT_FIELD]);
	risk = risk + Integer8(flags[SMOKER_FIELD]);
	risk = risk + Integer8(flags[DIABETES_FIELD]);
	risk = risk + Integer8(flags[PRESSURE_FIELD]);
	
	risk = risk + select(hdl < Integer8(40), Integer8(1), Integer8(0));
		
	//Integer8 riskH = 0;
	//Integer8 sum = 2; 
	//sum = height + Integer8(10);
	//riskH = select( weight > sum, Integer8(1), Integer8(0));
	
	risk = risk + select(weight - Integer8(10) > height, Integer8(1), Integer8(0));	
	
	risk = risk + select(physical_act < Integer8(30), Integer8(1), Integer8(0));
	
	risk = risk + select((flags[SEX_FIELD]) && (drinking > Integer8(3)), Integer8(1), Integer8(0));
	risk = risk + select((!flags[SEX_FIELD]) && (drinking > Integer8(2)), Integer8(1), Integer8(0));
	
	
	for (int i = 3; i >= 0; --i)	{
		cout << risk[i];
	}
	
	FINALIZE_CIRCUIT("cardio.blif");
}

