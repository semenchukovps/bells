
include $(TOPDIR)/rules.mk

PKG_NAME:=bells_system
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Bells System
  DEPENDS:=+libstdcpp +librt
endef

define Package/bells_system/description
 This package includes utils to make automatic bells through GPIO on Raspberry Pi
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure

endef

define Package/bells_system/install
	mkdir -p $(1)/bin/
	$(INSTALL_DIR) $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/$(PKG_NAME) $(1)/etc/init.d/$(PKG_NAME)
endef

$(eval $(call BuildPackage,bells_system))
