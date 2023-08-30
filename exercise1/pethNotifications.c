/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.notify.conf
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "pethNotifications.h"

extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

int send_pethPsePortOnOffNotification_trap(int grpid, int portid, int status)
{
    netsnmp_variable_list *var_list = NULL;
    const oid pethPsePortOnOffNotification_oid[] = {1, 3, 6, 1, 2, 1, 105, 0, 1};
    const oid pethPsePortDetectionStatus_oid[] = {1, 3, 6, 1, 2, 1, 105, 1, 1, 1, 6,
                                                  /* insert index here */};
    const oid pethPsePortGroupIndex_oid[] = {1, 3, 6, 1, 2, 1, 105, 1, 1, 1, 1,
                                             /* insert index here */};
    const oid pethPsePortIndex_oid[] = {1, 3, 6, 1, 2, 1, 105, 1, 1, 1, 2,
                                        /* insert index here */};

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list, snmptrap_oid, snmptrap_oid_len, ASN_OBJECT_ID, pethPsePortOnOffNotification_oid, sizeof(pethPsePortOnOffNotification_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list, pethPsePortDetectionStatus_oid, OID_LENGTH(pethPsePortDetectionStatus_oid), ASN_INTEGER, NULL, status);

    /*
     * Add any extra (optional) objects here
     */
    snmp_varlist_add_variable(&var_list, pethPsePortGroupIndex_oid, OID_LENGTH(pethPsePortGroupIndex_oid), ASN_INTEGER, NULL, grpid);
    snmp_varlist_add_variable(&var_list, pethPsePortIndex_oid, OID_LENGTH(pethPsePortIndex_oid), ASN_INTEGER, NULL, portid);

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}
int send_pethMainPowerUsageOnNotification_trap(void)
{
    netsnmp_variable_list *var_list = NULL;
    const oid pethMainPowerUsageOnNotification_oid[] = {1, 3, 6, 1, 2, 1, 105, 0, 2};
    const oid pethMainPseConsumptionPower_oid[] = {1, 3, 6, 1, 2, 1, 105, 1, 3, 1, 1, 4,
                                                   /* insert index here */};

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list, snmptrap_oid, snmptrap_oid_len, ASN_OBJECT_ID, pethMainPowerUsageOnNotification_oid, sizeof(pethMainPowerUsageOnNotification_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list, pethMainPseConsumptionPower_oid, OID_LENGTH(pethMainPseConsumptionPower_oid), ASN_GAUGE,
                              /* Set an appropriate value for pethMainPseConsumptionPower */
                              NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}
int send_pethMainPowerUsageOffNotification_trap(void)
{
    netsnmp_variable_list *var_list = NULL;
    const oid pethMainPowerUsageOffNotification_oid[] = {1, 3, 6, 1, 2, 1, 105, 0, 3};
    const oid pethMainPseConsumptionPower_oid[] = {1, 3, 6, 1, 2, 1, 105, 1, 3, 1, 1, 4,
                                                   /* insert index here */};

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list, snmptrap_oid, snmptrap_oid_len, ASN_OBJECT_ID, pethMainPowerUsageOffNotification_oid, sizeof(pethMainPowerUsageOffNotification_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list, pethMainPseConsumptionPower_oid, OID_LENGTH(pethMainPseConsumptionPower_oid), ASN_GAUGE,
                              /* Set an appropriate value for pethMainPseConsumptionPower */
                              NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}
