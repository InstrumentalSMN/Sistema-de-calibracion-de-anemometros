
#ifndef DATALOGGERSA_H_
#define DATALOGGERSA_H_

#include "..\src\sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'dataloggerSA'.
*/

#ifndef DATALOGGERSA_EVENTQUEUE_BUFFERSIZE
#define DATALOGGERSA_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define dimension of the state configuration vector for orthogonal states. */
#define DATALOGGERSA_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_DATALOGGERSA_MAIN_REGION_STATEA 0


/*! Enumeration of all states */ 
typedef enum
{
	DataloggerSA_last_state,
	DataloggerSA_main_region_StateA
} DataloggerSAStates;




/*! 
 * Type definition of the data structure for the DataloggerSA state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	DataloggerSAStates stateConfVector[DATALOGGERSA_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
} DataloggerSA;



/*! Initializes the DataloggerSA state machine data structures. Must be called before first usage.*/
extern void dataloggerSA_init(DataloggerSA* handle);

/*! Activates the state machine */
extern void dataloggerSA_enter(DataloggerSA* handle);

/*! Deactivates the state machine */
extern void dataloggerSA_exit(DataloggerSA* handle);

/*! Performs a 'run to completion' step. */
extern void dataloggerSA_runCycle(DataloggerSA* handle);



/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean dataloggerSA_isActive(const DataloggerSA* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean dataloggerSA_isFinal(const DataloggerSA* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean dataloggerSA_isStateActive(const DataloggerSA* handle, DataloggerSAStates state);


#ifdef __cplusplus
}
#endif 

#endif /* DATALOGGERSA_H_ */
