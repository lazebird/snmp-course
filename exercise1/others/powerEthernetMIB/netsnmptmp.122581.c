/* generated from net-snmp-config */
#include <net-snmp/net-snmp-config.h>
#ifdef HAVE_SIGNAL
#include <signal.h>
#endif /* HAVE_SIGNAL */
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /*  HAVE_SYS_STAT_H */
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
  #include "powerEthernetMIB.h"
const char *app_name = "a.out";
static int reconfig = 0;

extern int netsnmp_running;

RETSIGTYPE
stop_server(int a) {
    netsnmp_running = 0;
}

#ifdef SIGHUP
RETSIGTYPE
hup_handler(int sig)
{
    reconfig = 1;
    signal(SIGHUP, hup_handler);
}
#endif

static void
usage(const char *prog)
{
    fprintf(stderr,
            "USAGE: %s [OPTIONS]\n"
            "\n"
            "OPTIONS:\n", prog);

    fprintf(stderr,
            "  -c FILE[,...]\t\tread FILE(s) as configuration file(s)\n"
            "  -C\t\t\tdo not read the default configuration files\n"
            "  -d\t\t\tdump all traffic\n"
            "  -D TOKEN[,...]\tturn on debugging output for the specified "
            "TOKENs\n"
            "\t\t\t   (ALL gives extremely verbose debugging output)\n"
            "  -f\t\t\tDo not fork() from the calling shell.\n"
            "  -h\t\t\tdisplay this help message\n"
            "  -H\t\t\tdisplay a list of configuration file directives\n"
            "  -L LOGOPTS\t\tToggle various defaults controlling logging:\n");
    snmp_log_options_usage("\t\t\t  ", stderr);
#ifndef DISABLE_MIB_LOADING
    fprintf(stderr,
            "  -m MIB[" ENV_SEPARATOR "...]\t\tload given list of MIBs (ALL loads "
            "everything)\n"
            "  -M DIR[" ENV_SEPARATOR "...]\t\tlook in given list of directories for MIBs\n");
#endif /* DISABLE_MIB_LOADING */
    fprintf(stderr,
            "  -p FILE\t\tstore process id in FILE\n");
#ifndef DISABLE_MIB_LOADING
    fprintf(stderr,
            "  -P MIBOPTS\t\tToggle various defaults controlling mib "
            "parsing:\n");
    snmp_mib_toggle_options_usage("\t\t\t  ", stderr);
#endif /* DISABLE_MIB_LOADING */
    fprintf(stderr,
            "  -v\t\t\tdisplay package version number\n"
            "  -x TRANSPORT\tconnect to master agent using TRANSPORT\n");
    exit(1);
}

static void
version(void)
{
    fprintf(stderr, "NET-SNMP version: %s\n", netsnmp_get_version());
    exit(0);
}

