
#include "Sa.h"
#include "..\src\sc_types.h"
#include "SaRequired.h"

#include <stdlib.h>
#include <string.h>
/*! \file Implementation of the state machine 'sa'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_sa_tr0_tr0(const Sa* handle);
static void effect_main_region_sa_tr0(Sa* handle);
static void enact_main_region_sa(Sa* handle);
static void enseq_main_region_sa_default(Sa* handle);
static void enseq_main_region_default(Sa* handle);
static void exseq_main_region_sa(Sa* handle);
static void exseq_main_region(Sa* handle);
static void react_main_region_sa(Sa* handle);
static void react_main_region__entry_Default(Sa* handle);
static void clearInEvents(Sa* handle);
static void clearOutEvents(Sa* handle);

const sc_integer SA_SAIFACE_LED3 = 5;

void sa_init(Sa* handle)
{
		sc_integer i;
	
		for (i = 0; i < SA_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Sa_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
}

void sa_enter(Sa* handle)
{
	/* Default enter sequence for statechart sa */
	enseq_main_region_default(handle);
}

void sa_runCycle(Sa* handle)
{
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < SA_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Sa_main_region_sa:
		{
			react_main_region_sa(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}

void sa_exit(Sa* handle)
{
	/* Default exit sequence for statechart sa */
	exseq_main_region(handle);
}

sc_boolean sa_isActive(const Sa* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < SA_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Sa_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean sa_isFinal(const Sa* handle)
{
   return bool_false;
}

sc_boolean sa_isStateActive(const Sa* handle, SaStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Sa_main_region_sa :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SA_MAIN_REGION_SA] == Sa_main_region_sa
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

static void clearInEvents(Sa* handle)
{
	handle->iface.evTick_raised = bool_false;
}

static void clearOutEvents(Sa* handle)
{
}

void saIface_raise_evTick(Sa* handle)
{
	handle->iface.evTick_raised = bool_true;
}


const sc_integer saIface_get_lED3(const Sa* handle)
{
	return SA_SAIFACE_LED3;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_sa_tr0_tr0(const Sa* handle)
{
	return handle->iface.evTick_raised;
}

static void effect_main_region_sa_tr0(Sa* handle)
{
	exseq_main_region_sa(handle);
	enseq_main_region_sa_default(handle);
}

/* Entry action for state 'sa'. */
static void enact_main_region_sa(Sa* handle)
{
	/* Entry action for state 'sa'. */
	saIface_opLED(handle, SA_SAIFACE_LED3);
}

/* 'default' enter sequence for state sa */
static void enseq_main_region_sa_default(Sa* handle)
{
	/* 'default' enter sequence for state sa */
	enact_main_region_sa(handle);
	handle->stateConfVector[0] = Sa_main_region_sa;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Sa* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state sa */
static void exseq_main_region_sa(Sa* handle)
{
	/* Default exit sequence for state sa */
	handle->stateConfVector[0] = Sa_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(Sa* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of sa.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Sa_main_region_sa :
		{
			exseq_main_region_sa(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state sa. */
static void react_main_region_sa(Sa* handle)
{
	/* The reactions of state sa. */
	if (check_main_region_sa_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_sa_tr0(handle);
	} 
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Sa* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_sa_default(handle);
}


