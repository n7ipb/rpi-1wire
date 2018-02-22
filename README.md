# rpi-1wire
------------------------------------------------------------------------
README 
------------------------------------------------------------------------

        Simple programs to identify and read 1-wire temperature probes such as
        the DS18B20.  The results can then be used by SNMPD, SvxLink and 
        other programs.

        These are used by the RPi based SvxLink repeater controllers on 
        the Pacific Northwest Repeater Network.  http://pnw220.wetnet.net

# Programs
------------------------------------------------------------------------

read_probes   

        Identifies and cycles through attached probes
        and prints the probe ID and temperature.
        Use this to identify probes by heating/cooling
        each one and labeling as desired.  Then
        edit link_probes with the resulting information
        and your choice of symbolic names. See the
        provided file for examples from a typical
        installation

read_probe <symbolicname>  

        Returns the temperature in degrees F for the named probe. 

link_probes

        When edited to contain the ID/name pairs for 
        attached probes this script can be run to create
        symbolic links in /tmp/temps and is used by 'read_probe'
        
        run as './link_probes'

# Build and Install
------------------------------------------------------------------------

To build:

        make        
        sudo make install

To remove:

        sudo make uninstall
