# Asus DNSMasq surrogate
## About

Purpose of this tool is to provide a bit more functionality for Asus routers
that either don't have Merlin support (hello, GT-AC5300), or are not supported
by Merlin any more.

This tool does not require root-jailing, installation of any other third-party
software etc. As long as your Asus can install its own utilities (eg. time
machine or download master), you're good to go.

## Building
### Requirements

To build this for your router, I recommend installing a debian chroot on
your router and using up-to-date tools. One other option is to use
cross-platform toolchain on your PC.

Minimum requirements for building the tool are:

  - gcc version 7.2.0,
  - original or stub version of `libnvram.so`

if you chose to build with original `/lib/libnvram.so`, you will also need
to copy `/lib/libwlcsm.so` and `/usr/lib/libshared.so`.

### Compiling

This is the only part that requires a bit of an effort. Supplied build files
work well with modern build tools for linux. Assuming your tools meet
requirements, a single `make` execution should suffice

## Usage
### Installation

Before the tool can be deployed to your router, pen drive must be configured.
It is easiest done the router itself:

  - plug in pen drive to your router's USB port,
  - navigate to `USB Applications`,
  - install `Download Master`.

The last step will initialize SD card for you.
Once this step is done, copy:

  - `dnsmasq-surrogate` to `/opt/bin`,
  - `dnsmasq-surrogate.control` to `/opt/lib/ipkg/info/`,
  - `dnsmasq-surrogate.rc` to `/opt/etc/init.d/S50dnsmasq-surrogate`.
  - give execute permissions to the rc file
    (`chmod a+x /opt/etc/init.d/S50dnsmasq-surrogate`)
  - restart your router.

Or execute `make dist` and extract the `dnsmasq-surrogate.tgz` to `/opt` folder
on your router.

### Giving hosts names

Proceed to your Asus' control panel and use static dhcp assignment to configure
IP addresses and host names.

This tool does not automaticaly assign names to dynamically configured hosts
yet. There is nothing else you'd need to do.

### Using additional hosts / ad-block functionality

Surrogate can feed dnsmasq with any additional `hosts` files, thus providing a
functionality similar to adblock.

To enable this functionality, once surrogate is installed, copy any additional
hosts files to `/jffs/dnsmasq-surrogate/hosts/` folder. These files can be
acquired from multiple sites, eg. [StevenBlack's github
repo](https://github.com/StevenBlack/hosts).

Once files are copied to `/jffs/dnsmasq-surrogate/hosts/` folder, you will need
to manually restart dnsmasq service - or reboot your router.

```
# service restart_dnsmas
```

### Disabling and re-enabling utility

To disable utility, log in to your router as admin and execute:

```
app_set_enabled.sh dnsmasq-surrogate no
```

To re-enable utility, execute:

```
app_set_enabled.sh dnsmasq-surrogate yes
```

To query utility state:

```
app_get_field.sh dnsmasq-surrogate Enabled
```

After each enable/disable cycle, application will re-start dns service on your
router. This does not mean, however, that all the hosts will automatically start
working at instant: you may need to re-connect to your network or flush dns
cache using other methods.
