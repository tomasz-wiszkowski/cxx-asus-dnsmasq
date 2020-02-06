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

- arm-linux-gnueabi-gcc (8 or 9)
- cmake and ccmake

Simply invoke `cmake .` followed by `ccmake .`. `BUILD_DIST` controls whether
the target version (for device) will be built; when OFF, build defaults to
(host-side) unit testing.

### Compiling

- use `make all install dist` to build target package (assumes `BUILD_DIST=on`).
- use `make all test` to build and run tests (assumes `BUILD_DIST=off`).

## Usage

### Before installing

Before the tool can be deployed to your router, pen drive must be configured.
It is easiest done the router itself:

  - plug in pen drive to your router's USB port,
  - navigate to `USB Applications`,
  - install `Download Master`.

The last step will initialize SD card for you.

### Basic installation

  - Open
    [`Releases`](https://github.com/tomasz-wiszkowski/asus-dnsmasq/releases)
    tab here on Github and download archive for your router,
  - copy archive to your router (eg. using pendrive or `scp`)
  - extract contents of this archive on your router to /opt folder:

    ```
    cd /opt
    tar -zxf /path/to/dnsmasq-surrogate-*.tgz .
    ```

  - restart your router.

### Expert installation / building from sources

  - install armbian, debian or package manager on your router. Your package
    manager must provide decently modern gcc or clang and basic build tools.
  - execute `make dist` in the source folder. This will produce all artifacts
    and package them to a tgz file.
  - decompress the package to an `/opt` folder:

    ```
    cd /opt
    tar -zxf /path/to/dnsmasq-surrogate-*.tgz .
    ```

  - activate surrogate and restart dnsmasq:

    ```
    dnsmasq-surrogate install
    service restart_dnsmasq
    ```

By decompressing archive, you effectively:

  - copy `dnsmasq-surrogate` to `/opt/bin`,
  - copy `dnsmasq-surrogate.control` to `/opt/lib/ipkg/info/`,
  - copy `dnsmasq-surrogate.rc` to `/opt/etc/init.d/S50dnsmasq-surrogate`.
  - give execute permissions to the rc file
    (`chmod a+x /opt/etc/init.d/S50dnsmasq-surrogate`)

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
# service restart_dnsmasq
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
