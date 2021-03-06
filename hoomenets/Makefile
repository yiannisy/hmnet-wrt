# 
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile $

include $(TOPDIR)/rules.mk

PKG_NAME:=hoomenets
PKG_RELEASE:=0.1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/hoomenets/Default
	SECTION:=net
	CATEGORY:=Network
	URL:=http://www.openflowswitch.org/
endef

define Packet/hoomenets/Default/description
	This package contains basic utilites \\\
	for the Stanford HomeNets project.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Package/hoomenets
  $(call Package/hoomenets/Default)
  TITLE:=Stanford HomeNets Utilities.
endef

define Packet/hoomenets/description
	This package contains basic utilites \\\
	for the Stanford HomeNets project.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -std=c99" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		LDFLAGS_MODULES="$(TARGET_LDFLAGS)" \
		PRECOMPILED_FILTER=1 \
		STAGING_DIR="$(STAGING_DIR)" \
		DESTDIR="$(PKG_INSTALL_DIR)/usr" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		PATH="$(TARGET_PATH)" \
		KCC="$(KERNEL_CC)"
endef

define Package/hoomenets/install
	$(INSTALL_DIR) $(1)/etc/init.d $(1)/etc/config
	$(INSTALL_BIN) ./files/etc/hmnet_version $(1)/etc/
	$(INSTALL_BIN) ./files/etc/init.d/hmnet $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/etc/config/homenets $(1)/etc/config

	$(INSTALL_DIR) $(1)/lib/hmnet
	$(INSTALL_BIN) ./files/lib/hmnet/get_of_controller.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_of_controller.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_conf_controller.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_conf_controller.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_of_dpid.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_mac_address.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_mac_address.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_sw_version.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_sw_version.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_cpu_usage.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_active_flows.sh $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan0_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan0_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan0_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan0_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan1_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan1_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan1_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan1_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan2_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan2_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan2_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/set_wlan2_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan0_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan0_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan0_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan0_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan1_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan1_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan1_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan1_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan2_ssid.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan2_hidden.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan2_encr.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/get_wlan2_key.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/commit_homenets.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/commit_openflow.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/commit_wireless.lua $(1)/lib/hmnet/

	$(INSTALL_BIN) ./files/lib/hmnet/get_bitrates.sh $(1)/lib/hmnet/

	$(INSTALL_BIN) ./files/lib/hmnet/uci_set_snmp.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/uci_get_snmp.lua $(1)/lib/hmnet/
	$(INSTALL_BIN) ./files/lib/hmnet/uci_commit_snmp.lua $(1)/lib/hmnet/

	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) ./files/sbin/hmnetup $(1)/sbin/
	$(INSTALL_BIN) ./files/sbin/hmnetdown $(1)/sbin/

	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/snmp_tunnel $(1)/usr/bin/
endef

$(eval $(call BuildPackage,hoomenets))
