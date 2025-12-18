/**
 * @file    sim.c
 * @brief   SIM state handlers for FSM control and state transition logic
 */
#include <unistd.h>
#include "sys/log.h"
#include "sim_cmd.h"
#include "sim.h"
#include "transport/mqtt.h"

eSimState preState = STATE_RESET;
eSimState simState = STATE_RESET;

void updateSimState(eSimResult res, eSimState backState, eSimState nextState)
{
    if (res == FAIL) {
        simState = preState;
        preState = backState;
    } else if (res == PASS) {
        preState = simState;
        simState = nextState;
    } else {
        sleep(1);
    }
}

void simResetStatusHandler(void)
{
    while (1) {
        if (simCheckAlive() == PASS)
            break;
        sleep(1);
    }

    mqttDisconnectHandler();

    mqttReleaseClientHandler();

    updateSimState(PASS, STATE_RESET, STATE_AT_SYNC);
} 

void atSyncStatusHandler(void)
{
    eSimResult res = PASS;
    if (simCheckAlive() == FAIL || simEchoOff() == FAIL) {
        res = FAIL;
    }

    updateSimState(res, STATE_RESET, STATE_SIM_READY);
}

void simReadyStatusHandler(void)
{
    eSimResult res = simCheckReady();
    updateSimState(res, STATE_RESET, STATE_NET_READY);
}

void netReadyStatusHandler(void)
{
    eSimResult res = simCheckRegEps();
    updateSimState(res, STATE_AT_SYNC, STATE_PDP_ACTIVE);
}

void pdpActiveStatusHandler(void)
{
    eSimResult res = simSetPdpContext();
    if (res != PASS)    
        goto end;

    res = simAttachGprs();
    if (res != PASS)
        goto end;

    res = simActivatePdp();

end:
    updateSimState(res, STATE_SIM_READY, STATE_MQTT_START);
}
