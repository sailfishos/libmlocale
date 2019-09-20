This source tree contains the MLocale library

# Building

```
./configure
make
sudo make install
```

The library will be installed by default with a prefix of /usr/local,
see ./configure -help for options.

The reference documentation can be built with ```make doc```,
if doxygen is available.

## Building with Sailfish SDK's QtCreator

With the default SDK setup the permissions of the files in the SDK build engine's /home/src1 folder will loose the
executable permissions. This will make the compiling of the project with QtCreator fail with a similar error:


```
RPM build errors:
/var/tmp/rpm-tmp.L4oyur: line 41: ./configure: Permission denied
error: Bad exit status from /var/tmp/rpm-tmp.L4oyur (%build)
    Bad exit status from /var/tmp/rpm-tmp.L4oyur (%build)
```

To workaround this you can override the FMODE_HOME_MERSDK variable in the /etc/mersdk.env.systemd config file,
or you can drop the fmode and dmode mount options in /lib/systemd/system/home-mersdk.service.

## Build dependencies
- Qt development headers
- pkg-config

## Optional dependencies

If found, the following libraries will enable additional
features in the MLocale library:

- ICU: I18N and L10N extensions



