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

#include <iostream>
#include <fstream>

#include <integer.hxx>

/* namespaces */
using namespace std;

#define QUERY_SIZE 10

int main()
{
	Integer32 w,q[QUERY_SIZE];

	cin>>w;
	for(int i=0;i<QUERY_SIZE;i++)
		cin>>q[i];

	Integer8 r=0;
	for(int i=0;i<QUERY_SIZE;i++)
		r+=(w==q[i])*Integer8(i+1);
	r-=1;
	r&=0xf;

	cout<<r;

	FINALIZE_CIRCUIT(blif_name);
}

