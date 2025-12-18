/**
 * @file    sim.h
 * @brief   SIM state handlers for FSM control and state transition logic
 */
#ifndef _SIM_H_
#define _SIM_H_
#include "sim_cmd.h"

enum simState {
    STATE_RESET,
    STATE_AT_SYNC,
    STATE_SIM_READY,
    STATE_NET_READY,
    STATE_PDP_ACTIVE,        
    STATE_MQTT_START,
    STATE_MQTT_ACCQ,
    STATE_MQTT_CONNECT,
    STATE_MQTT_READY
};

typedef enum simState eSimState;

void updateSimState(eSimResult res, eSimState backState, eSimState nextState);

/**
 * @brief Handle SIM reset state.
 * @return none.
 */
void simResetStatusHandler(void);

/**
 * @brief Handle AT synchronization state.
 * @return none.
 */
void atSyncStatusHandler(void);

/**
 * @brief Handle SIM ready check state.
 * @return none.
 */
void simReadyStatusHandler(void);

/**
 * @brief Handle network registration state.
 * @return none.
 */
void netReadyStatusHandler(void);

/**
 * @brief Handle PDP activation state.
 * @return none.
 */
void pdpActiveStatusHandler(void);

#endif