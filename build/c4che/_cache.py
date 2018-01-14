APPNAME = 'ns'
AR = ['/usr/bin/ar']
ARCH_ST = ['-arch']
ARFLAGS = ['rcs']
BINDIR = '/usr/local/bin'
BOOST_VERSION = '1_65_1'
BUILD_PROFILE = 'debug'
BUILD_SUFFIX = '-debug'
CC = ['/usr/bin/clang']
CCDEFINES = ['_DEBUG']
CCFLAGS = ['-O0', '-ggdb', '-g3', '-Wall', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing', '-std=c++0x', '-std=c++11', '-O0', '-ggdb', '-g3', '-Wall', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing', '-Wno-unused-local-typedefs', '-Wno-potentially-evaluated-expression', '-std=c++11']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'clang'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('9', '0', '0')
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_cshlib = ['-fPIC']
COMPILER_CC = 'clang'
COMPILER_CXX = 'clang++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/clang++']
CXXDEFINES = ['_DEBUG']
CXXFLAGS = ['-std=c++0x', '-std=c++11', '-O0', '-ggdb', '-g3', '-Wall', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing', '-Wno-unused-local-typedefs', '-Wno-potentially-evaluated-expression', '-std=c++11']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_cxxshlib = ['-fPIC']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'clang'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DATADIR = '/usr/local/share'
DATAROOTDIR = '/usr/local/share'
DEFINES = ['NS3_BUILD_PROFILE_DEBUG', 'NS3_ASSERT_ENABLE', 'NS3_LOG_ENABLE', 'HAVE_SYS_IOCTL_H=1', 'HAVE_IF_NETS_H=1', 'HAVE_NET_ETHERNET_H=1', 'HAVE_IS_DEFAULT_CONSTRUCTIBLE=1', 'HAVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE=1', 'HAVE_IS_NOTHROW_MOVE_ASSIGNABLE=1', 'HAVE_STD_TO_STRING=1', 'HAVE_VECTOR_INSERT_ERASE_CONST_ITERATOR=1', 'HAVE_PTHREAD=1', 'HAVE_SQLITE3=1', 'HAVE_OPENSSL=1']
DEFINES_LIBXML2 = ['HAVE_LIBXML2=1']
DEFINES_SQLITE3 = ['HAVE_SQLITE3=1']
DEFINES_ST = '-D%s'
DEFINE_COMMENTS = {'HAVE_SYS_IOCTL_H': '', 'HAVE_IF_NETS_H': '', 'HAVE_SIGNAL_H': '', 'HAVE_OPENSSL': '', 'HAVE_SYS_TYPES_H': '', 'INT64X64_USE_128': '', 'HAVE_IS_DEFAULT_CONSTRUCTIBLE': '', 'HAVE_PACKET_H': '', 'HAVE_STDINT_H': '', 'HAVE_SQLITE3': '', 'HAVE_NET_ETHERNET_H': '', 'HAVE___UINT128_T': '', 'HAVE_DIRENT_H': '', 'HAVE_INTTYPES_H': '', 'HAVE_STDLIB_H': '', 'HAVE_PTHREAD_H': '', 'HAVE_STD_TO_STRING': '', 'HAVE_SYS_STAT_H': '', 'HAVE_IS_NOTHROW_MOVE_ASSIGNABLE': '', 'HAVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE': '', 'HAVE_GETENV': '', 'HAVE_RT': '', 'HAVE_PTHREAD': '', 'HAVE_IF_TUN_H': '', 'HAVE_SYS_INT_TYPES_H': '', 'HAVE_UINT128_T': '', 'HAVE_VECTOR_INSERT_ERASE_CONST_ITERATOR': ''}
DEST_BINFMT = 'mac-o'
DEST_CPU = 'x86_64'
DEST_OS = 'darwin'
DOCDIR = '/usr/local/share/doc/ns'
DVIDIR = '/usr/local/share/doc/ns'
ENABLE_BRITE = False
ENABLE_EMU = None
ENABLE_EXAMPLES = True
ENABLE_FDNETDEV = True
ENABLE_GSL = None
ENABLE_GTK2 = None
ENABLE_LIBXML2 = '-I/usr/local/Cellar/libxml2/2.9.7/include/libxml2 -L/usr/local/Cellar/libxml2/2.9.7/lib -lxml2\n'
ENABLE_NDNSIM = True
ENABLE_NSC = False
ENABLE_PYTHON_BINDINGS = False
ENABLE_PYVIZ = False
ENABLE_STATIC_NS3 = False
ENABLE_SUDO = False
ENABLE_TAP = None
ENABLE_TESTS = False
ENABLE_THREADING = True
EXAMPLE_DIRECTORIES = ['tutorial', 'udp', 'energy', 'matrix-topology', 'realtime', 'tcp', 'wireless', 'naming', 'traffic-control', 'error-model', 'routing', 'ipv6', 'stats', 'socket', 'udp-client-server']
EXEC_PREFIX = '/usr/local'
FRAMEWORKPATH_ST = '-F%s'
FRAMEWORK_ST = ['-framework']
HAVE_IS_DEFAULT_CONSTRUCTIBLE = True
HAVE_IS_NOTHROW_MOVE_ASSIGNABLE = True
HAVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE = True
HAVE_LIBXML2 = 1
HAVE_SQLITE3 = 1
HTMLDIR = '/usr/local/share/doc/ns'
INCLUDEDIR = '/usr/local/include'
INCLUDES_BOOST = '/usr/local/include'
INCLUDES_LIBXML2 = ['/usr/local/Cellar/libxml2/2.9.7/include/libxml2']
INCLUDES_OPENSSL = ['/usr/local/opt/openssl/include']
INFODIR = '/usr/local/share/info'
INT64X64_USE_128 = 1
LIBDIR = '/usr/local/lib'
LIBEXECDIR = '/usr/local/libexec'
LIBPATH_BOOST = ['/usr/local/lib']
LIBPATH_LIBXML2 = ['/usr/local/Cellar/libxml2/2.9.7/lib']
LIBPATH_OPENSSL = ['/usr/local/opt/openssl/lib']
LIBPATH_ST = '-L%s'
LIB_BOOST = ['boost_graph', 'boost_thread-mt', 'boost_unit_test_framework', 'boost_system', 'boost_random', 'boost_date_time', 'boost_iostreams', 'boost_regex', 'boost_program_options', 'boost_chrono', 'boost_filesystem', 'boost_system', 'boost_signals', 'boost_filesystem']
LIB_LIBXML2 = ['xml2']
LIB_OPENSSL = ['ssl', 'crypto']
LIB_PTHREAD = ['pthread']
LIB_SQLITE3 = ['sqlite3']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_cshlib = ['-dynamiclib']
LINKFLAGS_cstlib = []
LINKFLAGS_cxxshlib = ['-dynamiclib']
LINKFLAGS_cxxstlib = []
LINK_CC = ['/usr/bin/clang']
LINK_CXX = ['/usr/bin/clang++']
LOCALEDIR = '/usr/local/share/locale'
LOCALSTATEDIR = '/usr/local/var'
MANDIR = '/usr/local/share/man'
MODULES_NOT_BUILT = ['brite', 'click', 'openflow', 'tap-bridge', 'visualizer']
NS3_CONTRIBUTED_MODULES = []
NS3_ENABLED_CONTRIBUTED_MODULES = []
NS3_ENABLED_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-internet-apps', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-ndnSIM', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-test', 'ns3-topology-read', 'ns3-traffic-control', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_EXECUTABLE_PATH = ['/Users/srene/workspace/ndnSIM/build/src/fd-net-device']
NS3_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-fd-net-device', 'ns3-flow-monitor', 'ns3-internet', 'ns3-internet-apps', 'ns3-lr-wpan', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-ndnSIM', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-sixlowpan', 'ns3-spectrum', 'ns3-stats', 'ns3-test', 'ns3-topology-read', 'ns3-traffic-control', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wave', 'ns3-wifi', 'ns3-wimax']
NS3_MODULE_PATH = ['/Users/srene/workspace/ndnSIM/build']
NS3_OPTIONAL_FEATURES = [('python', 'Python Bindings', False, 'disabled by user request'), ('brite', 'BRITE Integration', False, 'BRITE not enabled (see option --with-brite)'), ('nsclick', 'NS-3 Click Integration', False, 'nsclick not enabled (see option --with-nsclick)'), ('GtkConfigStore', 'GtkConfigStore', [], "library 'gtk+-2.0 >= 2.12' not found"), ('XmlIo', 'XmlIo', '-I/usr/local/Cellar/libxml2/2.9.7/include/libxml2 -L/usr/local/Cellar/libxml2/2.9.7/lib -lxml2\n', "library 'libxml-2.0 >= 2.7' not found"), ('Threading', 'Threading Primitives', True, '<pthread.h> include not detected'), ('RealTime', 'Real Time Simulator', False, 'librt is not available'), ('FdNetDevice', 'File descriptor NetDevice', True, 'FdNetDevice module enabled'), ('TapFdNetDevice', 'Tap FdNetDevice', False, 'needs linux/if_tun.h'), ('EmuFdNetDevice', 'Emulation FdNetDevice', False, 'needs netpacket/packet.h'), ('PlanetLabFdNetDevice', 'PlanetLab FdNetDevice', False, 'PlanetLab operating system not detected (see option --force-planetlab)'), ('nsc', 'Network Simulation Cradle', False, 'NSC not found (see option --with-nsc)'), ('mpi', 'MPI Support', False, 'option --enable-mpi not selected'), ('ndnSIM', 'ndnSIM', True, ''), ('openflow', 'NS-3 OpenFlow Integration', False, 'OpenFlow not enabled (see option --with-openflow)'), ('SqliteDataOutput', 'SQlite stats data output', '-lsqlite3\n', "library 'sqlite3' not found"), ('TapBridge', 'Tap Bridge', [], '<linux/if_tun.h> include not detected'), ('PyViz', 'PyViz visualizer', False, 'Python Bindings are needed but not enabled'), ('ENABLE_SUDO', 'Use sudo to set suid bit', False, 'option --enable-sudo not selected'), ('ENABLE_TESTS', 'Tests', False, 'defaults to disabled'), ('ENABLE_EXAMPLES', 'Examples', True, 'option --enable-examples selected'), ('GSL', 'GNU Scientific Library (GSL)', [], 'GSL not found'), ('libgcrypt', 'Gcrypt library', [], 'libgcrypt not found: you can use libgcrypt-config to find its location.'), ('DES Metrics', 'DES Metrics event collection', [], 'defaults to disabled')]
OLDINCLUDEDIR = '/usr/include'
PACKAGE = 'ns'
PDFDIR = '/usr/local/share/doc/ns'
PKGCONFIG = ['/usr/local/bin/pkg-config']
PLATFORM = 'darwin'
PREFIX = '/usr/local'
PRINT_BUILT_MODULES_AT_END = False
PSDIR = '/usr/local/share/doc/ns'
REQUIRED_BOOST_LIBS = []
RPATH_ST = '-Wl,-rpath,%s'
SBINDIR = '/usr/local/sbin'
SHAREDSTATEDIR = '/usr/local/com'
SHLIB_MARKER = []
SONAME_ST = []
SQLITE_STATS = '-lsqlite3\n'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = []
STLIB_ST = '-l%s'
SUDO = ['/usr/bin/sudo']
SYSCONFDIR = '/usr/local/etc'
TAR = ['/usr/bin/tar']
VALGRIND_FOUND = False
VERSION = '3-dev'
cfg_files = ['/Users/srene/workspace/ndnSIM/build/ns3/config-store-config.h', '/Users/srene/workspace/ndnSIM/build/ns3/core-config.h', '/Users/srene/workspace/ndnSIM/build/ns3/ndnSIM/ndn-cxx/ndn-cxx-config.hpp', '/Users/srene/workspace/ndnSIM/build/ns3/ndnSIM/NFD/core/config.hpp']
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.dylib'
cstlib_PATTERN = 'lib%s.a'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.dylib'
cxxstlib_PATTERN = 'lib%s.a'
define_key = ['HAVE_SYS_IOCTL_H', 'HAVE_IF_NETS_H', 'HAVE_NET_ETHERNET_H', 'HAVE_IF_TUN_H', 'HAVE_PACKET_H', 'HAVE_IS_DEFAULT_CONSTRUCTIBLE', 'HAVE_IS_NOTHROW_MOVE_CONSTRUCTIBLE', 'HAVE_IS_NOTHROW_MOVE_ASSIGNABLE', 'HAVE_STD_TO_STRING', 'HAVE_VECTOR_INSERT_ERASE_CONST_ITERATOR', 'HAVE_PTHREAD', 'HAVE_SQLITE3', 'HAVE_OPENSSL']
macbundle_PATTERN = '%s.bundle'
