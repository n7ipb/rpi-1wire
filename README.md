------------------------------------------------------------------------
README 
------------------------------------------------------------------------

        Simple programs to identify and read 1-wire temperature probes such as
        the DS18B20.  The results can then be used by SNMPD, SvxLink and 
        other programs.

        These are used by the RPi based SvxLink repeater controllers on 
        the Pacific Northwest 220Mhz Repeater Network.  http://pnw220.net

### Programs
------------------------------------------------------------------------

read_probes   

        Identifies and cycles through attached probes printing out the 
        probe ID and temperature.
        
        Use this to identify probes by heating/cooling each one and 
        labeling as desired.  
        
        Then edit the link_probes script with the resulting information
        and your choice of symbolic names. 
        
        See link_probes for examples from a typical installation

read_probe `<symbolicname>`

        Returns the temperature in degrees F for the named probe. 
	Returns a list of available names when no name is specified

link_probes

        When edited to contain the ID/name pairs for attached probes
        this script can be run to create symbolic links in /tmp/temps 
        for use by 'read_probe'
        
        run as './link_probes'

### Build and Install
------------------------------------------------------------------------

To build:

        make        
        sudo make install

To remove:

        sudo make uninstall

### Configure probe names
------------------------------------------------------------------------

Run 'read_probes' to obtain the probe ID and identify each probe.

	The easiest way to do this is run the program and heat or cool
	each probe while monitoring the output.  Write down the ID for
	each and label the probes.

	Then edit 'link_probes' and add lines for each.  Examples from our
	Lyman 224.78 machine are commented out in the script. 

Then install and configure the link_probes.service

	sudo make link_probes
	sudo systemctl enable link_probes.service

Reboot or manually configure with:

	sudo systemctl start link_probes

### Test
------------------------------------------------------------------------	

Test your installation

	read_probe `<symbolicname>`
	
	Example: 
	$ read_probe transmitter
	90.5
	$
