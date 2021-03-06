# syster

  $Id: manual.txt,v 1.1 2013/08/12 20:57:54 zma0472 Exp $

Syster System Availability Metrics v.1.0 Manual
===============================================

Abstract

    Syster is a software package for unobtrusively monitoring the availability
    of the various subsystems of a POSIX operating system, and for reporting
    outages in these subsystems.  Syster is designed for use by POSIX system
    specialists.  Such specialists should find Syster easy to install,
    configure, maintain, and troubleshoot.  Syster is licensed under the terms
    of the GNU General Public License, version 2.

 1. Motivation

   1. Quantification of Levels of Service

      Organizations which provide POSIX systems to users typically guarantee
      the users to ensure a minimum level of service to be rendered.  These
      agreements are usually made binding on the provident organization in the
      terms of a Service Level Contract (SLC).  In order that the terms of an
      SLC be enforcable they must be unambiguous and their referents must be
      quantifiable.  Syster was created to provide a means of quantifying the
      level of service provided over time by a POSIX system.  These
      quantitative data may be used to assist in the arbitration of conformity
      to SLCs.

 2. Design Elements

   1. Reliability and Simplicity

      The value of the data provided by Syster is dependent upon the
      reliability of the Syster monitor programs.  The monitor programs must be
      as free from defects as possibile so that their logs produce a true
      representation of the monitored subsystems over time.  This reliability
      is achieved through simplicity of design.  Each monitor program is
      designed to perform a single, simple task.  Extraneous or redundant
      functionality is expressly avoided.  Simplicity is further pursued by the
      aggressive reuse of all common source materials.  Strict minimalism is
      enforced in all aspects of implementation.

   2. Accuracy and Precision

      The value of the data provided by Syster derives solely and entirely from
      accuracy and precision.  Data accuracy is maximized through the
      elimination of opportunities for false positive log records to occur.
      Precision is acheived by a policy of recording temporal measurements
      immediately as they are made.

   3. Unobtrusiveness

      In normal operation Syster should have no appreciable impact upon any of
      the performance characteristics of the system being monitored.  Syster's
      monitor programs are accordingly as sparing as possible of system
      resources.  Syster should require as little attention as possible from
      administrative personnel.

   4. Ease of Use

      Syster should be installed and maintained by POSIX system administrators.
      The correct and typical use of Syster should require as little special
      knowledge as possible.  To this end Syster must function in accordance
      with established preferred methods and best practices of POSIX system
      administration.

   5. Transparency and Verifiability

      The methods and internal states of each of Syster's monitor programs must
      be entirely visible to the POSIX administrator.  Each datum recorded by
      a given monitor program must be, as far as possible, dependent for its
      existence upon the functionality at the recorded point in time of the
      subsystem monitored by the program.  Syster data may thus be regarded as
      existence proof of the availability of the monitored subsystem.

   6. Orthogonality

      Syster should be implemented so as to prevent, as far as possible, a
      failure in a single subsystem from interfering with the monitoring or
      data recording functions of any other subsystem.

   7. Customizability, Modularity, and Extensibility

      Syster must not require that any particular subset of monitor programs be
      operative at any given time. Syster must enable users to select any
      subset of monitors for normal operation.  To this end, Syster should be
      implemented in a highly modular fashion.  This modularity must promote
      the extensibility of Syster in such a way that new monitoring
      functionailty may be added to Syster without imposing the need to modify
      the core Syster source materials.

   8. Portability

      Syster should be implemented in conformance to open standards so as to be
      portable to as many POSIX platforms as possible.  Syster will be written
      extirely in ANSI C and Korn Shell 88.

 3. Theory of Operation

   1. System Requirements

      1. CPU -- Syster has no special CPU requirements.  Any CPU capable of
         running a POSIX-compliant operating system should be able to run all
         of the Syster core monitors with negligible CPU overhead.

      2. Memory -- The core Syster monitors collectively require only a few
         kilobytes of system memory for operation.  The only exception to this
         might be the MEM monitor which can be configured to allocate
         arbitrarily large regions of virtual memory.

      3. Disk Space -- The Syster distribution itself requires on the order of
         one megabyte of diskspace and is located entirely within a single
         directory.  The Syster monitors generate continuous output to record
         and log files, however.  Consequently, these files will continue to
         grow in size over time if left unattended.  The task of periodically
         reducing the size of the monitor record and log files is the
         responsibility of the Syster administrator.

      4. Network -- Syster requires no networking capability for its own
         operation.  Should the administrator choose to enable the NET monitor,
         an IPv4 network must be made available.  This network must connect to
         at least one TCP echo server.

      5. Operating System -- Any POSIX operating system should be able to run
         Syster.

   2. Installation, Upgrade, and Removal

      1. Planning and preparation for the installation of a Syster instance.

         1. Diskspace -- It is highly recommended that each Syster instance
            be located in a dedicated filesystem.  While not strictly
            necessary, this practice will minimize the effect upon the system
            of disk space issues caused by Syster.  The correct amount of
            working diskspace to dedicate to a Syster instance varies as a
            function of several factors.  A Syster monitor running in debug
            mode will generate large quantities of error log output during
            normal operation.  A Syster monitor's test failures generate more
            logging output than do test successes.  A system which experiences
            a large percentage of test failures will therefore require 
            proportionately more disk space than one with a high degree of
            monitor test success.  As a general rule 100MB of diskspace should
            suffice for a system undergoing file reduction on a weekly basis.

         2. Network -- In order for the NET monitor to complete a successful
            monitor test, Syster must have IPv4 access to at least one TCP echo
            server.  The echo service is assigned IPv4 port number 7 by ICANN.
            Syster may be configured to attempt to connect to any valid IPv4
            port number.

      2. Syster Distribution -- Syster is distributed as a native package for
         several POSIX operating systems.  Syster is also distributed in source
         code form.  The native package method of installation, upgrade, and
         removal is preferred.  Syster should compile on any POSIX operating
         system with an ANSI C compiler.
         
         1. Native Package Method -- The system administrator performing the
         installation, upgrade, or removal task should consult the relevant
         system manual page for instruction on the commands and practices
         pertaining to native software package use.

         2. Source code method -- The Syster distribution requires the "make"
         program for compilation.  Once the software has been compiled, the
         Syster "install" script may be used to install the Syster software.

   3. Configuration

      Once Syster has been installed, the software is configured by means of
      editing the syster.conf file located in the "/etc" sub-directory of the
      Syster directory.  See the syster.conf(5) and syster.defaults(5) Syster
      manual pages for complete information on these files.

      Often it is necessary only to assign the IPv4 network address or hostname
      of the NET monitor echo server to the "net_echo_port" variable in the
      syster.conf file.

   4. Operation

      1. Monitors -- Syster continuously tests and records the availability of
         POSIX subsystems.  Each subsystem is monitored by a distinct,
         specialized program called a "monitor."  Each monitor is assigned a
         unique 3-letter type designation.  Syster's core monitor type
         designations and their corresponding monitor program names and
         POSIX subsystems are as follows:

         Designator    Program Name   POSIX Subsystem
            CPC         syster_cpc      Process Management
            MEM         syster_mem      Virtual Memory
            NET         syster_net      IPv4 Network
            UFS         syster_ufs      Filesystem

      2. Serialization -- Syster uses an advisory locking mechanism to ensure
         that exactly 0 or 1 monitors of a given type run concurrently.  The
         locking mechanism is enforced by the syster_ctl(8) program, and relies
         on files in the "/var/lock" subdirectory of the Syster instance
         directory.  In conformance with standard practice, each monitor
         process records its own process ID in its lockfile and removes the
         lockfile upon exit.

      3. POSIX Subsystem Testing -- Each of Syster's monitors runs as a daemon
         process, continuously testing a POSIX subsystem by accessing and using
         the subsystem in "user mode."

      4. XXX 
   5. Maintenance

   6. Troubleshooting

 4. License

 5. Distribution

 6. Future Plans

