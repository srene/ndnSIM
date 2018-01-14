nfdc-face
=========

SYNOPSIS
--------
| nfdc face [list [[remote] <FACEURI>] [local <FACEURI>] [scheme <SCHEME>]]
| nfdc face show [id] <FACEID>
| nfdc face create [remote] <FACEURI> [[persistency] <PERSISTENCY>] [local <FACEURI>] [reliability on|off]
| nfdc face destroy [face] <FACEID|FACEURI>
| nfdc channel [list]

DESCRIPTION
-----------
In NFD, a face is the generalization of network interface.
It could be a physical network interface to communicate on a physical link,
an overlay communication channel between NFD and a remote node,
or an inter-process communication channel between NFD and a local application.

The **nfdc face list** command shows a list of faces, their properties, and statistics,
optionally filtered by remote endpoint, local endpoint, and FaceUri scheme.
When multiple filters are specified, returned faces must satisfy all filters.

The **nfdc face show** command shows properties and statistics of one specific face.

The **nfdc face create** command creates a UDP unicast, TCP, or Ethernet unicast face.
Local FaceUri is required for creating Ethernet unicast faces; otherwise it must be omitted.
The NDNLPv2 unicast reliability feature may be explicitly enabled by specifying **reliability on**
or explicitly disabled by specifying **reliability off**.
If enabled, this feature must also be enabled on the other endpoint to function properly.

The **nfdc face destroy** command destroys an existing face.

The **nfdc channel list** command shows a list of channels.
Channels are listening sockets that can accept incoming connections and create new faces.

OPTIONS
-------
<FACEID>
    Numerical identifier of the face.
    It is displayed in the output of **nfdc face list** and **nfdc face create** commands.

<FACEURI>
    A URI representing the remote or local endpoint of a face.
    Examples:

    - udp4://192.0.2.1:6363
    - udp6://[2001:db8::1]:6363
    - udp://example.net
    - tcp4://192.0.2.1:6363
    - tcp6://[2001:db8::1]:6363
    - tcp://example.net
    - unix:///var/run/nfd.sock
    - fd://6
    - ether://[08:00:27:01:01:01]
    - dev://eth0

    When a hostname is specified, a DNS query is used to obtain the IP address.

<SCHEME>
    The scheme portion of either remote or local endpoint.
    Examples:

    - udp4
    - unix
    - dev

<PERSISTENCY>
    Either "persistent" or "permanent".
    A "persistent" face (the default) is closed when a socket error occurs.
    A "permanent" face survives socket errors, and is closed only with a **nfdc destroy** command.

EXIT CODES
----------

0: Success

1: An unspecified error occurred

2: Malformed command line

3: Face not found (**nfdc face show** and **nfdc face destroy** only)

4: FaceUri canonization failed (**nfdc face create** and **nfdc face destroy** only)

5: Ambiguous: multiple matching faces are found (**nfdc face destroy** only)

SEE ALSO
--------
nfd(1), nfdc(1)