int
main (int argc, char **argv)
{
  int arg;
  char* cp = NULL;
  int dont_fork = 0, do_help = 0;
  char* pid_file = NULL;

  while ((arg = getopt(argc, argv, "c:CdD:fhHL:"
#ifndef DISABLE_MIB_LOADING
                       "m:M:"
#endif /* DISABLE_MIB_LOADING */
                       "n:p:"
#ifndef DISABLE_MIB_LOADING
                       "P:"
#endif /* DISABLE_MIB_LOADING */
                       "vx:")) != EOF) {
    switch (arg) {
      case 'c':
        if (optarg != NULL) {
          netsnmp_ds_set_string(NETSNMP_DS_LIBRARY_ID,
                                NETSNMP_DS_LIB_OPTIONALCONFIG, optarg);
        } else {
          usage(argv[0]);
        }
        break;

      case 'C':
        netsnmp_ds_set_boolean(NETSNMP_DS_LIBRARY_ID,
                               NETSNMP_DS_LIB_DONT_READ_CONFIGS, 1);
        break;

    case 'd':
      netsnmp_ds_set_boolean(NETSNMP_DS_LIBRARY_ID,
                             NETSNMP_DS_LIB_DUMP_PACKET, 1);
      break;

    case 'D':
      debug_register_tokens(optarg);
      snmp_set_do_debugging(1);
      break;

    case 'f':
      dont_fork = 1;
      break;

    case 'h':
      usage(argv[0]);
      break;

    case 'H':
      do_help = 1;
      break;

    case 'L':
      if (snmp_log_options(optarg, argc, argv) < 0) {
        exit(1);
      }
      break;

#ifndef DISABLE_MIB_LOADING
    case 'm':
      if (optarg != NULL) {
        setenv("MIBS", optarg, 1);
      } else {
        usage(argv[0]);
      }
      break;

    case 'M':
      if (optarg != NULL) {
        setenv("MIBDIRS", optarg, 1);
      } else {
        usage(argv[0]);
      }
      break;
#endif /* DISABLE_MIB_LOADING */

    case 'n':
      if (optarg != NULL) {
        app_name = optarg;
        netsnmp_ds_set_string(NETSNMP_DS_LIBRARY_ID,
                              NETSNMP_DS_LIB_APPTYPE, app_name);
      } else {
        usage(argv[0]);
      }
      break;

    case 'p':
      if (optarg != NULL) {
        pid_file = optarg;
      } else {
        usage(argv[0]);
      }
      break;

#ifndef DISABLE_MIB_LOADING
    case 'P':
      cp = snmp_mib_toggle_options(optarg);
      if (cp != NULL) {
        fprintf(stderr, "Unknown parser option to -P: %c.\n", *cp);
        usage(argv[0]);
      }
      break;
#endif /* DISABLE_MIB_LOADING */

    case 'v':
      version();
      break;

    case 'x':
      if (optarg != NULL) {
        netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
                              NETSNMP_DS_AGENT_X_SOCKET, optarg);
      } else {
        usage(argv[0]);
      }
      break;

    default:
      fprintf(stderr, "invalid option: -%c\n", arg);
      usage(argv[0]);
      break;
    }
  }

  if (do_help) {
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID,
                           NETSNMP_DS_AGENT_NO_ROOT_ACCESS, 1);
  } else {
    /* we are a subagent */
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID,
                           NETSNMP_DS_AGENT_ROLE, 1);

    if (!dont_fork) {
      if (netsnmp_daemonize(1, snmp_stderrlog_status()) != 0)
        exit(1);
    }

#ifdef HAVE_GETPID
    if (pid_file != NULL) {
      /*
       * unlink the pid_file, if it exists, prior to open.  Without
       * doing this the open will fail if the user specified pid_file
       * already exists.
       */
      int fd;
      unlink(pid_file);
      fd = open(pid_file, O_CREAT | O_EXCL | O_WRONLY, 0600);
      if (fd == -1) {
        snmp_log_perror(pid_file);
        if (!netsnmp_ds_get_boolean(NETSNMP_DS_APPLICATION_ID,
                                    NETSNMP_DS_AGENT_NO_ROOT_ACCESS)) {
          exit(1);
        }
      } else {
        char buf[3 + sizeof(long) * 3];
        int len = snprintf(buf, sizeof(buf), "%ld\n", (long int)getpid());
        write(fd, buf, len);
        close(fd);
      }
    }
#endif

    /* initialize tcpip, if necessary */
    SOCK_STARTUP;
  }

  /* initialize the agent library */
  init_agent(app_name);

  /* initialize your mib code here */
  init_powerEthernetMIB();

  /* a.out will be used to read a.out.conf files. */
  init_snmp("a.out");

  if (do_help) {
    fprintf(stderr, "Configuration directives understood:\n");
    read_config_print_usage("  ");
    exit(0);
  }

  /* In case we received a request to stop (kill -TERM or kill -INT) */
  netsnmp_running = 1;
#ifdef SIGTERM
  signal(SIGTERM, stop_server);
#endif
#ifdef SIGINT
  signal(SIGINT, stop_server);
#endif
#ifdef SIGHUP
  signal(SIGHUP, hup_handler);
#endif

  /* main loop here... */
  while(netsnmp_running) {
    if (reconfig) {
      free_config();
      read_configs();
      reconfig = 0;
    }
    agent_check_and_process(1);
  }

  /* at shutdown time */
  snmp_shutdown(app_name);

  /* deinitialize your mib code here */

  /* shutdown the agent library */
  shutdown_agent();

  if (pid_file != NULL) {
    unlink(pid_file);
  }

  SOCK_CLEANUP;
  exit(0);
}
