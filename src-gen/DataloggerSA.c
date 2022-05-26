
#include "DataloggerSA.h"
#include "..\src\sc_types.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'dataloggerSA'
*/

/* prototypes of all internal functions */
static void enseq_main_region_StateA_default(DataloggerSA* handle);
static void enseq_main_region_default(DataloggerSA* handle);
static void exseq_main_region_StateA(DataloggerSA* handle);
static void exseq_main_region(DataloggerSA* handle);
static void react_main_region_StateA(DataloggerSA* handle);
static void react_main_region__entry_Default(DataloggerSA* handle);
static void clearInEvents(DataloggerSA* handle);
static void clearOutEvents(DataloggerSA* handle);


void dataloggerSA_init(DataloggerSA* handle)
{
		sc_integer i;
	
		for (i = 0; i < DATALOGGERSA_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = DataloggerSA_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
}

void dataloggerSA_enter(DataloggerSA* handle)
{
	/* Default enter sequence for statechart dataloggerSA */
	enseq_main_region_default(handle);
}

void dataloggerSA_runCycle(DataloggerSA* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < DATALOGGERSA_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case DataloggerSA_main_region_StateA:
		{
			react_main_region_StateA(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void dataloggerSA_exit(DataloggerSA* handle)
{
	/* Default exit sequence for statechart dataloggerSA */
	exseq_main_region(handle);
}

sc_boolean dataloggerSA_isActive(const DataloggerSA* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < DATALOGGERSA_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != DataloggerSA_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean dataloggerSA_isFinal(const DataloggerSA* handle)
{
   return bool_false;
}

sc_boolean dataloggerSA_isStateActive(const DataloggerSA* handle, DataloggerSAStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case DataloggerSA_main_region_StateA :
			result = (sc_boolean) (handle->stateConfVector[SCVI_DATALOGGERSA_MAIN_REGION_STATEA] == DataloggerSA_main_region_StateA
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(DataloggerSA* handle)
{
}

static void clearOutEvents(DataloggerSA* handle)
{
}




/* implementations of all internal functions */

/* 'default' enter sequence for state StateA */
static void enseq_main_region_StateA_default(DataloggerSA* handle)
{
	/* 'default' enter sequence for state StateA */
	handle->stateConfVector[0] = DataloggerSA_main_region_StateA;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(DataloggerSA* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state StateA */
static void exseq_main_region_StateA(DataloggerSA* handle)
{
	/* Default exit sequence for state StateA */
	handle->stateConfVector[0] = DataloggerSA_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(DataloggerSA* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of dataloggerSA.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case DataloggerSA_main_region_StateA :
		{
			exseq_main_region_StateA(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state StateA. */
static void react_main_region_StateA(DataloggerSA* handle)
{
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(DataloggerSA* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_StateA_default(handle);
}


