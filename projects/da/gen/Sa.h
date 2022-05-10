
#ifndef SA_H_
#define SA_H_

#include "..\src\sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'sa'.
*/

/*! Define dimension of the state configuration vector for orthogonal states. */
#define SA_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_SA_MAIN_REGION_SA 0

/*! Enumeration of all states */ 
typedef enum
{
	Sa_last_state,
	Sa_main_region_sa
} SaStates;

/*! Type definition of the data structure for the SaIface interface scope. */
typedef struct
{
	sc_boolean evTick_raised;
} SaIface;


/* Declaration of constants for scope SaIface. */
extern const sc_integer SA_SAIFACE_LED3;





/*! 
 * Type definition of the data structure for the Sa state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	SaStates stateConfVector[SA_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	SaIface iface;
} Sa;



/*! Initializes the Sa state machine data structures. Must be called before first usage.*/
extern void sa_init(Sa* handle);

/*! Activates the state machine */
extern void sa_enter(Sa* handle);

/*! Deactivates the state machine */
extern void sa_exit(Sa* handle);

/*! Performs a 'run to completion' step. */
extern void sa_runCycle(Sa* handle);


/*! Raises the in event 'evTick' that is defined in the default interface scope. */ 
extern void saIface_raise_evTick(Sa* handle);

/*! Gets the value of the variable 'LED3' that is defined in the default interface scope. */ 
extern const sc_integer saIface_get_lED3(const Sa* handle);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean sa_isActive(const Sa* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean sa_isFinal(const Sa* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean sa_isStateActive(const Sa* handle, SaStates state);


#ifdef __cplusplus
}
#endif 

#endif /* SA_H_ */
