LUFSMeter
=========

A VST and AudioUnit plugin (as well as a standalone application for testing) to measure the loudness,
according to ITU BS.1770 and EBU R128.

These sources correspond to the LUFSMeter version 0.5.1. The source code of newer versions
are not public anymore.
But if any issues in the measurement algorithm are discovered, they will be fixed here as well.

Official website: http://www.lufsmeter.com


The excellent JUCE framework (http://www.juce.com) is used extensively (included here).


Getting started
---------------

Compile the standalone version for your platform first. You find a Makefile, a Visualstudio project as well as an XCode project in `projects/LUFSMeterStandalone/Builds/ <projects/LUFSMeterStandalone/Builds/>`_.
In the same vein go to `projects/LUFSMeter/Builds/ <projects/LUFSMeter/Builds/>`_ to start building the plugin version.

The main sources are in `projects/LUFSMeter/Source/ <projects/LUFSMeter/Source/>`_.

Additional help is provided in `docs/ <docs/>`_.
Feel free to `contact me <http://www.klangfreund.com/about/contact/>`_.


Requirements
------------

You have to obtain a copy of the VST SDK by Steinberg if you want to compile it as a VST 
(It's not allowed to redistribute it, but you get it for free).
Search for vst_sdk2_4_rev2.zip on http://www.steinberg.net/en/company/developer.html and copy
the ``pluginterfaces`` and ``public.sdk`` folders into the ``vstsdk2.4`` directory.
