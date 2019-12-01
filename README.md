# showconsole

The package showconsole includes several helpers which had been developed
over the years for SUSE and openSUSE Linux. There are

  * blogd       - Does boot logging on /dev/console
  * blogctl     - Control the boot logging daemon blogd
  * blogger     - Writes messages to a running blogd process
  * isserial    - Determines if the underlying tty of stdin is a serial line
  * showconsole - Determines the real tty of stdin
  * setconsole  - Redirect system console output to a tty

Whereas blogd had been written for SysVinit based systems this newer version
also supports systemd based system.  The blogd together with the new tool
blogctl can replace the well known plymouth but without any frame buffer and
splash support.  Therefore blogd may used on workstations as well as on so
called big irons.

The blogd causes systemd to show its boot messages on the system console to
collect them for repeating them on all devices used for the system console
as well as it writes out a copy to boot.log and boot.old below /var/log/.

Beside boot messages and logging them the blogd can handle password or
passphrase requests send by password agents.  For those requests the blogd
do ask on all terminal lines used for the system console.
