/* ot_config.c - KNoT OpenThread Credentials Handler */

/*
 * Copyright (c) 2019, CESAR. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include <net/net_if.h>
#include <net/openthread.h>
#include <openthread/thread.h>
#include <openthread/link.h>
// #include <settings/settings_ot.h>

#include "ot_config.h"

#define NET_NAME_LEN 17
#define XPANID_LEN 24
#define MASTERKEY_LEN 48

LOG_MODULE_REGISTER(otconfig, LOG_LEVEL_DBG);

static int set_ot_credentials(const char *net_name, const u8_t channel,
			 const u16_t panid, const char *xpanid,
			 const char *masterkey)
{
	struct net_if *iface = net_if_get_default();
	struct openthread_context *ot_context = net_if_l2_data(iface);
	int rc;

	otExtendedPanId xpanid_ctx;
	otMasterKey masterkey_ctx;

	net_bytes_from_str(xpanid_ctx.m8, 8, (char *)xpanid);
	net_bytes_from_str(masterkey_ctx.m8, 16, (char *)masterkey);

	LOG_DBG("Setting OpenThread credentials");

	rc = otThreadSetEnabled(ot_context->instance, false);
	if (rc) {
		LOG_ERR("Failed to stop Thread protocol. (err %d)", rc);
		return rc;
	}
	rc = otIp6SetEnabled(ot_context->instance, false);
	if (rc) {
		LOG_ERR("Failed to disable IPv6 communication. (err %d)", rc);
		return rc;
	}

	rc = otThreadSetNetworkName(ot_context->instance, net_name);
	if (rc) {
		LOG_ERR("Failed to configure net_name. (err %d)", rc);
		return rc;
	}
	rc = otLinkSetChannel(ot_context->instance, channel);
	if (rc) {
		LOG_ERR("Failed to configure channel. (err %d)", rc);
		return rc;
	}
	rc = otLinkSetPanId(ot_context->instance, panid);
	if (rc) {
		LOG_ERR("Failed to configure panid. (err %d)", rc);
		return rc;
	}
	rc = otThreadSetExtendedPanId(ot_context->instance, &xpanid_ctx);
	if (rc) {
		LOG_ERR("Failed to configure xpanid. (err %d)", rc);
		return rc;
	}
	rc = otThreadSetMasterKey(ot_context->instance, &masterkey_ctx);
	if (rc) {
		LOG_ERR("Failed to configure masterkey. (err %d)", rc);
		return rc;
	}

	rc = otIp6SetEnabled(ot_context->instance, true);
	if (rc) {
		LOG_ERR("Failed to enable IPv6 communication. (err %d)", rc);
		return rc;
	}
	rc = otThreadSetEnabled(ot_context->instance, true);
	if (rc) {
		LOG_ERR("Failed to start Thread protocol. (err %d)", rc);
		return rc;
	}

	return OT_ERROR_NONE;
}

int ot_config_update(void)
{
	u16_t panid = 4369;			// PAN Id
	u8_t channel= 20;			// Channel
	char net_name[NET_NAME_LEN] = "knot1";	// Network name
	char xpanid[XPANID_LEN] = "11:11:11:11:22:22:22:22";	// Expanded PAN Id
	char masterkey[MASTERKEY_LEN] = "00:11:22:33:44:55:66:77:88:99:aa:bb:cc:dd:ee:ff";	// Master key
	int rc;

	LOG_DBG("Reading OpenThread settings");

	// rc = settings_ot_init();
	// if (rc) {
	// 	LOG_ERR("Failed to init OpenThread settings. (err %d)", rc);
	// 	return rc;
	// }

	// rc = settings_ot_read(SETTINGS_OT_PANID, &panid);
	// if (rc != sizeof(panid)) {
	// 	LOG_ERR("Failed to read PANID from settings. (err %d)", rc);
	// 	return rc;
	// }
	// rc = settings_ot_read(SETTINGS_OT_CHANNEL, &channel);
	// if (rc != sizeof(channel)) {
	// 	LOG_ERR("Failed to read CHANNEL from settings. (err %d)", rc);
	// 	return rc;
	// }
	// rc = settings_ot_read(SETTINGS_OT_NET_NAME, net_name);
	// if (rc != sizeof(net_name)) {
	// 	LOG_ERR("Failed to read NET_NAME from settings. (err %d)", rc);
	// 	return rc;
	// }
	// rc = settings_ot_read(SETTINGS_OT_XPANID, xpanid);
	// if (rc != sizeof(xpanid)) {
	// 	LOG_ERR("Failed to read XPANID from settings. (err %d)", rc);
	// 	return rc;
	// }
	// rc = settings_ot_read(SETTINGS_OT_MASTERKEY, masterkey);
	// if (rc != sizeof(masterkey)) {
	// 	LOG_ERR("Failed to read MASTERKEY from settings. (err %d)", rc);
	// 	return rc;
	// }

	printk("***** panid: %d *****\n", panid);
	printk("***** channel: %d *****\n", channel);
	printk("***** net_name: %s *****\n", net_name);
	printk("***** xpanid: %s *****\n", xpanid);
	printk("***** masterkey: %s *****\n", masterkey);
	rc = set_ot_credentials(net_name, channel, panid, xpanid, masterkey);

	return rc;
}
