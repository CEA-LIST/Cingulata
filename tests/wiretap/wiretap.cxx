/*
    (C) Copyright 2019 CEA LIST. All Rights Reserved.
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

#include <cstdint>

/* local includes */
#include <bit_exec/tracker.hxx>
#include <ci_context.hxx>
#include <ci_int.hxx>
#include <int_op_gen/mult_depth.hxx>

/* namespaces */
using namespace std;
using namespace cingulata;

#define QUERY_SIZE 10

int main()
{
    CiInt w{CiInt::s32};
    vector<CiInt> q(QUERY_SIZE,CiInt::u32);
	cin>>w;
	for(int i=0;i<QUERY_SIZE;i++)
		cin>>q[i];
	
    CiInt r{0, 4, false};// result of 4 bits in run.sh.in
	for(int i=0;i<QUERY_SIZE;i++)
		r+=(w==q[i])*CiInt(i+1,8,false); 
	r-=1;
	r.write("s");
	CiContext::get_bit_exec_t<BitTracker>()->export_blif(blif_name, "wiretap");
}

